cmake_minimum_required(VERSION 3.17)
project(sudoku)

# add SDL2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

file(GLOB_RECURSE ProjectSOURCES "src/*.cpp")
file(GLOB_RECURSE ProjectHEADERS "src/*.h")

set (ProjectINCLUDE_DIRS "")
foreach (_headerFile ${ProjectHEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND ProjectINCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES ProjectINCLUDE_DIRS)

add_executable (sudoku ${ProjectSOURCES})
target_include_directories(sudoku PRIVATE ${ProjectINCLUDE_DIRS})

target_link_libraries(sudoku ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

target_include_directories(sudoku PRIVATE ${project_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)


