cmake_minimum_required(VERSION 3.16)
project(LibrarySerialization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Shared core: hierarchy + factory + plugin manager (single copy for app and plugins).
add_library(library_core SHARED
    src/LibraryObject.cpp
    src/Publication.cpp
    src/Book.cpp
    src/Magazine.cpp
    src/FictionBook.cpp
    src/Textbook.cpp
    src/Newspaper.cpp
    src/ObjectFactory.cpp
    src/PluginManager.cpp
)

target_include_directories(library_core PUBLIC include)
target_compile_definitions(library_core PRIVATE LIBRARY_CORE_BUILDING)

add_executable(library_app
    src/main.cpp
    src/TextSerializer.cpp
    src/ConsoleUI.cpp
)

target_include_directories(library_app PRIVATE include)
target_link_libraries(library_app PRIVATE library_core)

# Sample plugin: adds ComicBook type without recompiling the main application.
add_library(comic_plugin SHARED
    plugins/comic_plugin/ComicBook.cpp
)

target_include_directories(comic_plugin PRIVATE include plugins/comic_plugin)
target_link_libraries(comic_plugin PRIVATE library_core)

set(PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/plugins")
set_target_properties(comic_plugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
)

if(MSVC)
    target_compile_options(library_core PRIVATE /W4)
    target_compile_options(library_app PRIVATE /W4)
    target_compile_options(comic_plugin PRIVATE /W4)
else()
    target_compile_options(library_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(library_app PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(comic_plugin PRIVATE -Wall -Wextra -Wpedantic)
endif()
