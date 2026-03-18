cmake_minimum_required(VERSION 3.24)
# set the project name
project(stamp VERSION 0.1.0 LANGUAGES C CXX)


#options

#config
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include subdirectories
add_subdirectory(lib/core)
add_subdirectory(lib/reflect)
add_subdirectory(lib/diagnose)
add_subdirectory(lib/math)
add_subdirectory(lib/serialize)
add_subdirectory(lib/signal)

add_subdirectory(app/my_app)

add_subdirectory(test)


add_library(stamp_stamp INTERFACE)
add_library(stamp::stamp ALIAS stamp_stamp)
target_link_libraries(stamp_stamp INTERFACE stamp::core stamp::math stamp::reflect)


#project warnings
if (MSVC)
    add_compile_options(/W4 /WX)
    add_compile_options(/std:c++latest) 
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()