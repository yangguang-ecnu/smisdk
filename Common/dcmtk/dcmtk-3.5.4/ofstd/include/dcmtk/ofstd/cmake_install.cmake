# Install script for directory: C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Work/dcmtk-3.5.4/../dcmtk-3.5.4-win32-i386")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofalgo.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofbmanip.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofcast.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofcmdln.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofconapp.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofcond.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofconfig.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofconsol.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofcrc32.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofdate.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofdatime.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/offname.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofglobal.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/oflist.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/oflogfil.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofoset.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofset.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofsetit.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofstack.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofstd.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofstdinc.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofstream.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofstring.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofthread.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/oftime.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/oftimer.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/oftypes.h")
FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/ofstd" TYPE FILE FILES "C:/Work/dcmtk-3.5.4/ofstd/include/dcmtk/ofstd/ofuoset.h")
