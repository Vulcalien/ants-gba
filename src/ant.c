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

void ant_init(struct Ant *ant, i32 x, i32 y, u8 team) {
    ant->x = x;
    ant->y = y;

    ant->team = team;
}

IWRAM_SECTION
void ant_tick(struct Ant *ant) {
    i32 new_x = ant->x + (rand() % 3) - 1;
    i32 new_y = ant->y + (rand() % 3) - 1;

    // check if the ant is going out of bounds
    if(new_x < 0 || new_y < 0 || new_x >= LEVEL_W || new_y >= LEVEL_H)
        return;

    i16 id = level_get_ant_id(ant->x, ant->y);

    // check if an ant is where we want to go
    if(id >= 0)
        return;

    // update position
    level_set_ant_id(ant->x, ant->y, -1);
    ant->x = new_x;
    ant->y = new_y;
    level_set_ant_id(ant->x, ant->y, id);
}

IWRAM_SECTION
void ant_draw(struct Ant *ant) {
    screen_set_pixel(ant->x, ant->y, ant->team);

    ant->draw.old_x2 = ant->draw.old_x1;
    ant->draw.old_y2 = ant->draw.old_y1;

    ant->draw.old_x1 = ant->x;
    ant->draw.old_y1 = ant->y;
}

IWRAM_SECTION
void ant_undraw(struct Ant *ant) {
    screen_set_pixel(ant->draw.old_x2, ant->draw.old_y2, 0);
}
