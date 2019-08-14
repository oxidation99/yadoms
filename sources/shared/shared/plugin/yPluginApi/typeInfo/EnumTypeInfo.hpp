#pragma once
#include "ITypeInfo.h"
#include <shared/enumeration/IExtendedEnum.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            //-----------------------------------------------------
            ///\brief Interface for keyword type information
            //-----------------------------------------------------
            template <class TMyExtendedEnum>
            class CEnumTypeInfo : public ITypeInfo
            {
            public:
               CEnumTypeInfo()
                  : m_data(helper<TMyExtendedEnum>::getTypeInfo())
               {
               }

               virtual ~CEnumTypeInfo() = default;

               // ITypeInfo implementation 
               CDataContainer serialize() const override
               {
                  return m_data;
               }

               // END ITypeInfo implementation 

            private:
               CDataContainer m_data;


               //-----------------------------------------------------
               ///\brief     Helpers to normalize access to simple value and enum values
               //-----------------------------------------------------
               template <typename T, class Enable = void>
               struct helper
               {
                  static CDataContainer getTypeInfo()
                  {
                     return CDataContainer();
                  }
               };

               template <typename T>
               struct helper<T, typename boost::enable_if<boost::is_base_of<enumeration::IExtendedEnum, T>>::type>
               {
                  static CDataContainer getTypeInfo()
                  {
                     CDataContainer result;
                     T value; //extended enum are ctor parameterless
                     result.set("name", value.getName());
                     result.set("values", value.getAllStrings());
                     return result;
                  }
               };
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo