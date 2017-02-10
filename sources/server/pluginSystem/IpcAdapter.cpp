#include "stdafx.h"
#include "IpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "serializers/Information.h"
#include "FromPluginHistorizer.h"
#include "shared/exception/EmptyResult.hpp"
#include <plugin_IPC/yadomsToPlugin.pb.h>

namespace pluginSystem
{
   const size_t CIpcAdapter::m_maxMessages(100);
   const size_t CIpcAdapter::m_maxMessageSize(100000);

   CIpcAdapter::CIpcAdapter(boost::shared_ptr<CYPluginApiImplementation> yPluginApi)
      : m_pluginApi(yPluginApi),
        m_id(createId()),
        m_sendMessageQueueId(m_id + ".plugin_IPC.toPlugin"),
        m_receiveMessageQueueId(m_id + ".plugin_IPC.toYadoms"),
        m_sendMessageQueueRemover(m_sendMessageQueueId),
        m_receiveMessageQueueRemover(m_receiveMessageQueueId),
        m_sendMessageQueue(boost::interprocess::create_only, m_sendMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
        m_receiveMessageQueue(boost::interprocess::create_only, m_receiveMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
        m_sendBuffer(boost::make_shared<unsigned char[]>(m_sendMessageQueue.get_max_msg_size())),
        m_messageQueueReceiveThread(boost::thread(&CIpcAdapter::messageQueueReceiveThreaded, this, yPluginApi->getPluginId()))
   {
   }

   CIpcAdapter::~CIpcAdapter()
   {
      m_messageQueueReceiveThread.interrupt();
      m_messageQueueReceiveThread.join();
   }

   std::string CIpcAdapter::id() const
   {
      return m_id;
   }

   std::string CIpcAdapter::createId()
   {
      std::stringstream ss;
      ss << "yPlugin." << boost::uuids::random_generator()();
      return ss.str();
   }

   void CIpcAdapter::messageQueueReceiveThreaded(int pluginId)
   {
      // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;

      YADOMS_LOG_CONFIGURE("plugin.IpcAdapter#" + std::to_string(pluginId));
      YADOMS_LOG(information) << "Message queue ID : " << m_id;

      try
      {
         auto message(boost::make_shared<unsigned char[]>(m_receiveMessageQueue.get_max_msg_size()));
         size_t messageSize;
         unsigned int messagePriority;
         while (true)
         {
            try
            {
               // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
               // polling and call boost::this_thread::interruption_point to exit properly
               // Note that boost::interprocess::message_queue::timed_receive requires universal time to work (can not use shared::currentTime::Provider)
               auto messageWasReceived = m_receiveMessageQueue.timed_receive(message.get(),
                                                                             m_receiveMessageQueue.get_max_msg_size(),
                                                                             messageSize,
                                                                             messagePriority,
                                                                             boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1));
               boost::this_thread::interruption_point();

               if (messageWasReceived)
                  processMessage(message, messageSize);
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(error) << "Error receiving/processing queue message : " << ex.what();
            }
         }
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         YADOMS_LOG(error) << "Error creating/using message queues (" << m_id << ") in context accessor : " << ex.what();
      }
      catch (boost::thread_interrupted&)
      {
      }
      catch (std::exception& ex)
      {
         YADOMS_LOG(error) << ex.what();
      }
      catch (...)
      {
         YADOMS_LOG(error) << "Unknown error";
      }

      YADOMS_LOG(debug) << "Close message queues";

      // Delete all global objects allocated by libprotobuf.
      google::protobuf::ShutdownProtobufLibrary();
   }

   class IpcMessagePart //TODO d�placer
   {
      struct Header
      {
         explicit Header(unsigned char partNumber, unsigned char partCount)
            : m_partNumber(partNumber),
              m_partCount(partCount)
         {
         }

         unsigned char m_partNumber;
         unsigned char m_partCount;
      };

   public:
      explicit IpcMessagePart(unsigned char partNumber,
         unsigned char partCount,
                              const unsigned char* usefulData,
                              size_t usefulSize)
         : m_header(partNumber,
                    partCount),
           m_usefulData(usefulData),
           m_usefulSize(usefulSize)
      {
         formatMessage();
      }

      virtual ~IpcMessagePart()
      {
      }

      static size_t headerSize()
      {
         return sizeof(Header);
      }

      const unsigned char* formattedMessage() const
      {
         return m_formattedMessage.get();
      }

      size_t formattedSize() const
      {
         return m_formattedSize;
      }

   private:
      void formatMessage()
      {
         m_formattedSize = sizeof(Header) + m_usefulSize;
         m_formattedMessage = boost::make_shared<unsigned char[]>(m_formattedSize);

         auto index = 0;
         m_formattedMessage[index] = m_header.m_partNumber;
         index += sizeof(m_header.m_partNumber);
         m_formattedMessage[index] = m_header.m_partCount;
         index += sizeof(m_header.m_partCount);
         memcpy(&m_formattedMessage[index], m_usefulData, m_usefulSize);
      }

      const Header m_header;
      const unsigned char* m_usefulData;
      const size_t m_usefulSize;
      boost::shared_ptr<unsigned char[]> m_formattedMessage;
      size_t m_formattedSize;
   };

   class IpcMessageCutter //TODO d�placer
   {
   public:
      explicit IpcMessageCutter(const plugin_IPC::toPlugin::msg& pbMsg,
                                const size_t maxSizePerMessage,
                                const size_t maxMessage)
      {
         if (!pbMsg.IsInitialized())
         {
            YADOMS_LOG(error) << "CIpcAdapter::send : message is not fully initialized ==> ignored";
            return;
         }

         const auto pbMessageSize = pbMsg.ByteSize();
         const auto maxUsefulPartSize = maxSizePerMessage - IpcMessagePart::headerSize();
         const auto maxUsefulSize = maxUsefulPartSize * maxMessage; // TODO pourrait �tre mis en static dans la classe pour ne pas avoir � le recalculer

         if (pbMessageSize > maxUsefulSize)
         {
            YADOMS_LOG(error) << "CIpcAdapter::send : message is too big (" << pbMsg.ByteSize() << " bytes) ==> ignored";
            return;
         }

         const auto serializedMessage = boost::make_shared<unsigned char[]>(pbMessageSize);
         if (!pbMsg.SerializeToArray(serializedMessage.get(), pbMessageSize))
         {
            YADOMS_LOG(error) << "CIpcAdapter::send : fail to serialize message ==> ignored";
            return;
         }

         const auto nbParts = pbMessageSize / maxUsefulPartSize + (pbMessageSize % maxUsefulPartSize ? 1 : 0);
         for (auto idxPart = 0; idxPart < nbParts; ++idxPart)
         {
            const auto serializedPartOffset = idxPart * maxUsefulPartSize;
            m_parts.push_back(IpcMessagePart(idxPart,
                                             nbParts,
                                             &serializedMessage.get()[serializedPartOffset],
                                             (pbMessageSize - serializedPartOffset) > maxUsefulPartSize ? maxUsefulPartSize : (pbMessageSize - serializedPartOffset)));
         }
      }

      virtual ~IpcMessageCutter()
      {
      }

      bool valid() const
      {
         return !m_parts.empty();
      }

      const std::vector<IpcMessagePart>& parts() const
      {
         return m_parts;
      }

   private:
      std::vector<IpcMessagePart> m_parts;
   };

   void CIpcAdapter::send(const plugin_IPC::toPlugin::msg& pbMsg)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);

