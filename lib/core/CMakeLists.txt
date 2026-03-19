include(FetchContent)

#source files
set(PROJECT_SOURCES
	"src/core.cpp"
)
set(PROJECT_HEADERS
	"include/elements/core.h"
	"include/elements/core/word_iterable.h"
)

# add the library
add_library(elements_core STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
add_library(elements::core ALIAS elements_core)
target_compile_features(elements_core PUBLIC cxx_std_20)

# adds a directory to search when finding header files
target_include_directories(elements_core 
	PUBLIC "include" 
	PUBLIC "${stamp_BINARY_DIR}/lib/core/include"
)

# dependencies
# include(boost)
# target_link_libraries(core PUBLIC 
# 	Boost::asio
#	Boost::filesystem
#	Boost::thread
#	Boost::program_options
# )

# include(glew)
# target_link_libraries(core PUBLIC glew_s)

# include(glfw)
# target_link_libraries(core PUBLIC glfw)

# include(imgui)
# target_link_libraries(core PUBLIC imgui)

# include(miniaudio)
# target_link_libraries(core PUBLIC miniaudio)

# include(concurrentqueue)
# target_link_libraries(core PUBLIC concurrentqueue)

# set the configure file for the project

configure_file("include/elements/elements_config.h.in" "include/elements/elements_config.h")
