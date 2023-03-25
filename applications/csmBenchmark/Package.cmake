
include_directories(
  ${SOURCE_ROOT}
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)


SET(EXEC_NAME csmBenchmark)
IF(NOT CMAKE_SYSTEM MATCHES "Windows")
    SET(EXEC_NAME ${EXEC_NAME}.exe)
ENDIF(NOT CMAKE_SYSTEM MATCHES "Windows")
ADD_EXECUTABLE(${EXEC_NAME}
       ${SOURCE_ROOT}/applications/csmBenchmark/CSM1B.cpp
       ${SOURCE_ROOT}/applications/csmBenchmark/CSM2B.cpp
       ${SOURCE_ROOT}/applications/csmBenchmark/CSM3B.cpp
       ${SOURCE_ROOT}/applications/csmBenchmark/main.cpp
   )
target_link_libraries(${EXEC_NAME}
   tmcCommon
   tmcAlgebra
   tmcStructuralSolver
   tmcViewsPlot
   )

INSTALL(TARGETS ${EXEC_NAME} RUNTIME DESTINATION lib)
