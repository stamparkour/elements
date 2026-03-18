#include "core_test.h"
#include "ui_test.h"
#include "game_test.h"
#include <gtest/gtest.h>

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}