
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(TELEINFO_SOURCES
   Decoder.h
   Decoder.cpp
   GPIOManager.hpp
   IDecoder.h
   IGPIOManager.h
   ITeleInfoConfiguration.h
   ITeleInfoMessage.h
   stdafx.h
   stdafx.cpp
   TeleInfo.h
   TeleInfo.cpp
   TeleInfoReceiveBufferHandler.h
   TeleInfoReceiveBufferHandler.cpp
   Keywords/ForecastTomorrow.cpp
   Keywords/ForecastTomorrow.h
   Keywords/IKeyword.h
   Keywords/KeywordException.hpp
   Keywords/RunningPeriod.cpp
   Keywords/RunningPeriod.h
   specificHistorizer/Color.h
   specificHistorizer/Color.cpp   
   specificHistorizer/Period.h
   specificHistorizer/Period.cpp
   IFTD2XManager.h
   ftdi/ftd2xx.h
)

IF (WIN32)
    set(TELEINFO_SOURCES ${TELEINFO_SOURCES}
	# Add Windows sources
        Windows/FT2xxSerialPort.h
        Windows/FT2xxSerialPort.cpp
        Windows/TeleInfoFactory.h
        Windows/TeleInfoFactory.cpp
        Windows/TeleInfoConfiguration.h
        Windows/TeleInfoConfiguration.cpp
	)
	
	source_group(sources\\Windows Windows/*.*)
	
ENDIF()

IF (UNIX)
    set(TELEINFO_SOURCES ${TELEINFO_SOURCES}
	     # Add Common Linux and RaspberryPI sources
        Linux/TeleInfoFactory.h
        Linux/TeleInfoFactory.cpp
        Linux/TeleInfoConfiguration.h
        Linux/TeleInfoConfiguration.cpp        
	)
	
	source_group(sources\\Linux Linux/*.*)
ENDIF()

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\Keywords Keywords/*.*)
source_group(sources\\specificHistorizer specificHistorizer/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################

IF(WIN32)
   set(TELEINFO_ADDITIONAL_INC_DIR
	   ${CMAKE_CURRENT_SOURCE_DIR}/Windows
   )
ENDIF()
	
IF (UNIX)
   set(TELEINFO_ADDITIONAL_INC_DIR
      ${CMAKE_CURRENT_SOURCE_DIR}/Linux
   )
ENDIF()

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################


#########################################
## Always leave this three lines to configure plugin correctly
## even if TELEINFO_ADDITIONAL_INC_DIR or TELEINFO_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(teleinfo ${TELEINFO_SOURCES})
PLUGIN_INCLDIR(teleinfo ${TELEINFO_ADDITIONAL_INC_DIR})
PLUGIN_LINK(teleinfo ${TELEINFO_ADDITIONAL_LIB})

#########################################
## post-build commands
## copy files to build directory
## Use PLUGIN_POST_BUILD_COPY_FILE and PLUGIN_POST_BUILD_COPY_DIRECTORY macros
## to copy extra-files to the binary folder
#########################################
IF(WIN32)
   PLUGIN_POST_BUILD_COPY_FILE(teleinfo ftdi/ftd2xx.dll)
ENDIF()
