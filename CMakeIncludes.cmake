
# find boost
IF(NOT Boost_FOUND)
    IF(CMAKE_SYSTEM MATCHES "Windows")
        find_package(Boost 1.78 REQUIRED COMPONENTS python39 thread iostreams filesystem regex system program_options serialization timer chrono container)
    ELSE(CMAKE_SYSTEM MATCHES "Windows")
      find_package(Boost REQUIRED COMPONENTS python thread iostreams filesystem regex system program_options serialization timer chrono )
    ENDIF(CMAKE_SYSTEM MATCHES "Windows")
   INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
   LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(NOT Boost_FOUND)

# find qt
IF(NOT QT5_FOUND)
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
    ADD_DEFINITIONS(${QT_DEFINITIONS})
ENDIF(NOT QT5_FOUND)


find_package(ParaView REQUIRED)
include_directories(${ParaView_INCLUDE_DIR} ${ParaView_CORE_INCLUDE_DIR})
link_directories(${ParaView_LIBRARY_DIR})

INCLUDE (FindMPI)
INCLUDE_DIRECTORIES( ${MPI_INCLUDE_PATH} )

FIND_PACKAGE(TBB REQUIRED)




