cmake_minimum_required(VERSION 3.14)

project(Tetris LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(sfml)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets Core Network Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Network Charts REQUIRED)

set(SOURCES_FILES
    main.cpp
    stats.cpp
    tetromino/tetromino.cpp
    tetromino/tetrominogenerator.cpp
    tetromino/tpiece.cpp
    tetromino/ipiece.cpp
    tetromino/spiece.cpp
    tetromino/opiece.cpp
    tetromino/zpiece.cpp
    tetromino/lpiece.cpp
    tetromino/jpiece.cpp
    gui/qsfmlcanvas.cpp
    gui/menuform.cpp
    dialogs/saveleader.cpp
    utils/scoreboardutils.cpp
    utils/settings.cpp
    gui/tetriswidget.cpp
)

set(HEADERS_FILES
    stats.h
    tetromino/tetromino.h
    tetromino/tetrominogenerator.h
    tetromino/tpiece.h
    tetromino/ipiece.h
    tetromino/spiece.h
    tetromino/opiece.h
    tetromino/zpiece.h
    tetromino/lpiece.h
    tetromino/jpiece.h
    gui/qsfmlcanvas.h
    gui/menuform.h
    dialogs/saveleader.h
    utils/scoreboardutils.h
    utils/nlohmann/json.hpp
    utils/nlohmann/json_fwd.hpp
    utils/settings.h
    gui/tetriswidget.h
)

set(UI_FILES
    gui/menuform.ui
    dialogs/saveleader.ui
)

add_executable(
    ${PROJECT_NAME}
    WIN32
    ${SOURCES_FILES}
    ${HEADERS_FILES}
    ${UI_FILES}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE _CRT_SECURE_NO_WARNINGS
    PRIVATE WIN32_LEAN_AND_MEAN
)

target_link_libraries(
    ${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Charts
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

include_directories(${SFML_INCLUDE_DIRS})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sfml-graphics>
    $<TARGET_FILE:sfml-window>
    $<TARGET_FILE:sfml-system>
    $<TARGET_FILE:sfml-audio>
    $<TARGET_FILE:Qt5::Widgets>
    $<TARGET_FILE:Qt5::Core>
    $<TARGET_FILE:Qt5::Gui>
    $<TARGET_FILE:Qt5::Network>
    $<TARGET_FILE:Qt5::Charts>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

file(COPY ${PROJECT_SOURCE_DIR}/images DESTINATION ${PROJECT_BINARY_DIR})
