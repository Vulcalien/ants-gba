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
#include "screen.h"

void ant_init(struct Ant *ant, i32 x, i32 y, u8 team, u8 type) {
    ant->x = x;
    ant->y = y;

    ant->team = team;
    ant->type = type;

    for(u32 i = 0; i < sizeof(ant->data); i++)
        ant->data[i] = 0;
}

IWRAM_SECTION
void ant_tick(struct Ant *ant) {
    ant->old_x = ant->x;
    ant->old_y = ant->y;

    const struct ant_Type *type = ant_get_type(ant);
    type->tick(ant);

    if(ant->x == ant->old_x && ant->y == ant->old_y)
        return;

    // update position
    i16 id = level_get_ant_id(ant->old_x, ant->old_y);
    level_set_ant_id(ant->old_x, ant->old_y, -1);
    while(id < 0);
    level_set_ant_id(ant->x, ant->y, id);
}

IWRAM_SECTION
void ant_draw(struct Ant *ant) {
    // undraw
    screen_set_pixel(ant->old_x, ant->old_y, 0);

    screen_set_pixel(
        ant->x, ant->y,
        (ant->team - 1) * 16 + ant->type + 1
    );
}

const struct ant_Type *ant_types[ANT_TYPES] = {
    &ant_queen,
    &ant_soldier,
    &ant_explorer,
    &ant_builder,
    &ant_gatherer
};
