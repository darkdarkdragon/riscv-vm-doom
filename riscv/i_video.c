
#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"
#include <stdio.h>


/* */
/* I_ReadScreen */
/* */
void I_ReadScreen (byte* scr)
{
	memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}

/* */
/* I_UpdateNoBlit */
/* */
void I_UpdateNoBlit (void)
{
	/* what is this? */
}

void I_InitGraphics(void)
{

	static int firsttime=1;

	if (!firsttime)
		return;
	firsttime=0;

	/*enter graphics mode */
#ifdef GRAPHICS

      r.x.ax = 0x13;
      __dpmi_int(0x10, &r);
//dascreen= (byte *)(__djgpp_conventional_base+0xa0000);
dascreen=(byte *)malloc(SCREENWIDTH*SCREENHEIGHT);
#else
// dascreen=(byte *)malloc(SCREENWIDTH*SCREENHEIGHT);
#endif

	screens[0]=(byte *)malloc(SCREENWIDTH*SCREENHEIGHT);

	/*init the mouse */

	/*init keyboard */
	//initkeyhandler();
}

void I_FinishUpdate(void)
{
	static int lasttic;
	int tics;
	int i;
	/* UNUSED static unsigned char *bigscreen=0; */

	/* draws little dots on the bottom of the screen */
	if (devparm)
	{

		i = I_GetTime();
		tics = i - lasttic;
		lasttic = i;
		if (tics > 20) tics = 20;

		for (i=0; i<tics*2; i+=2)
			screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
		for (; i<20*2; i+=2)
			screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;

	}

	/*blast it to the screen */
	//  memcpy(dascreen,screens[0],SCREENWIDTH*SCREENHEIGHT);
	// dosmemput(screens[0], 320*200, 0xA0000);
}

void I_GetEvent()
{
}

/* */
/* I_StartTic */
/* */
void I_StartTic()
{
	I_GetEvent();
	/*i dont think i have to do anything else here */

}

void I_SetPalette (byte* palette)
{
#ifdef GRAPHICS
	int c,i;
  outportb(0x3c8,0);
  for (i=0;i<256;i++)
    {
    c=gammatable[usegamma][*palette++];
    outportb(0x3c9,c>>2);
    c=gammatable[usegamma][*palette++];
    outportb(0x3c9,c>>2);
    c=gammatable[usegamma][*palette++];
    outportb(0x3c9,c>>2);
    }
#endif
}

/* */
/* I_StartFrame */
/* */
void I_StartFrame (void)
{
	/* er? */
	printf("I_StartFrame\n");

}

void I_ShutdownGraphics(void)
{
#ifdef GRAPHICS
      __dpmi_regs r;

      r.x.ax = 3;
      __dpmi_int(0x10, &r);
#endif
}
