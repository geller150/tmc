GET_FILENAME_COMPONENT( SOURCE_ROOT  ${CMAKE_CURRENT_LIST_FILE} PATH)
STRING(REGEX REPLACE "(.*)/cmake" "\\1" SOURCE_ROOT "${SOURCE_ROOT}" )
INCLUDE_DIRECTORIES(${SOURCE_ROOT})

SET(CMAKE_MODULE_PATH "${SOURCE_ROOT}/cmake" ${CMAKE_MODULE_PATH})

INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}")

CONFIGURE_FILE(${SOURCE_ROOT}/README.md ${CMAKE_INSTALL_PREFIX}/README.md @ONLY)
CONFIGURE_FILE(${SOURCE_ROOT}/LICENSE ${CMAKE_INSTALL_PREFIX}/LICENSE @ONLY)


###############################################################
# Activate IF(ARG)...ELSE()...ENDIF() in CMake
###############################################################
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS 1)

###############################################################
# definitions
###############################################################
add_definitions(-DBOOST_NO_AUTO_PTR)

###############################################################
# compilerflags
###############################################################

SET(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

IF(CMAKE_SYSTEM MATCHES "Windows")
    ADD_DEFINITIONS(-D_GLIBCXX_USE_CXX11_ABI=0)
    add_definitions(${CMAKE_CXX_FLAGS} "-Wno-strict-aliasing")
    add_definitions(${CMAKE_CXX_FLAGS} "-Wno-parentheses")
ENDIF(CMAKE_SYSTEM MATCHES "Windows")

