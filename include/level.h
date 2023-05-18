/* Copyright 2023 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef VULC_ANTS_LEVEL
#define VULC_ANTS_LEVEL

#include "main.h"

#include "ant.h"

#define LEVEL_ANTS_COUNT (256)

#define LEVEL_W (240)
#define LEVEL_H (160)

#define LEVEL_SIZE (LEVEL_W * LEVEL_H)

struct Level {
    struct Ant ants[LEVEL_ANTS_COUNT];

    i16 ant_ids[LEVEL_W * LEVEL_H];
    u8 tiles[LEVEL_W * LEVEL_H];
};

#define TILE_BLANK (0)
#define TILE_FOOD  (1)
#define TILE_WALL  (2)

#define TILE_PHEROMONE_HOME (16)
#define TILE_PHEROMONE_FOOD (17)

extern struct Level level;

inline i16 level_get_ant_id(i32 x, i32 y) {
    if(x < 0 || y < 0 || x >= LEVEL_W || y >= LEVEL_H)
        return -1;

    i16 id = level.ant_ids[x + y * LEVEL_W];
    if(id < 0 || id >= LEVEL_ANTS_COUNT)
        return -1;

    return id;
}

inline void level_set_ant_id(i32 x, i32 y, i16 id) {
    if(x < 0 || y < 0 || x >= LEVEL_W || y >= LEVEL_H)
        return;

    level.ant_ids[x + y * LEVEL_W] = id;
}

inline struct Ant *level_get_ant(i32 x, i32 y) {
    i16 id = level_get_ant_id(x, y);
    if(id >= 0)
        return &level.ants[id];
    return NULL;
}

inline u8 level_get_tile(i32 x, i32 y) {
    if(x < 0 || y < 0 || x >= LEVEL_W || y >= LEVEL_H)
        return TILE_WALL;
    return level.tiles[x + y * LEVEL_W];
}

inline bool level_can_walk(i32 x, i32 y) {
    if(level_get_ant_id(x, y) >= 0)
        return false;

    u8 tile = level_get_tile(x, y);
    if(tile != TILE_BLANK && tile < 16)
        return false;

    return true;
}

void level_init(void);
void level_tick(void);
void level_draw(void);

#endif // VULC_ANTS_LEVEL
