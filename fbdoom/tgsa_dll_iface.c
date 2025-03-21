#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "doomtype.h"
#include "i_system.h"
#include "m_argv.h"

#define ALIGN(a) __declspec(align(a))
#define TGSACALL __fastcall
#define TGSAAPI __declspec( dllexport )

struct ALIGN(4) struct_a1_str
{
    const char *pchar0;
    unsigned int f0_len;
};

struct ALIGN(2) struct_a2
{
    __int16 width_int16_0;
    __int16 height_int16_2;
    __int16 bytes_per_pixel_int16_4;
};

struct ALIGN(2) struct_a3
{
    unsigned __int16 unsigned___int16_0;
    char gap2[2];
    unsigned int unsigned_int_4;
    uint16_t unsigned___int16_8;
};

struct ALIGN(2) struct_a4
{
    unsigned __int64 qword0;
    unsigned short word8;
};

struct ALIGN(2) inputs_probably_struc_2 {
    uint8_t field_0[14];
};

struct ALIGN(4) update_struct_a4 {
    uint8_t *pchar0;
    unsigned int unsigned_int8;
};

int isInited;
void (*pfunc_qword_180801A18)(void);

/////// timing
///
///
///
///

int init_tic;

int I_GetTicks_original(void)
{
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);
    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
}

// end

TGSAAPI char TGSACALL ArcadeTestDLL_CanInsertCoin(void) {
    return 0;
}

TGSAAPI char TGSACALL ArcadeTestDLL_CanStartGame(void) {
    return 1; // TODO
}

#define ALEN(arr) (sizeof(arr) / sizeof((arr)[0]))
const char *self_name = "fbdoom";

const char *args[] = {
    "fbdoom",
    "-iwad",
    "doom.wad"
};

TGSAAPI char TGSACALL ArcadeTestDLL_InitRom(
    struct struct_a1_str *str_gameid,
    struct struct_a2 *a2,
    struct struct_a3 *a3,
    struct struct_a4 *a4,
    char a5, void *a6
) {
    pfunc_qword_180801A18 = a6;

    //init_tic = I_GetTicks();


    myargc = ALEN(args);
    myargv = (char **) args;
    extern void D_DoomMain(void);
    D_DoomMain();

    //I_Error("%d %d %d", a3->unsigned___int16_0, a3->unsigned_int_4, a3->unsigned___int16_8);

    isInited = 1;
    return 1;
}

TGSAAPI int TGSACALL ArcadeTestDLL_Shutdown(void) {
    int was_init = isInited;

    isInited = 0;

    // TODO

    if (pfunc_qword_180801A18)
        pfunc_qword_180801A18();
    pfunc_qword_180801A18 = 0;

    return was_init;
}

extern uint8_t i_inputs[14];

TGSAAPI char TGSACALL ArcadeTestDLL_Update(
    double dt,
    struct inputs_probably_struc_2 *a2,
    uint8_t **a3,
    struct update_struct_a4 *a4
) {
    if (!isInited)
        return 0;

    // todo dt -> ticks?

    {
        // todo convert inputs
        char input[16];
        input[14] = '\n';
        input[15] = '\0';

        memcpy(i_inputs, a2->field_0, sizeof(i_inputs));

        for (int i = 0; i < 14; ++i)
        {
            input[i] = i_inputs[i] ? 'X' : 'O';
        }

        //printf("%s\n", input);
    }

    extern void D_DoomLoop_once(void);
    D_DoomLoop_once();

    // output screen to *a3
    extern byte* I_VideoBuffer_FB;
    extern int I_VideoBuffer_FB_sz;

    memcpy(*a3, I_VideoBuffer_FB, I_VideoBuffer_FB_sz);

    // TODO output sound somehow
    memset(a4->pchar0, 0, a4->unsigned_int8);

    return 1;
}

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if ( fdwReason == DLL_PROCESS_ATTACH )
        DisableThreadLibraryCalls(hinstDLL);
    return TRUE;
}
