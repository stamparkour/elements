include(FetchContent)

#source files
set(PROJECT_SOURCES
	"src/source.cpp"
)
set(PROJECT_HEADERS
)


# add the executable
add_executable(elements_my_app ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(elements_my_app PUBLIC elements::elements)
target_compile_features(elements_my_app PUBLIC cxx_std_20)

set_target_properties(elements_my_app PROPERTIES OUTPUT_NAME "Elements${CMAKE_VERSION}")

# config files
# configure_file(<input> <output> COPYONLY)