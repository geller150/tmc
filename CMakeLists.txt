CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(TMC)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeMacros.cmake)
INCLUDE(${SOURCE_ROOT}/CMakeIncludes.cmake)

#packages
INCLUDE(${SOURCE_ROOT}/common/Package.cmake)
INCLUDE(${SOURCE_ROOT}/numerics/algebra/Package.cmake)
INCLUDE(${SOURCE_ROOT}/numerics/structuralsolver/Package.cmake)
INCLUDE(${SOURCE_ROOT}/views/plot2d/Package.cmake)

#applications
INCLUDE(${SOURCE_ROOT}/applications/equationSystem/Package.cmake)
INCLUDE(${SOURCE_ROOT}/applications/csmBenchmark/Package.cmake)
INCLUDE(${SOURCE_ROOT}/applications/massOscillator/Package.cmake)

