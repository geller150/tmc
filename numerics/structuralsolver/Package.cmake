
set(SOURCES
  ${SOURCE_ROOT}/numerics/structuralsolver/TmcInitialValueSolver.cpp
  ${SOURCE_ROOT}/numerics/structuralsolver/TmcInitialValue3rdOrderSolver.cpp
  ${SOURCE_ROOT}/numerics/structuralsolver/beam/TmcBeam.cpp
  ${SOURCE_ROOT}/numerics/structuralsolver/beam/TmcBeamSystem.cpp
  ${SOURCE_ROOT}/numerics/structuralsolver/massoscillator/TmcMassOscillator.cpp
)


SET(libName "tmcStructuralSolver")

# create the library
ADD_LIBRARY(
   ${libName} SHARED
   ${SOURCES}
)

SET(LINK_LIBS
    tmcCommon
    tmcAlgebra
)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET(LINK_LIBS ${LINK_LIBS} z)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

TARGET_LINK_LIBRARIES(${libName} ${LINK_LIBS} )
INSTALL(TARGETS ${libName} DESTINATION lib)


