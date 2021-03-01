cmake_minimum_required(VERSION 3.17)
project(Calculator C)

set(CMAKE_C_STANDARD 99)

add_executable(Calculator main.c src/parser.c src/parser.h src/structsAndEnums.h src/input.c src/input.h src/ui.c src/ui.h res/ui.h src/utils.c src/utils.h src/evaluator.c src/evaluator.h src/operations.c src/operations.h src/output.c src/output.h)