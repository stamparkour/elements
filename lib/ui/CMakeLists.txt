include(FetchContent)

#source files
set(PROJECT_SOURCES
	"src/ui.cpp"
)
set(PROJECT_HEADERS
	"include/elements/ui.h"
)

# add the library
add_library(elements_ui STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
add_library(elements::ui ALIAS elements_ui)
target_compile_features(elements_ui PUBLIC cxx_std_20)

target_link_libraries(elements_ui PUBLIC elements::core elements::game)

# adds a directory to search when finding header files
target_include_directories(elements_ui 
	PUBLIC "include" 
)
