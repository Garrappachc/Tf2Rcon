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
    commandlinewidget.cpp
    logindialog.cpp
    maplistcommand.cpp
    savedserverswindow.cpp
    sourcemodpluginlistcommand.cpp
    statuscommand.cpp
    tf2rconmainwindow.cpp
    userlistcommand.cpp
    
    main.cpp
)

set (tf2rcon_HEADERS
    commandlinewidget.h
    logindialog.h
    maplistcommand.h
    savedserverswindow.h
    sourcemodpluginlistcommand.h
    statuscommand.h
    tf2rconmainwindow.h
    userlistcommand.h
)

set (tf2rcon_QRCS resources.qrc)
set (tf2rcon_UIS
    commandlinewidget.ui
    savedserverswindow.ui
    tf2rconmainwindow.ui
)

add_executable (tf2rcon MACOSX_BUNDLE WIN32 ${tf2rcon_SOURCES} ${tf2rcon_HEADERS} ${tf2rcon_QRCS} ${tf2rcon_UIS})
target_link_libraries (tf2rcon Qt5::Core Qt5::Widgets Qt5::Network ${QRCON_LIBRARIES})

if (WIN32)
    install (TARGETS tf2rcon RUNTIME DESTINATION .)

    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
    include(InstallRequiredSystemLibraries)

    include (QtUtils)
    query_qmake (QT_INSTALL_LIBS QT_INSTALL_BINS)

    include (Utils)
    get_platform_qpa_plugin (QT_QPA_PLUGIN)

    get_filename_component (QRCON_INSTALL_LIBS ${QRCON_LIBRARIES} DIRECTORY)
    
    include (DeployQt5)
    install_qt5_executable("tf2rcon.exe" "${QT_QPA_PLUGIN}" "" "${QT_INSTALL_LIBS};${QRCON_INSTALL_LIBS}/../bin")
endif ()
