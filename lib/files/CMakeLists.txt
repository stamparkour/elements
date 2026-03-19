include(FetchContent)

#source files
set(PROJECT_SOURCES
	"src/files.cpp"
)
set(PROJECT_HEADERS
	"include/elements/files.h"
)

# add the library
add_library(elements_files STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
add_library(elements::files ALIAS elements_files)
target_compile_features(elements_files PUBLIC cxx_std_20)

target_link_libraries(elements_files PUBLIC elements::core elements::game)

# adds a directory to search when finding header files
target_include_directories(elements_files 
	PUBLIC "include" 
)
