cmake_minimum_required(VERSION 3.24)
# set the project name
project(elements VERSION 0.1.0 LANGUAGES C CXX)


#options

#config
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include subdirectories
add_subdirectory(lib/core)
add_subdirectory(lib/game)
add_subdirectory(lib/serialize)
add_subdirectory(lib/ui)

add_library(elements_elements INTERFACE)
add_library(elements::elements ALIAS elements_elements)
target_link_libraries(elements_elements INTERFACE elements::core elements::ui elements::game elements::serialize)

add_subdirectory(app/game)

add_subdirectory(test)


#project warnings
if (MSVC)
    add_compile_options(/W4 /WX)
    add_compile_options(/std:c++latest) 
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()