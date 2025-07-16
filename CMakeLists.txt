cmake_minimum_required(VERSION 3.16)
project(SpaceAliens VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(Qt6 6.2 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_executable(appSpaceAliens
    main.cpp
)

qt_add_qml_module(appSpaceAliens
    URI SpaceAliens
    VERSION 1.0
    QML_FILES
    SOURCES Controller.h Controller.cpp
    QML_FILES Bullet.qml
    SOURCES Bullet.h Bullet.cpp
    SOURCES main.cpp
    QML_FILES main.qml
    SOURCES Enemy.h Enemy.cpp
    QML_FILES Enemy.qml
    RESOURCES Assets/Assets.qrc
    SOURCES SoundManager.h SoundManager.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSpaceAliens PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSpaceAliens
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSpaceAliens
    PRIVATE
    Qt6::Quick
    Qt6::Multimedia)
target_link_libraries(appSpaceAliens PRIVATE Qt6::Core)
target_link_libraries(appSpaceAliens PRIVATE Qt6::Core)
target_link_libraries(appSpaceAliens PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appSpaceAliens
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
