
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )


SET(COMMON_SOURCES
  ${SOURCE_ROOT}/common/utilities/TmcFileInputBinary.cpp
  ${SOURCE_ROOT}/common/utilities/TmcFileOutputBinary.cpp
  ${SOURCE_ROOT}/common/utilities/TmcFileInputASCII.cpp
  ${SOURCE_ROOT}/common/utilities/TmcFileOutputASCII.cpp
  ${SOURCE_ROOT}/common/utilities/TmcStaticPathMap.cpp
  ${SOURCE_ROOT}/common/utilities/TmcLogger.cpp
  ${SOURCE_ROOT}/common/math/TmcMath.cpp
)

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} )

ADD_LIBRARY(
   tmcCommon SHARED
   ${COMMON_SOURCES}
)

SET(LINK_LIBS
   ${Boost_LIBRARIES}
   ${VTK_LIBRARIES}
   ${PYTHON_LIBRARY}
   Qt5::Core
)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
   SET(LINK_LIBS ${LINK_LIBS} z)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

TARGET_LINK_LIBRARIES(tmcCommon ${LINK_LIBS} )
INSTALL(TARGETS tmcCommon DESTINATION lib)

