#ifndef GRID_H
#define GRID_H

#include "colors.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>

#define MAX_GRID_SIZE 64
#define EMPTY_CELL '_'

static const size_t valid_size[8] = {1, 4, 9, 16, 25, 36, 49, 64};

static const char color_table[] =
    "123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "@"
    "abcdefghijklmnopqrstuvwxyz"
    "&*";

typedef struct _grid_t grid_t;

typedef struct choice_t choice_t;

void grid_free(grid_t *grid);
void grid_print(const grid_t *grid, FILE *fd);
void grid_set_cell(grid_t *grid, const size_t row, const size_t column,
                   const char color);
void set_full(grid_t *grid);
void get_sub_row(const grid_t *grid, colors_t *sub_row[], const size_t pos_row);
void get_sub_column(const grid_t *grid, colors_t *sub_row[],
                    const size_t pos_col);
void grid_choice_free(choice_t *choice);
void grid_choice_apply(grid_t *grid, const choice_t *choice);
void grid_choice_blank(grid_t *grid, const choice_t *choice);
void grid_choice_discard(grid_t *grid, const choice_t *choice);
void grid_simple_copy(grid_t *grid, grid_t *grid_c);
void grid_print_full(const grid_t *grid, FILE *fd);

size_t grid_get_size(const grid_t *grid);
size_t grid_heuristics(grid_t *grid);

grid_t *grid_alloc(size_t size);
grid_t *grid_copy(const grid_t *grid);

char *grid_get_cell(const grid_t *grid, const size_t row, const size_t column);

bool grid_check_char(const grid_t *grid, const int c);
bool grid_check_size(size_t size);
bool grid_is_solved(grid_t *grid);
bool grid_is_consistent(grid_t *grid);
bool grid_choice_is_empty(const choice_t *choice);

choice_t *grid_choice(grid_t *grid);
choice_t *grid_choice_alloc();

#endif /*GRID_H*/