#include "stdafx.h"
#include "PowerFactor.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CPowerFactor::CPowerFactor(const std::string& keywordName,
                                       const EKeywordAccessMode& accessMode,
                                       const EMeasureType& measureType,
                                       const typeInfo::CDoubleTypeInfo& additionalInfo,
                                       const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::PowerFactor(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo,
                                                 historyDepth)
            {
            }

            double CPowerFactor::Normalize(double value)
            {
               if (value > 1.0)
                  return 1.0;
               if (value < -1.0)
                  return -1.0;
               return value;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
