
include_directories(
    ${VTK_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(qt_HEADERS
    ${SOURCE_ROOT}/views/plot2d/TmcPlotWidget.h
    ${SOURCE_ROOT}/numerics/structuralsolver/massoscillator/views/TmcMassOscillatorWidget.h
    )
set(qt_SOURCES
    ${SOURCE_ROOT}/views/plot2d/TmcPlotWidget.cxx
    ${SOURCE_ROOT}/numerics/structuralsolver/massoscillator/views/TmcMassOscillatorWidget.cxx
    )

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


add_library(tmcViewsPlot SHARED
    ${vtk_HEADERS} ${vtk_SOURCES}
    ${qt_HEADERS} ${qt_SOURCES} ${qt_FORMS_UIC} ${qt_HEADERS_MOC} ${qt_RESOURCES}
    )

SET(LINK_LIBS
    tmcCommon
    tmcAlgebra
    tmcStructuralSolver
    )

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET(LINK_LIBS ${LINK_LIBS} z)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

target_link_libraries(tmcViewsPlot ${Boost_LIBRARIES} ${VTK_LIBRARIES} ${LINK_LIBS} Qt5::Core Qt5::Widgets)
INSTALL(TARGETS tmcViewsPlot DESTINATION lib)

