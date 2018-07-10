#pragma once
#include <shared/plugin/yPluginApi/historization/typeInfo/ByteArrayTypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Interface for keyword type information
//-----------------------------------------------------
class CByteArrayTypeInfo : public shared::plugin::yPluginApi::historization::typeInfo::CByteArrayTypeInfo
{
public:
   //-----------------------------------------------------
   ///\brief   Constructor
   //-----------------------------------------------------
   explicit CByteArrayTypeInfo(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~CByteArrayTypeInfo();

   // shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo implementation 
   shared::CDataContainer serialize() const override;
   // END shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo implementation 

private:
   //-----------------------------------------------------
   ///\brief   Generate typeInfo content
   ///\param [in] vID   The openzwave ValueID
   //-----------------------------------------------------
   void initialize(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   The typeInfo content
   //-----------------------------------------------------
   shared::CDataContainer m_data;
};

