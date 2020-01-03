#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IdeviceConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IdeviceConfiguration()
   {}

   //--------------------------------------------------------------
   /// \brief      Load configuration data
   /// \param [in] data The new configuration
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the IP address with the port from the configuration
   /// \return     the IP address with the port
   //--------------------------------------------------------------
   virtual Poco::Net::SocketAddress getIPAddressWithSocket() const = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the password used to connect the WES
   /// \return     the password
   //--------------------------------------------------------------
   virtual std::string getPassword() const = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the user used to connect the WES
   /// \return     the user
   //--------------------------------------------------------------
   virtual std::string getUser() const = 0;

   //--------------------------------------------------------------
   /// \brief      return if authentification is active
   /// \return     true if active else false
   //--------------------------------------------------------------
   virtual bool isAuthentificationActive() const = 0;

   //--------------------------------------------------------------
   /// \brief      return the delay configured for a shutter
   /// \param      the shutter desired [0-1]
   /// \return     coeff of the 10s configured [0-9]
   //--------------------------------------------------------------
   virtual long getShutterDelay(int index) const = 0;
};