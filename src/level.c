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
#include "level.h"

SBSS_SECTION
struct Level level;

void level_init(void) {
    for(u32 i = 0; i < LEVEL_SIZE; i++) {
        level.ant_ids[i] = -1;
        level.tiles[i] = TILE_BLANK;
    }

    // DEBUG
    for(u32 i = 0; i < 20; i++) {
        struct Ant *ant = &level.ants[i];

        ant_init(ant, rand() % LEVEL_W, rand() % LEVEL_H, 1, 2);
        level_set_ant_id(ant->x, ant->y, i);
    }
}

IWRAM_SECTION
void level_tick(void) {
    for(u32 i = 0; i < LEVEL_ANTS_COUNT; i++) {
        struct Ant *ant = &level.ants[i];

        if(ant->team > 0)
            ant_tick(ant);
    }
}

IWRAM_SECTION
void level_draw(void) {
    for(u32 i = 0; i < LEVEL_ANTS_COUNT; i++) {
        struct Ant *ant = &level.ants[i];

        if(ant->team > 0)
            ant_draw(ant);
    }
}
