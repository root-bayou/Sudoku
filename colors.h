#ifndef COLORS_H
#define COLORS_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_COLORS 64

typedef uint64_t colors_t;

colors_t colors_full(const size_t size);
colors_t colors_empty(void);
colors_t colors_set(const size_t colors_id);
colors_t colors_add(const colors_t colors, const size_t colors_id);
colors_t colors_discard(const colors_t colors, const size_t colors_id);
colors_t colors_negate(const colors_t colors);
colors_t colors_and(const colors_t colors1, const colors_t colors2);
colors_t colors_or(const colors_t colors1, const colors_t colors2);
colors_t colors_xor(const colors_t colors1, const colors_t colors2);
colors_t colors_rightmost(const colors_t colors);
colors_t colors_leftmost(const colors_t colors);
colors_t colors_random(const colors_t colors);
colors_t colors_subtract(const colors_t colors1, const colors_t colors2);

bool colors_is_in(const colors_t colors, const size_t colors_id);
bool colors_is_equal(const colors_t colors1, const colors_t colors2);
bool colors_is_subset(const colors_t colors1, const colors_t colors2);
bool colors_is_singleton(const colors_t colors);
bool subgrid_consistency(colors_t subgrid[], const size_t size);
bool subgrid_heuristics(colors_t *subgrid[], size_t size);
bool subgrid_only_singleton(colors_t subgrid[], size_t size);

size_t colors_count(const colors_t colors);
size_t colors_position(const colors_t color);

#endif /* COLORS_H */