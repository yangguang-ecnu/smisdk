# Install script for directory: C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/../dcmtk-3.5.4-win32-i386")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/dcmimage" TYPE FILE FILES
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diargimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diargpxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicmyimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicmypxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicocpt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicoflt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicoimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicomot.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicoopx.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicoopxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicopx.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicopxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicorot.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dicosct.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dihsvimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dihsvpxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dipalimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dipalpxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dipipng.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dipitiff.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqtcmap.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqtctab.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqtfs.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqthash.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqthitl.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqthitm.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqtid.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqtpbox.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqtpix.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqtstab.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diqttype.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diquant.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diregist.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dirgbimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/dirgbpxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diybrimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diybrpxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diyf2img.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diyf2pxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diyp2img.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimage/include/dcmtk/dcmimage/diyp2pxt.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

