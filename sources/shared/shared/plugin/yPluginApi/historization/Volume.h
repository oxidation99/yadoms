#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/DoubleTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A weight historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CVolume : public CSingleHistorizableData<double>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CVolume(const std::string& keywordName,
                                const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                const EMeasureType& measureType = EMeasureType::kAbsolute,
                                const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                                const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CVolume() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
