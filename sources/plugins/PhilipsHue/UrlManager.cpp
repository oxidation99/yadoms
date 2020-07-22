#include "UrlManager.h"

const std::string CUrlManager::ApiPath("/api");
const std::string CUrlManager::GetAllLightsPath("/lights");
const std::string CUrlManager::DescriptionPath("/description.xml");

CUrlManager::CUrlManager(CHueBridgeDiscovery::HueInformations& hueInformations, CConfiguration& configuration)
   : m_hueInformations(hueInformations),
     m_configuration(configuration)
{
}


std::string CUrlManager::getUrlPatternPath(EUrlPattern urlPattern) const
{
   std::string urlPatternPath;
   switch (urlPattern)
   {
   case kApi:
      urlPatternPath = ApiPath;
      break;
   case kGetAllLights:
      urlPatternPath = ApiPath + "/" + m_username + GetAllLightsPath;
      break;
   default:
      urlPatternPath = "";
   }

   return urlPatternPath;
}

std::string CUrlManager::getPatternUrl(const std::string& urlPatternPath) const
{
   const auto protocolType = "http://";
   const auto ipAddress = m_configuration.getPairingMode() == EPairingMode::kAuto
                            ? m_hueInformations.ip
                            : m_configuration.getIPAddress();

   return protocolType + ipAddress + urlPatternPath;
}
