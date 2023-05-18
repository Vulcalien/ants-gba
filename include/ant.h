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
#ifndef VULC_ANTS_ANT
#define VULC_ANTS_ANT

#include "main.h"

struct Ant {
    i16 x, y;
    i16 old_x, old_y;

    u8 team;
    u8 type;

    u8 data[4];
};

void ant_init(struct Ant *ant, i32 x, i32 y, u8 team, u8 type);
void ant_tick(struct Ant *ant);
void ant_draw(struct Ant *ant);

struct ant_Type {
    void (*tick)(struct Ant *ant);
};

#define ANT_TYPES (5)
extern const struct ant_Type *ant_types[ANT_TYPES];

extern const struct ant_Type ant_queen;
extern const struct ant_Type ant_soldier;
extern const struct ant_Type ant_explorer;
extern const struct ant_Type ant_builder;
extern const struct ant_Type ant_gatherer;

inline const struct ant_Type *ant_get_type(struct Ant *ant) {
    return ant_types[ant->type];
}

#endif // VULC_ANTS_ANT
