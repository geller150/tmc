
include_directories(
  ${SOURCE_ROOT}
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)


SET(EXEC_NAME massOscillator)
IF(NOT CMAKE_SYSTEM MATCHES "Windows")
    SET(EXEC_NAME ${EXEC_NAME}.exe)
ENDIF(NOT CMAKE_SYSTEM MATCHES "Windows")
ADD_EXECUTABLE(${EXEC_NAME}
                ${SOURCE_ROOT}/applications/massOscillator/main.cpp
               )
target_link_libraries(${EXEC_NAME}
   tmcCommon
   tmcAlgebra
   tmcStructuralSolver
   tmcViewsPlot
   )

INSTALL(TARGETS ${EXEC_NAME} RUNTIME DESTINATION lib)
