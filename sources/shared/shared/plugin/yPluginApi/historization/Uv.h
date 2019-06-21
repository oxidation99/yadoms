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
            ///\brief A UV historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CUv : public CSingleHistorizableData<double>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  Additional information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CUv(const std::string& keywordName,
                            const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                            const EMeasureType& measureType = EMeasureType::kAbsolute,
                            typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                            const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CUv() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
