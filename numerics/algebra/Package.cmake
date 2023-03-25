
set(ALGEBRA_SOURCES
  ${SOURCE_ROOT}/numerics/algebra/LaScalar.cpp
  ${SOURCE_ROOT}/numerics/algebra/LaVector.cpp
  ${SOURCE_ROOT}/numerics/algebra/LaSquareMatrix.cpp
  ${SOURCE_ROOT}/numerics/algebra/LaLinearEquation.cpp
)


SET(libName "tmcAlgebra")

# create the library
ADD_LIBRARY(
   ${libName} SHARED
   ${ALGEBRA_SOURCES}
)

SET(LINK_LIBS
    tmcCommon
)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET(LINK_LIBS ${LINK_LIBS} z)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

TARGET_LINK_LIBRARIES(${libName} ${LINK_LIBS} )
INSTALL(TARGETS ${libName} DESTINATION lib)


