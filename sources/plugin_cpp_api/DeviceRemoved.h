#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>


namespace plugin_cpp_api
{
   class CDeviceRemoved : public shared::plugin::yPluginApi::IDeviceRemoved
   {
   public:
      explicit CDeviceRemoved(const plugin_IPC::toPlugin::DeviceRemoved& msg);
      virtual ~CDeviceRemoved();

      // IDeviceRemoved Implementation
      const std::string& device() const override;
      const boost::shared_ptr<shared::CDataContainer>& details() const override;
      // [END] IDeviceRemoved Implementation

   private:

      const std::string m_device;
      const boost::shared_ptr<shared::CDataContainer> m_details;
   };
} // namespace plugin_cpp_api	
