# Install script for directory: C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/dcmimgle" TYPE FILE FILES
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dcmimage.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dibaslut.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diciefn.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dicielut.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dicrvfit.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/didislut.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/didispfn.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/didocu.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diflipt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/digsdfn.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/digsdlut.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diimage.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diinpx.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diinpxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diluptab.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimo1img.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimo2img.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimocpt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimoflt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimoimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimoipxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimomod.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimoopx.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimoopxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimopx.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimopxt.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimorot.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dimosct.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diobjcou.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diovdat.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diovlay.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diovlimg.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diovpln.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dipixel.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diplugin.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dipxrept.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diregbas.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/dirotat.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/discalet.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/displint.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/ditranst.h"
    "C:/SW/dcmtk-3.5.4/dcmtk-3.5.4/dcmimgle/include/dcmtk/dcmimgle/diutils.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

