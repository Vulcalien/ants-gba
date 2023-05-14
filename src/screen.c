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
#include "screen.h"

#define DISPLAY_CONTROL *((vu16 *) 0x04000000)
#define DISPLAY_STATUS  *((vu16 *) 0x04000004)

#define WINDOW_IN  *((vu16 *) 0x04000048)
#define WINDOW_OUT *((vu16 *) 0x0400004a)

#define CHAR_BLOCK_0 ((vu16 *) 0x06000000)
#define CHAR_BLOCK_1 ((vu16 *) 0x06004000)
#define CHAR_BLOCK_2 ((vu16 *) 0x06008000)
#define CHAR_BLOCK_3 ((vu16 *) 0x0600c000)

#define SPR_TILESET ((vu16 *) 0x06010000)

#define BG0_CONTROL *((vu16 *) 0x04000008)
#define BG1_CONTROL *((vu16 *) 0x0400000a)
#define BG2_CONTROL *((vu16 *) 0x0400000c)
#define BG3_CONTROL *((vu16 *) 0x0400000e)

#define BG_PALETTE  ((vu16 *) 0x05000000)
#define SPR_PALETTE ((vu16 *) 0x05000200)

#define FRAME_0 ((vu16 *) 0x06000000)
#define FRAME_1 ((vu16 *) 0x0600a000)

static u32 frame = 0;

void screen_init(void) {
    BG_PALETTE[0] = 0x0000;
    BG_PALETTE[1] = 0x7fff;

    DISPLAY_CONTROL = 4 << 0  | // Video Mode 4
                      0 << 4  | // Select Frame 0
                      1 << 10 | // Enable BG 2
                      1 << 12;  // Enable OBJs

    // hide all sprites
    for(u32 i = 0; i < 128; i++)
        OAM[i * 4] = 1 << 9;

    // enable V-Blank IRQ
    DISPLAY_STATUS = (1 << 3);
}

void screen_switch_frame(void) {
    frame ^= 1;
}

IWRAM_SECTION
void screen_set_pixel(u32 x, u32 y, u8 color) {
    if(x + y * SCREEN_W > SCREEN_W * SCREEN_H)
        return;

    vu16 *raster;
    if(frame == 0)
        raster = FRAME_1;
    else
        raster = FRAME_0;

    vu16 *addr = &raster[(x + y * SCREEN_W) / 2];
    if(x & 1) {
        u16 lo = *addr & 0x00ff;
        *addr = (color << 8) | lo;
    } else {
        u16 hi = *addr & 0xff00;
        *addr = hi | color;
    }
}

IWRAM_SECTION
void vsync(void) {
    __asm__ volatile ("swi 0x05 << 16");
}