      IpcMessageCutter messageCutter(pbMsg,
                                     m_sendMessageQueue.get_max_msg_size(),
                                     m_maxMessages);

      if (!messageCutter.valid())
      {
         YADOMS_LOG(error) << "CIpcAdapter::send : message is not valid ==> ignored)";
         return;
      }

      YADOMS_LOG(trace) << "[SEND] message " << pbMsg.OneOf_case() << " to plugin instance #" << m_pluginApi->getPluginId();
      for (const auto& part:messageCutter.parts())
      {
         m_sendMessageQueue.send(part.formattedMessage(),
                                 part.formattedSize(),
                                 0);
      }
   }

   void CIpcAdapter::send(const plugin_IPC::toPlugin::msg& pbMsg,
                          boost::function1<bool, const plugin_IPC::toYadoms::msg&> checkExpectedMessageFunction,
                          boost::function1<void, const plugin_IPC::toYadoms::msg&> onReceiveFunction,
                          const boost::posix_time::time_duration& timeout)
   {
      shared::event::CEventHandler receivedEvtHandler;

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         m_onReceiveHook = [&](const plugin_IPC::toYadoms::msg& receivedMsg)-> bool
            {
               if (!checkExpectedMessageFunction(receivedMsg))
                  return false;

               receivedEvtHandler.postEvent<const plugin_IPC::toYadoms::msg>(shared::event::kUserFirstId, receivedMsg);
               return true;
            };
      }

      send(pbMsg);

      if (receivedEvtHandler.waitForEvents(timeout) == shared::event::kTimeout)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         m_onReceiveHook.clear();
         throw std::runtime_error((boost::format("No answer from plugin when sending message %1%") % pbMsg.OneOf_case()).str());
      }

      onReceiveFunction(receivedEvtHandler.getEventData<const plugin_IPC::toYadoms::msg>());
   }

   void CIpcAdapter::processMessage(boost::shared_ptr<const unsigned char[]> message, size_t messageSize)
   {
      if (messageSize < 1)
         throw shared::exception::CInvalidParameter("messageSize");

      // Unserialize message
      plugin_IPC::toYadoms::msg toYadomsProtoBuffer;
      if (!toYadomsProtoBuffer.ParseFromArray(message.get(), messageSize))
         throw shared::exception::CInvalidParameter("message : fail to parse received data into protobuf format");

      YADOMS_LOG(trace) << "[RECEIVE] message " << toYadomsProtoBuffer.OneOf_case() << " from plugin instance #" << m_pluginApi->getPluginId() << (m_onReceiveHook ? " (onReceiveHook ENABLED)" : "");

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         if (m_onReceiveHook && m_onReceiveHook(toYadomsProtoBuffer))
         {
            m_onReceiveHook.clear();
            return;
         }
      }

      // Process message
      switch (toYadomsProtoBuffer.OneOf_case())
      {
      case plugin_IPC::toYadoms::msg::kPluginState: processSetPluginState(toYadomsProtoBuffer.pluginstate());
         break;
      case plugin_IPC::toYadoms::msg::kConfigurationRequest: processGetConfiguration(toYadomsProtoBuffer.configurationrequest());
         break;
      case plugin_IPC::toYadoms::msg::kDeviceExists: processDeviceExistsRequest(toYadomsProtoBuffer.deviceexists());
         break;
      case plugin_IPC::toYadoms::msg::kDeviceDetails: processDeviceDetailsRequest(toYadomsProtoBuffer.devicedetails());
         break;
      case plugin_IPC::toYadoms::msg::kUpdateDeviceDetails: processUpdateDeviceDetails(toYadomsProtoBuffer.updatedevicedetails());
         break;
      case plugin_IPC::toYadoms::msg::kKeywordExists: processKeywordExistsRequest(toYadomsProtoBuffer.keywordexists());
         break;
      case plugin_IPC::toYadoms::msg::kDeclareDevice: processDeclareDevice(toYadomsProtoBuffer.declaredevice());
         break;
      case plugin_IPC::toYadoms::msg::kDeclareKeyword: processDeclareKeyword(toYadomsProtoBuffer.declarekeyword());
         break;
      case plugin_IPC::toYadoms::msg::kRecipientValueRequest: processRecipientValueRequest(toYadomsProtoBuffer.recipientvaluerequest());
         break;
      case plugin_IPC::toYadoms::msg::kFindRecipientsFromFieldRequest: processFindRecipientsFromFieldRequest(toYadomsProtoBuffer.findrecipientsfromfieldrequest());
         break;
      case plugin_IPC::toYadoms::msg::kRecipientFieldExitsRequest: processRecipientFieldExitsRequest(toYadomsProtoBuffer.recipientfieldexitsrequest());
         break;
      case plugin_IPC::toYadoms::msg::kHistorizeData: processHistorizeData(toYadomsProtoBuffer.historizedata());
         break;
      case plugin_IPC::toYadoms::msg::kYadomsInformationRequest: processYadomsInformationRequest(toYadomsProtoBuffer.yadomsinformationrequest());
         break;
      case plugin_IPC::toYadoms::msg::kRemoveDevice: processRemoveDeviceRequest(toYadomsProtoBuffer.removedevice());
         break;
      case plugin_IPC::toYadoms::msg::kAllDevicesRequest: processAllDevicesRequest(toYadomsProtoBuffer.alldevicesrequest());
         break;
      case plugin_IPC::toYadoms::msg::kRemoveKeyword: processRemoveKeywordRequest(toYadomsProtoBuffer.removekeyword());
         break;
      case plugin_IPC::toYadoms::msg::kAllKeywordsRequest: processAllKeywordsRequest(toYadomsProtoBuffer.allkeywordsrequest());
         break;
      case plugin_IPC::toYadoms::msg::kDeclareKeywords: processDeclareKeywords(toYadomsProtoBuffer.declarekeywords());
         break;
      case plugin_IPC::toYadoms::msg::kDeviceModelRequest: processDeviceModelRequest(toYadomsProtoBuffer.devicemodelrequest());
         break;
      case plugin_IPC::toYadoms::msg::kUpdateDeviceModel: processUpdateDeviceModel(toYadomsProtoBuffer.updatedevicemodel());
         break;
      case plugin_IPC::toYadoms::msg::kDeviceConfigurationRequest: processDeviceConfigurationRequest(toYadomsProtoBuffer.deviceconfigurationrequest());
         break;
      case plugin_IPC::toYadoms::msg::kUpdateDeviceConfiguration: processUpdateDeviceConfiguration(toYadomsProtoBuffer.updatedeviceconfiguration());
         break;
      default:
         throw shared::exception::CInvalidParameter((boost::format("message : unknown message type %1%") % toYadomsProtoBuffer.OneOf_case()).str());
      }
   }

   void CIpcAdapter::processSetPluginState(const plugin_IPC::toYadoms::SetPluginState& msg) const
   {
      shared::plugin::yPluginApi::historization::EPluginState state;
      switch (msg.pluginstate())
      {
      case plugin_IPC::toYadoms::SetPluginState_EPluginState_kUnknown: state = shared::plugin::yPluginApi::historization::EPluginState::kUnknownValue;
         break;
      case plugin_IPC::toYadoms::SetPluginState_EPluginState_kError: state = shared::plugin::yPluginApi::historization::EPluginState::kErrorValue;
         break;
      case plugin_IPC::toYadoms::SetPluginState_EPluginState_kStopped: state = shared::plugin::yPluginApi::historization::EPluginState::kStoppedValue;
         break;
      case plugin_IPC::toYadoms::SetPluginState_EPluginState_kRunning: state = shared::plugin::yPluginApi::historization::EPluginState::kRunningValue;
         break;
      case plugin_IPC::toYadoms::SetPluginState_EPluginState_kCustom: state = shared::plugin::yPluginApi::historization::EPluginState::kCustomValue;
         break;
      default:
         throw std::out_of_range((boost::format("Unsupported plugin state received : %1%") % msg.pluginstate()).str());
      }

      shared::CDataContainer dc(msg.custommessagedata());

      auto values = dc.get<std::map<std::string, std::string>>();

      m_pluginApi->setPluginState(state, msg.custommessageid(), values);
   }

   void CIpcAdapter::processGetConfiguration(const plugin_IPC::toYadoms::ConfigurationRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_configurationanswer();
      answer->set_configuration(m_pluginApi->getConfiguration().serialize());
      send(ans);
   }

   void CIpcAdapter::processDeviceExistsRequest(const plugin_IPC::toYadoms::DeviceExitsRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_deviceexists();

      answer->set_exists(m_pluginApi->deviceExists(msg.device()));
      send(ans);
   }

   void CIpcAdapter::processDeviceDetailsRequest(const plugin_IPC::toYadoms::DeviceDetailsRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      try
      {
         auto answer = ans.mutable_devicedetails();
         answer->set_details(m_pluginApi->getDeviceDetails(msg.device()).serialize());
      }
      catch (const std::exception& e)
      {
         ans.set_error("Fail to get device details : " + std::string(e.what()));
      }
      send(ans);
   }

   void CIpcAdapter::processUpdateDeviceDetails(const plugin_IPC::toYadoms::UpdateDeviceDetails& msg) const
   {
      m_pluginApi->updateDeviceDetails(msg.device(),
                                       shared::CDataContainer(msg.details()));
   }

   void CIpcAdapter::processAllDevicesRequest(const plugin_IPC::toYadoms::AllDevicesRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_alldevicesanswer();
      auto devices = m_pluginApi->getAllDevices();
      std::copy(devices.begin(), devices.end(), RepeatedFieldBackInserter(answer->mutable_devices()));
      send(ans);
   }

   void CIpcAdapter::processKeywordExistsRequest(const plugin_IPC::toYadoms::KeywordExitsRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_keywordexists();
      answer->set_exists(m_pluginApi->keywordExists(msg.device(), msg.keyword()));
      send(ans);
   }

   void CIpcAdapter::processDeclareDevice(const plugin_IPC::toYadoms::DeclareDevice& msg) const
   {
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> keywords;
      for (auto keyword = msg.keywords().begin(); keyword != msg.keywords().end(); ++keyword)
         keywords.push_back(boost::make_shared<CFromPluginHistorizer>(*keyword));

      m_pluginApi->declareDevice(msg.device(),
                                 msg.model(),
                                 keywords,
                                 msg.details().empty() ? shared::CDataContainer::EmptyContainer : shared::CDataContainer(msg.details()));
   }

   void CIpcAdapter::processDeclareKeyword(const plugin_IPC::toYadoms::DeclareKeyword& msg) const
   {
      m_pluginApi->declareKeyword(msg.device(),
                                  boost::make_shared<CFromPluginHistorizer>(msg.keyword()),
                                  msg.details().empty() ? shared::CDataContainer::EmptyContainer : shared::CDataContainer(msg.details()));
   }

   void CIpcAdapter::processRecipientValueRequest(const plugin_IPC::toYadoms::RecipientValueRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      try
      {
         auto answer = ans.mutable_recipientvalue();
         answer->set_value(m_pluginApi->getRecipientValue(msg.recipientid(), msg.fieldname()));
      }
      catch (const std::exception& e)
      {
         ans.set_error("Fail to get recipient value : " + std::string(e.what()));
      }
      send(ans);
   }

   void CIpcAdapter::processFindRecipientsFromFieldRequest(const plugin_IPC::toYadoms::FindRecipientsFromFieldRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_findrecipientsfromfieldanswer();
      auto recipientIds = m_pluginApi->findRecipientsFromField(msg.fieldname(), msg.expectedfieldvalue());
      std::copy(recipientIds.begin(), recipientIds.end(), RepeatedFieldBackInserter(answer->mutable_recipientids()));
      send(ans);
   }

   void CIpcAdapter::processRecipientFieldExitsRequest(const plugin_IPC::toYadoms::RecipientFieldExitsRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_recipientfieldexitsanswer();
      answer->set_exists(m_pluginApi->recipientFieldExists(msg.fieldname()));
      send(ans);
   }

   void CIpcAdapter::processHistorizeData(const plugin_IPC::toYadoms::HistorizeData& msg) const
   {
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> dataVect;
      for (auto value = msg.value().begin(); value != msg.value().end(); ++value)
      {
         dataVect.push_back(boost::make_shared<CFromPluginHistorizer>(value->historizable(), value->formattedvalue()));
      }
      m_pluginApi->historizeData(msg.device(), dataVect);
   }

   void CIpcAdapter::processYadomsInformationRequest(const plugin_IPC::toYadoms::YadomsInformationRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_yadomsinformationanswer();
      auto yadomsInformation = m_pluginApi->getYadomsInformation();
      answer->set_developpermode(yadomsInformation->developperMode());
      answer->set_version(yadomsInformation->version().toString());

      try
      {
         auto longitude = yadomsInformation->location()->longitude();
         auto latitude = yadomsInformation->location()->latitude();
         auto altitude = yadomsInformation->location()->altitude();

         auto location = answer->mutable_location();

         location->set_longitude(longitude);
         location->set_latitude(latitude);
         location->set_altitude(altitude);
      }
      catch (shared::exception::CEmptyResult&)
      {
         // Location unknown
      }

      send(ans);
   }

   void CIpcAdapter::processRemoveDeviceRequest(const plugin_IPC::toYadoms::RemoveDevice& msg) const
   {
      m_pluginApi->removeDevice(msg.device());
   }

   void CIpcAdapter::processRemoveKeywordRequest(const plugin_IPC::toYadoms::RemoveKeyword& msg) const
   {
      m_pluginApi->removeKeyword(msg.device(),
                                 msg.keyword());
   }

   void CIpcAdapter::processAllKeywordsRequest(const plugin_IPC::toYadoms::AllKeywordsRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_allkeywordsanswer();
      auto keywords = m_pluginApi->getAllKeywords(msg.device());
      std::copy(keywords.begin(), keywords.end(), RepeatedFieldBackInserter(answer->mutable_keywords()));
      send(ans);
   }

   void CIpcAdapter::processDeclareKeywords(const plugin_IPC::toYadoms::DeclareKeywords& msg) const
   {
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> keywords;
      for (auto keyword = msg.keywords().begin(); keyword != msg.keywords().end(); ++keyword)
         keywords.push_back(boost::make_shared<CFromPluginHistorizer>(*keyword));

      m_pluginApi->declareKeywords(msg.device(),
                                   keywords);
   }

   void CIpcAdapter::processDeviceModelRequest(const plugin_IPC::toYadoms::DeviceModelRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_devicemodelanswer();
      answer->set_model(m_pluginApi->getDeviceModel(msg.device()));
      send(ans);
   }

   void CIpcAdapter::processUpdateDeviceModel(const plugin_IPC::toYadoms::UpdateDeviceModel& msg) const
   {
      m_pluginApi->updateDeviceModel(msg.device(),
                                     msg.model());
   }

   void CIpcAdapter::processDeviceConfigurationRequest(const plugin_IPC::toYadoms::DeviceConfigurationRequest& msg)
   {
      plugin_IPC::toPlugin::msg ans;
      auto answer = ans.mutable_deviceconfigurationanswer();
      answer->set_configuration(m_pluginApi->getDeviceConfiguration(msg.device()).serialize());
      send(ans);
   }

   void CIpcAdapter::processUpdateDeviceConfiguration(const plugin_IPC::toYadoms::UpdateDeviceConfiguration& msg) const
   {
      m_pluginApi->updateDeviceConfiguration(msg.device(),
                                             shared::CDataContainer(msg.configuration()));
   }

   void CIpcAdapter::postStopRequest()
   {
      plugin_IPC::toPlugin::msg msg;
      auto message = msg.mutable_system();
      message->set_type(plugin_IPC::toPlugin::System_EventType_kRequestStop);

      send(msg);
   }

   void CIpcAdapter::postInit(boost::shared_ptr<const shared::plugin::information::IInformation> information,
                              const boost::filesystem::path& dataPath,
                              const boost::filesystem::path& logFile,
                              const std::string& logLevel)
   {
      plugin_IPC::toPlugin::msg msg;
      auto message = msg.mutable_init();
      serializers::CInformation(information).toPb(message->mutable_plugininformation());
      message->set_datapath(dataPath.string());
      message->set_logfile(logFile.string());
      message->set_loglevel(logLevel);

      send(msg);
   }

   void CIpcAdapter::postUpdateConfiguration(const shared::CDataContainer& newConfiguration)
   {
      plugin_IPC::toPlugin::msg msg;
      auto message = msg.mutable_updateconfiguration();
      message->set_configuration(newConfiguration.serialize());
      send(msg);
   }

   void CIpcAdapter::postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request)
   {
      plugin_IPC::toPlugin::msg req;
      auto message = req.mutable_bindingquery();
      message->set_query(request->getData().getQuery());

      auto success = false;
      std::string result;

      try
      {
         send(req,
              [&](const plugin_IPC::toYadoms::msg& ans) -> bool
              {
                 return ans.has_bindingqueryanswer();
              },
              [&](const plugin_IPC::toYadoms::msg& ans) -> void
              {
                 success = ans.bindingqueryanswer().success();
                 result = ans.bindingqueryanswer().result();
              });
      }
      catch (std::exception& e)
      {
         request->sendError((boost::format("Plugin doesn't answer to binding query : %1%") % e.what()).str());
         return;
      }

      if (success)
         request->sendSuccess(shared::CDataContainer(result));
      else
         request->sendError(result);
   }

   void CIpcAdapter::postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request)
   {
      plugin_IPC::toPlugin::msg req;
      auto message = req.mutable_deviceconfigurationschemarequest();
      message->set_device(request->device());

      auto success = false;
      std::string result;

      try
      {
         send(req,
              [&](const plugin_IPC::toYadoms::msg& ans) -> bool
              {
                 return ans.has_deviceconfigurationschemaanswer();
              },
              [&](const plugin_IPC::toYadoms::msg& ans) -> void
              {
                 success = ans.deviceconfigurationschemaanswer().success();
                 result = ans.deviceconfigurationschemaanswer().result();
              });
      }
      catch (std::exception& e)
      {
         request->sendError((boost::format("Plugin doesn't answer to device configuration schema request : %1%") % e.what()).str());
         return;
      }

      if (success)
         request->sendSuccess(shared::CDataContainer(result));
      else
         request->sendError(result);
   }

   void CIpcAdapter::postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration>& command)
   {
      plugin_IPC::toPlugin::msg msg;
      auto message = msg.mutable_setdeviceconfiguration();
      message->set_device(command->device());
      message->set_configuration(command->configuration().serialize());
      send(msg);
   }

   void CIpcAdapter::postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand)
   {
      plugin_IPC::toPlugin::msg msg;
      auto message = msg.mutable_devicecommand();
      message->set_device(deviceCommand->getDevice());
      message->set_keyword(deviceCommand->getKeyword());
      message->set_body(deviceCommand->getBody());
      send(msg);
   }

   void CIpcAdapter::postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery)
   {
      plugin_IPC::toPlugin::msg req;
      auto message = req.mutable_extraquery();
      message->set_query(extraQuery->getData().query());
      message->set_data(extraQuery->getData().data().serialize());

      auto success = false;
      std::string result;

      try
      {
         send(req,
              [&](const plugin_IPC::toYadoms::msg& ans) -> bool
              {
                 return ans.has_extraqueryanswer();
              },
              [&](const plugin_IPC::toYadoms::msg& ans) -> void
              {
                 success = ans.extraqueryanswer().success();
                 result = ans.extraqueryanswer().result();
              });
      }
      catch (std::exception& e)
      {
         extraQuery->sendError((boost::format("Plugin doesn't answer to extra query : %1%") % e.what()).str());
      }

      if (success)
         extraQuery->sendSuccess(shared::CDataContainer(result));
      else
         extraQuery->sendError(result);
   }

   void CIpcAdapter::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request)
   {
      plugin_IPC::toPlugin::msg req;
      auto message = req.mutable_manuallydevicecreation();
      message->set_name(request->getData().getDeviceName());
      message->set_configuration(request->getData().getConfiguration().serialize());

      auto success = false;
      std::string result;

      try
      {
         send(req,
              [&](const plugin_IPC::toYadoms::msg& ans) -> bool
              {
                 return ans.has_manuallydevicecreationanswer();
              },
              [&](const plugin_IPC::toYadoms::msg& ans) -> void
              {
                 success = ans.manuallydevicecreationanswer().has_sucess();
                 result = success ?
                             ans.manuallydevicecreationanswer().sucess().newdevicename() :
                             ans.manuallydevicecreationanswer().error().message();
              });
      }
      catch (std::exception& e)
      {
         request->sendError((boost::format("Plugin doesn't answer to binding query : %1%") % e.what()).str());
         return;
      }

      if (success)
         request->sendSuccess(result);
      else
         request->sendError(result);
   }

   void CIpcAdapter::postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event)
   {
      plugin_IPC::toPlugin::msg msg;
      auto message = msg.mutable_deviceremoved();
      message->set_device(event->device());
      message->set_details(event->details().serialize());
      send(msg);
   }
} // namespace pluginSystem


