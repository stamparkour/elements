include(FetchContent)

#source files
set(PROJECT_SOURCES
	"src/source.cpp"
)
set(PROJECT_HEADERS
	"src/core_test.h"
)


# add the executable
add_executable(elements_test ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(elements_test PUBLIC elements::elements)

enable_testing() # Enable testing in CMake

include(gtest)
target_link_libraries(elements_test PUBLIC GTest::gtest_main)


#google test required lines
include(GoogleTest)
gtest_discover_tests(elements_test)