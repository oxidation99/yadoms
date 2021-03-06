#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationData.h>

class CManuallyDeviceFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Create a device (manually)
   /// \param [in] api                    The plugin api
   /// \param [in] request                The device information
   //--------------------------------------------------------------
   static void createDeviceManually(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, const shared::plugin::yPluginApi::IManuallyDeviceCreationData & request);
};

