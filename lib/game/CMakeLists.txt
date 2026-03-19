include(FetchContent)

#source files
set(PROJECT_SOURCES
	"src/game.cpp"
)
set(PROJECT_HEADERS
	"include/elements/game.h"
	"include/elements/game/element.h"
 "include/elements/game/recipe.h" "include/elements/game/game_state.h" "include/elements/game/element_inventory.h")

# add the library
add_library(elements_game STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
add_library(elements::game ALIAS elements_game)
target_compile_features(elements_game PUBLIC cxx_std_20)

target_link_libraries(elements_game PUBLIC elements::core)

# adds a directory to search when finding header files
target_include_directories(elements_game 
	PUBLIC "include" 
)
