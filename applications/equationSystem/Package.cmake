
include_directories(
  ${SOURCE_ROOT}
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)


SET(EXEC_NAME equationSystem)
IF(NOT CMAKE_SYSTEM MATCHES "Windows")
    SET(EXEC_NAME ${EXEC_NAME}.exe)
ENDIF(NOT CMAKE_SYSTEM MATCHES "Windows")
ADD_EXECUTABLE(${EXEC_NAME}
                ${SOURCE_ROOT}/applications/equationSystem/example.cpp
               )
target_link_libraries(${EXEC_NAME}
   tmcCommon
   tmcAlgebra
   )

INSTALL(TARGETS ${EXEC_NAME} RUNTIME DESTINATION lib)
