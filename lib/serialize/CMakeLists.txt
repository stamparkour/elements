include(FetchContent)

#source files
set(PROJECT_SOURCES
	"src/serialize.cpp"
)
set(PROJECT_HEADERS
	"include/elements/serialize.h"
)

# add the library
add_library(elements_serialize STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
add_library(elements::serialize ALIAS elements_serialize)
target_compile_features(elements_serialize PUBLIC cxx_std_20)

target_link_libraries(elements_serialize PUBLIC elements::core elements::game)

# adds a directory to search when finding header files
target_include_directories(elements_serialize 
	PUBLIC "include" 
)
