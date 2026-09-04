cmake_minimum_required(VERSION 2.6)

project(TAKUZU-10D)

#set(CMAKE_C_FLAGS "-std=c99 -g -Wall --coverage")
set(CMAKE_C_FLAGS "-Wall -std=c99")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 --coverage")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# file copy
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/default.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# folder copy
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/build/)

include(SDL2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")
include_directories(${SDL2_ALL_INC})

link_directories(${CMAKE_SOURCE_DIR})

include(CTest)
enable_testing()


add_library(game game.c game_aux.c game_ext.c queue.c game_private.c game_tools.c game_solve.c)
add_executable(game_text game_text.c)
target_link_libraries(game_text game)

add_executable(game_solve game_solve.c)
target_link_libraries(game_solve game)

add_executable(game_sdl main.c game_sdl.c)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m game)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(game_test_smervyn game_test_smervyn.c)
target_link_libraries(game_test_smervyn game)

add_test(test_smervyn_dummy ./game_test_smervyn dummy)
add_test(test_smervyn_game_copy ./game_test_smervyn test_game_copy)
add_test(test_smervyn_game_equal ./game_test_smervyn test_game_equal)
add_test(test_smervyn_game_delete ./game_test_smervyn test_game_delete)
add_test(test_smervyn_game_get_number ./game_test_smervyn test_game_get_number)
add_test(test_smervyn_game_get_next_number ./game_test_smervyn test_game_get_next_number)
add_test(test_smervyn_game_is_empty ./game_test_smervyn test_game_is_empty)
add_test(test_smervyn_game_has_error ./game_test_smervyn test_game_has_error)
add_test(test_smervyn_game_is_over ./game_test_smervyn test_game_is_over)
add_test(test_smervyn_game_restart ./game_test_smervyn test_game_restart)
add_test(test_smervyn_game_print ./game_test_smervyn test_game_print)
add_test(test_smervyn_game_default ./game_test_smervyn test_game_default)
add_test(test_smervyn_game_new_ext ./game_test_smervyn test_game_new_ext)
add_test(test_smervyn_game_new_empty_ext ./game_test_smervyn test_game_new_empty_ext)
add_test(test_smervyn_game_is_unique ./game_test_smervyn test_game_is_unique)
add_test(test_smervyn_game_is_wrapping ./game_test_smervyn test_game_is_wrapping)
add_test(test_smervyn_game_nb_rows ./game_test_smervyn test_game_nb_rows)
add_test(test_smervyn_game_nb_cols ./game_test_smervyn test_game_nb_cols)
add_test(test_smervyn_game_undo ./game_test_smervyn test_game_undo)
add_test(test_smervyn_game_redo ./game_test_smervyn test_game_redo)
add_test(test_smervyn_game_save_and_load ./game_test_smervyn test_game_save_and_load)
add_test(test_smervyn_game_load ./game_test_smervyn test_game_load)

add_executable(game_test_dyrobert game_test_dyrobert.c)
target_link_libraries(game_test_dyrobert game)

add_test(test_dyrobert_dummy ./game_test_dyrobert dummy)
add_test(test_dyrobert_game_default_solution ./game_test_dyrobert test_game_default_solution)
add_test(test_dyrobert_game_new_empty ./game_test_dyrobert test_game_new_empty)
add_test(test_dyrobert_game_new ./game_test_dyrobert test_game_new)
add_test(test_dyrobert_game_set_square ./game_test_dyrobert test_game_set_square)
add_test(test_dyrobert_game_get_square ./game_test_dyrobert test_game_get_square)
add_test(test_dyrobert_game_get_next_square ./game_test_dyrobert test_game_get_next_square)
add_test(test_dyrobert_game_check_move ./game_test_dyrobert test_game_check_move)
add_test(test_dyrobert_game_play_move ./game_test_dyrobert test_game_play_move)
add_test(test_dyrobert_game_is_immutable ./game_test_dyrobert test_game_is_immutable)
add_test(test_dyrobert_game_nb_solutions ./game_test_dyrobert test_game_nb_solutions)
add_test(test_dyrobert_game_solve ./game_test_dyrobert test_game_solve)
add_test(test_dyrobert_game_solve_4x4 ./game_test_dyrobert test_game_solve_4x4)

