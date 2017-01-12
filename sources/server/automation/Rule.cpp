#include "stdafx.h"
#include "Rule.h"
#include "script/YScriptApiImplementation.h"
#include "script/Properties.h"
#include "script/IpcAdapter.h"
#include "RuleLogger.h"

namespace automation
{
   CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
                boost::shared_ptr<interpreter::IManager> interpreterManager,
                boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                boost::shared_ptr<script::IGeneralInfo> generalInfo)
      : m_ruleData(ruleData),
        m_interpreterManager(interpreterManager)
   {
      start(pluginGateway,
            dbAcquisitionRequester,
            dbDeviceRequester,
            keywordAccessLayer,
            dbRecipientRequester,
            generalInfo);
   }

   CRule::~CRule()
   {
      m_interpreterManager->getRuleLogDispatcher()->remove(m_scriptLogger);
   }

   void CRule::start(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                     boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                     boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                     boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                     boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                     boost::shared_ptr<script::IGeneralInfo> generalInfo)
   {
      m_scriptLogger = createScriptLogger(m_interpreterManager->getScriptLogFile(m_ruleData->Id()));
      m_interpreterManager->getRuleLogDispatcher()->add(m_scriptLogger);

      auto apiImplementation = createScriptApiImplementation(pluginGateway,
                                                             dbAcquisitionRequester,
                                                             dbDeviceRequester,
                                                             keywordAccessLayer,
                                                             dbRecipientRequester,
                                                             generalInfo,
                                                             m_scriptLogger);

      m_ipcAdapter = createScriptIpcAdapter(m_ruleData->Id(),
                                            apiImplementation);

      script::CProperties ruleProperties(m_ruleData);

      m_scriptInterpreter = m_interpreterManager->getInterpreterInstance(ruleProperties.interpreterName());

      m_scriptInterpreter->startScript(m_ruleData->Id(),
                                       ruleProperties.scriptPath(),
                                       m_ipcAdapter->id());
   }

   boost::shared_ptr<IRuleLogger> CRule::createScriptLogger(const std::string& scriptLogFile) const
   {
      return boost::make_shared<CRuleLogger>(scriptLogFile);
   }

   void CRule::requestStop()
   {
      m_scriptInterpreter->stopScript(m_ruleData->Id());
   }

   boost::shared_ptr<script::IIpcAdapter> CRule::createScriptIpcAdapter(int ruleId,
                                                                        boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> apiImplementation) const
   {
      return boost::make_shared<script::CIpcAdapter>(apiImplementation,
                                                     ruleId);
   }

   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> CRule::createScriptApiImplementation(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                                                                                                   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                                                                                                   boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                                                                                                   boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                                                                                                   boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                                                                                                   boost::shared_ptr<script::IGeneralInfo> generalInfo,
                                                                                                   boost::shared_ptr<IRuleLogger> scriptLogger) const
   {
      return boost::make_shared<script::CYScriptApiImplementation>(scriptLogger,
                                                                   pluginGateway,
                                                                   dbAcquisitionRequester,
                                                                   dbDeviceRequester,
                                                                   keywordAccessLayer,
                                                                   dbRecipientRequester,
                                                                   generalInfo);
   }
} // namespace automation	


