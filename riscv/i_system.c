#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stdarg.h>
// #include <sys/time.h>
#include "malloc.h"
#include <unistd.h>

#include "doomdef.h"
#include "i_sound.h"
#include "i_video.h"
#include "m_misc.h"

#include "d_net.h"
#include "g_game.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"

// How much memory to allocate to Zone
int mb_used = 32;

/* */
/* I_Error */
/* */
extern boolean demorecording;

void I_Error(char *error, ...) {

  va_list argptr;

  // Message first.
  va_start(argptr, error);
  fprintf(stderr, "Error: ");
  vfprintf(stderr, error, argptr);
  fprintf(stderr, "\n");
  va_end(argptr);

  fflush(stderr);

  //  Shutdown. Here might be other errors.
  if (demorecording)
    G_CheckDemoStatus();

  D_QuitNetGame();
  I_ShutdownGraphics();

  va_start(argptr, error);
  fprintf(stdout, "Error: ");
  vfprintf(stdout, error, argptr);
  fprintf(stdout, "\n");
  va_end(argptr);

  fflush(stdout);

  exit(-1);
}

byte *I_ZoneBase(int *size) {
  // *size = mb_used * 1024 * 1024;
  // return (byte *)malloc(*size);
  *size = TOTAL_MEMORY - HEAP_SIZE - HEAP_START;
  return (byte *)(HEAP_START + HEAP_SIZE);
}

byte *I_AllocLow(int length) {
  printf("I_AllocLow %d\n", length);
  byte *mem;

  mem = (byte *)malloc(length);
  memset(mem, 0, length);
  return mem;
}

//
// I_GetTime
// returns time in 1/TICRATE second tics
//
int I_GetTime(void) {
  printf("I_GetTime unimplemented \n");
  //   if (mselapsed>0)
  //   {
  //     return (mselapsed);
  //   }
  //   else
  //   {
  //     struct timeval        tp;
  //     struct timezone       tzp;
  //     int                   newtics;
  //     static int            basetime=0;

  //     gettimeofday(&tp, &tzp);

  //     if (!basetime)
  //       basetime = tp.tv_sec;

  //     newtics = (tp.tv_sec-basetime)*TICRATE + tp.tv_usec*TICRATE/1000000;
  //     return newtics;
  //   }
  return 0;
}

/* */
/* I_Init */
/* */
void I_Init(void) {
  // load and init Allegro?
  fprintf(stdout, "I_InitSound: Allegro init\n");
  // allegro_init();

  I_InitSound();
  /*  I_InitGraphics(); */

  // init timer
  //   LOCK_VARIABLE(mselapsed);
  //   LOCK_FUNCTION(I_Timer);
  //   install_timer();
  // install_int(I_Timer,5);
  //   install_int_ex(I_Timer,BPS_TO_TIMER(TICRATE));

  // Init music

  I_InitMusic();

  // Load entire midi patch set.
  // load_midi_patches();

  // memset(oldkeystate,0,128);
  // install_keyboard();

  // S_StartMusic (mus_intro);
}

/* */
/* I_Quit */
/* */
void I_Quit(void) {
  int i;
  unsigned char *scr;

  D_QuitNetGame();
  I_ShutdownSound();
  I_ShutdownMusic();
  M_SaveDefaults();
  I_ShutdownGraphics();
  // remove_keyboard();
  // remove_timer();
  // allegro_exit();

  /*
         scr = (byte *)W_CacheLumpName("ENDOOM", 101); // PU_CACHE 101
 //	for (i=0;i<=22;i++) printf("\n");
            memcpy((void *)(__djgpp_conventional_base)+0xb8000,scr,4000);
        */

  exit(0);
}

void I_Tactile(int on, int off, int total) {
  /* UNUSED. */
  on = off = total = 0;
}

void I_WaitVBL(int count) {}

ticcmd_t emptycmd;
ticcmd_t *I_BaseTiccmd(void) { return &emptycmd; }