#pragma once

#include "IResultAdapter.h"
#include "SqlExtension.hpp"
#include "database/common/IResultHandler.h"

namespace database
{
   namespace common
   {
      namespace adapters
      {
         //--------------------------------------------------------------
         ///\Brief		Class which adapt in single string, using a json like output [[data],[data],....]
         //--------------------------------------------------------------
         class CHugeDataVectorAdapter : public IResultAdapterEx<std::string>
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		Constructor
            //--------------------------------------------------------------
            CHugeDataVectorAdapter();

            //--------------------------------------------------------------
            /// \Brief		Destructor
            //--------------------------------------------------------------
            virtual ~CHugeDataVectorAdapter();

            // ISQLiteResultAdapter implementation
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override;
            std::vector<std::string> getResults() override;
            // [END] ISQLiteResultAdapter implementation

            //--------------------------------------------------------------
            /// \Brief		Get the result (raw format)
            /// \return		The result (raw format) using a json like ouptut [[data],[data],....]
            //--------------------------------------------------------------
            std::string getRawResults() const;

         private:
            std::string m_internalValue;
         };
      } //namespace adapters
   } //namespace common
} //namespace database 