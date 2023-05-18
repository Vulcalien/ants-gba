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
#include "ant.h"

#include "level.h"

#define STATUS_STARTING   (0)
#define STATUS_SEARCHING  (1)
#define STATUS_FOOD_FOUND (2)

struct explorer_Data {
    u8 status;

    i8 dir_x;
    i8 dir_y;

    u8 unsed[1];
};

static_assert(sizeof(struct explorer_Data) == 4, "");

static inline bool check_for_food(struct Ant *ant) {
    // TODO optimize: probably doesn't need to
    // check every direction, but only forward.
    for(i32 y = ant->y - 1; y < ant->y + 1; y++) {
        for(i32 x = ant->x - 1; x < ant->x + 1; x++) {
            if(level_get_tile(x, y) == TILE_FOOD)
                return true;
        }
    }
    return false;
}

IWRAM_SECTION
void explorer_tick(struct Ant *ant) {
    struct explorer_Data *data = (struct explorer_Data *) ant->data;

    switch(data->status) {
        case STATUS_STARTING: {
            u16 r = rand();
            data->dir_x = ((r << 0) & 2) - 1;
            data->dir_y = ((r << 1) & 2) - 1;

            data->status = STATUS_SEARCHING;
            break;
        }

        case STATUS_SEARCHING: {
            if(check_for_food(ant)) {
                data->status = STATUS_FOOD_FOUND;
            } else {
                u16 r = rand();

                // random direction change
                if(r % 16 == 0) {
                    if(r >> 15)
                        data->dir_x = ((r >> 8) & 2) - 1;
                    else
                        data->dir_y = ((r >> 8) & 2) - 1;
                }

                i32 new_x = ant->x + data->dir_x;
                i32 new_y = ant->y + data->dir_y;

                if(level_can_walk(ant->x, new_y))
                    ant->y = new_y;
                if(level_can_walk(new_x, ant->y))
                    ant->x = new_x;
            }
            break;
        }

    }
}

const struct ant_Type ant_explorer = {
    .tick = explorer_tick
};
