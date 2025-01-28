//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	DOOM keyboard input via linux tty
//


#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
//#include <termios.h>
//#include <sys/ioctl.h>
//#include <linux/keyboard.h>
//#include <linux/kd.h>

#include "config.h"
#include "deh_str.h"
#include "doomtype.h"
#include "doomkeys.h"
#include "i_joystick.h"
#include "i_system.h"
#include "i_swap.h"
#include "i_timer.h"
#include "i_video.h"
#include "i_scale.h"
#include "m_argv.h"
#include "m_config.h"
#include "m_misc.h"
#include "tables.h"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"

int vanilla_keyboard_mapping = 1;

static const char y0_to_doom[] =
{
    /* 0x00 */ KEY_UPARROW, // UP
    /* 0x01 */ 0, // UP R
    /* 0x02 */ KEY_RIGHTARROW, // R
    /* 0x03 */ 0, // R DOWN
    /* 0x04 */ KEY_DOWNARROW, // DOWN
    /* 0x05 */ 0, // DOWN L
    /* 0x06 */ KEY_LEFTARROW, // LEFT
    /* 0x07 */ 0, // LEFT UP
    /* 0x08 */ KEY_FIRE, //
    /* 0x09 */ KEY_USE,
    /* 0x0a */ KEY_RSHIFT,
    /* 0x0b */ KEY_RALT,
    /* 0x0c */ 0, // COIN
    /* 0x0d */ 0, // START
};

void kbd_shutdown(void)
{
    /* Shut down nicely. */

    printf("Cleaning up.\n");
    fflush(stdout);

    printf("Exiting normally.\n");

    exit(0);
}

uint8_t i_inputs[14];

uint8_t inputs_latched[14];
static int kbd_init(void)
{
    memset(i_inputs, 0, sizeof(i_inputs));
    memset(inputs_latched, 0, sizeof(inputs_latched));
    // nothing to do, input comes from a buffer every Update call
    return 0;
}


void I_GetEvent(void)
{
    event_t event;
    int pressed;
    unsigned char key;

    // put event-grabbing stuff in here

    // rewrite diagonal inputs
    if (i_inputs[1])
    {
        i_inputs[0] = 1;
        i_inputs[2] = 1;
    } else if (i_inputs[3])
    {
        i_inputs[4] = 1;
        i_inputs[2] = 1;
    } else if (i_inputs[5])
    {
        i_inputs[4] = 1;
        i_inputs[6] = 1;
    } else if (i_inputs[7])
    {
        i_inputs[6] = 1;
        i_inputs[0] = 1;
    }

    for (int i = 0; i < 14; i++)
    {
        pressed = i_inputs[i];
        key = y0_to_doom[i];

        if (pressed && !inputs_latched[i])
        {
            inputs_latched[i] = 1;
            // data1 has the key pressed, data2 has the character
            // (shift-translated, etc)
            event.type = ev_keydown;
            event.data1 = key;
            event.data2 = key;

            if (event.data1 != 0)
            {
                D_PostEvent(&event);
            }
        }
        else
        {
            inputs_latched[i] = 0;
            event.type = ev_keyup;
            event.data1 = key;

            // data2 is just initialized to zero for ev_keyup.
            // For ev_keydown it's the shifted Unicode character
            // that was typed, but if something wants to detect
            // key releases it should do so based on data1
            // (key ID), not the printable char.

            event.data2 = 0;

            if (event.data1 != 0)
            {
                D_PostEvent(&event);
            }
        }
    }
}

void I_InitInput(void)
{
    kbd_init();

    //UpdateFocus();
}

