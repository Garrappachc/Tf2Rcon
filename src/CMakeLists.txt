configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

find_package (Qt5Core REQUIRED)
include_directories (${Qt5Core_INCLUDE_DIRS})

find_package (Qt5Widgets REQUIRED)
include_directories (${Qt5Widgets_INCLUDE_DIRS})

find_package (Qt5Network REQUIRED)
include_directories (${Qt5Network_INCLUDE_DIRS})

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_BINARY_DIR})

find_library (QRCON_LIBRARIES qrcon)
find_path (QRCON_INCLUDE_DIR qrcon.h
    PATH_SUFFIXES qrcon
)

include_directories (${QRCON_INCLUDE_DIR})

set (tf2rcon_SOURCES
    logindialog.cpp
    maplistcommand.cpp
    statuscommand.cpp
    tf2rconmainwindow.cpp
    userlistcommand.cpp
    
    main.cpp
)

set (tf2rcon_HEADERS
    logindialog.h
    maplistcommand.h
    statuscommand.h
    tf2rconmainwindow.h
    userlistcommand.h
)

set (tf2rcon_QRCS resources.qrc)

add_executable (tf2rcon MACOSX_BUNDLE WIN32 ${tf2rcon_SOURCES} ${tf2rcon_HEADERS} ${tf2rcon_QRCS})
target_link_libraries (tf2rcon Qt5::Core Qt5::Widgets Qt5::Network ${QRCON_LIBRARIES})
