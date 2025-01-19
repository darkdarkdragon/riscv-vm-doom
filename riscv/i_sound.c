
#include "z_zone.h"

#include "i_sound.h"
#include "i_system.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"

#include "doomdef.h"

#define NUM_CHANNELS 8

#define SAMPLECOUNT 512
int lengths[NUMSFX];

// The channel step amount...
unsigned int channelstep[NUM_CHANNELS];
// ... and a 0.16 bit remainder of last step.
unsigned int channelstepremainder[NUM_CHANNELS];

// The channel data pointers, start and end.
unsigned char *channels[NUM_CHANNELS];
unsigned char *channelsend[NUM_CHANNELS];

// Time/gametic that the channel started playing,
//  used to determine oldest, which automatically
//  has lowest priority.
// In case number of active sounds exceeds
//  available channels.
int channelstart[NUM_CHANNELS];

// The sound in channel handles,
//  determined on registration,
//  might be used to unregister/stop/modify,
//  currently unused.
int channelhandles[NUM_CHANNELS];

// SFX id of the playing sound effect.
// Used to catch duplicates (like chainsaw).
int channelids[NUM_CHANNELS];

// Pitch to stepping lookup, unused.
int steptable[256];

// Volume lookups.
int vol_lookup[128 * 256];

// Hardware left and right channel volume lookup.
int *channelleftvol_lookup[NUM_CHANNELS];
int *channelrightvol_lookup[NUM_CHANNELS];

/* */
/* This function loads the sound data from the WAD lump, */
/*  for single sound. */
/* */
void *getsfx(char *sfxname, int *len) {
  unsigned char *sfx;
  unsigned char *paddedsfx;
  int i;
  int size;
  int paddedsize;
  char name[20];
  int sfxlump;

  /* Get the sound data from the WAD, allocate lump */
  /*  in zone memory. */
  sprintf(name, "ds%s", sfxname);

  /* Now, there is a severe problem with the */
  /*  sound handling, in it is not (yet/anymore) */
  /*  gamemode aware. That means, sounds from */
  /*  DOOM II will be requested even with DOOM */
  /*  shareware. */
  /* The sound list is wired into sounds.c, */
  /*  which sets the external variable. */
  /* I do not do runtime patches to that */
  /*  variable. Instead, we will use a */
  /*  default sound for replacement. */
  if (W_CheckNumForName(name) == -1)
    sfxlump = W_GetNumForName("dspistol");
  else
    sfxlump = W_GetNumForName(name);

  size = W_LumpLength(sfxlump);

  /* Debug. */
  /* fprintf( stderr, "." ); */
  /*fprintf( stderr, " -loading  %s (lump %d, %d bytes)\n", */
  /*	     sfxname, sfxlump, size ); */
  /*fflush( stderr ); */

  sfx = (unsigned char *)W_CacheLumpNum(sfxlump, PU_STATIC);

  /* Pads the sound effect out to the mixing buffer size. */
  /* The original realloc would interfere with zone memory. */
  paddedsize = ((size - 8 + (SAMPLECOUNT - 1)) / SAMPLECOUNT) * SAMPLECOUNT;

  /* Allocate from zone memory. */
  paddedsfx = (unsigned char *)Z_Malloc(paddedsize + 8, PU_STATIC, 0);
  /* ddt: (unsigned char *) realloc(sfx, paddedsize+8); */
  /* This should interfere with zone memory handling, */
  /*  which does not kick in in the soundserver. */

  /* Now copy and pad. */
  memcpy(paddedsfx, sfx, size);
  for (i = size; i < paddedsize + 8; i++)
    paddedsfx[i] = 128;

  /* Remove the cached lump. */
  Z_Free(sfx);

  /* Preserve padded length. */
  *len = paddedsize;

  /* Return allocated padded data. */
  return (void *)(paddedsfx + 8);
}

//
// This function adds a sound to the
//  list of currently active sounds,
//  which is maintained as a given number
//  (eight, usually) of internal channels.
// Returns a handle.
//
int addsfx(int sfxid, int volume, int step, int seperation) {

  static unsigned short handlenums = 0;

  int i;
  int rc = -1;

  int oldest = gametic;
  int oldestnum = 0;
  int slot;

  int rightvol;
  int leftvol;

  // Chainsaw troubles.
  // Play these sound effects only one at a time.
  if (sfxid == sfx_sawup || sfxid == sfx_sawidl || sfxid == sfx_sawful || sfxid == sfx_sawhit || sfxid == sfx_stnmov ||
      sfxid == sfx_pistol) {
    // Loop all channels, check.
    for (i = 0; i < NUM_CHANNELS; i++) {
      // Active, and using the same SFX?
      if ((channels[i]) && (channelids[i] == sfxid)) {
        // Reset.
        channels[i] = 0;
        // We are sure that iff,
        //  there will only be one.
        break;
      }
    }
  }

  // Loop all channels to find oldest SFX.
  for (i = 0; (i < NUM_CHANNELS) && (channels[i]); i++) {
    if (channelstart[i] < oldest) {
      oldestnum = i;
      oldest = channelstart[i];
    }
  }

  // Tales from the cryptic.
  // If we found a channel, fine.
  // If not, we simply overwrite the first one, 0.
  // Probably only happens at startup.
  if (i == NUM_CHANNELS)
    slot = oldestnum;
  else
    slot = i;

  // Okay, in the less recent channel,
  //  we will handle the new SFX.
  // Set pointer to raw data.
  channels[slot] = (unsigned char *)S_sfx[sfxid].data;
  // Set pointer to end of raw data.
  channelsend[slot] = channels[slot] + lengths[sfxid];

  // Reset current handle number, limited to 0..100.
  if (!handlenums)
    handlenums = 100;

  // Assign current handle number.
  // Preserved so sounds could be stopped (unused).
  channelhandles[slot] = rc = handlenums++;

  // Set stepping???
  // Kinda getting the impression this is never used.
  channelstep[slot] = step;
  // ???
  channelstepremainder[slot] = 0;
  // Should be gametic, I presume.
  channelstart[slot] = gametic;

  // Separation, that is, orientation/stereo.
  //  range is: 1 - 256
  seperation += 1;

  // Per left/right channel.
  //  x^2 seperation,
  //  adjust volume properly.
  leftvol = volume - ((volume * seperation * seperation) >> 16); ///(256*256);
  seperation = seperation - 257;
  rightvol = volume - ((volume * seperation * seperation) >> 16);

  // Sanity check, clamp volume.
  if (rightvol < 0 || rightvol > 127)
    I_Error("rightvol out of bounds");

  if (leftvol < 0 || leftvol > 127)
    I_Error("leftvol out of bounds");

  // Get the proper lookup table piece
  //  for this volume level???
  channelleftvol_lookup[slot] = &vol_lookup[leftvol * 256];
  channelrightvol_lookup[slot] = &vol_lookup[rightvol * 256];

  // Preserve sound SFX id,
  //  e.g. for avoiding duplicates of chainsaw.
  channelids[slot] = sfxid;

  // this is where the sound actually gets played.  i kept the previous code
  // because i dont want to risk breaking anything
  // max num in vol seems to be 8, i mul by 28 and not 31 just to be safe
  // @FIXME
  // play_sample(S_sfx[sfxid].data, volume * 16, -seperation, 1000, 0);

  // You tell me.
  return rc;
}

// Retrieve the raw data lump index
//  for a given SFX name.
//
int I_GetSfxLumpNum(sfxinfo_t *sfx) {
  char namebuf[9];
  sprintf(namebuf, "ds%s", sfx->name);
  return W_GetNumForName(namebuf);
}

void I_InitSound() {
  int i;

  // Secure and configure sound device first.
  fprintf(stdout, "I_InitSound: ");
  //   if (install_sound(DIGI_AUTODETECT, MIDI_MPU,NULL)==-1)
  //     fprintf(stdout,"ALLEGRO SOUND INIT ERROR!!!!\n");
  //   else
  //     fprintf(stdout, " configured audio device\n" );
  //   if(midi_driver)
  //     fprintf(stdout, "Found a %s:%s\n",midi_driver->name,midi_driver->desc);
  //   if(digi_driver)
  //     fprintf(stdout, "Found a %s:%s\n",digi_driver->name,digi_driver->desc);

  // Initialize external data (all sounds) at start, keep static.
  fprintf(stdout, "I_InitSound: ");

  for (i = 1; i < NUMSFX; i++) {
    // Alias? Example is the chaingun sound linked to pistol.
    if (!S_sfx[i].link) {
      // Load data from WAD file.
      S_sfx[i].data = getsfx(S_sfx[i].name, &lengths[i]);
    } else {
      // Previously loaded already?
      S_sfx[i].data = S_sfx[i].link->data;
      lengths[i] = lengths[(S_sfx[i].link - S_sfx) / sizeof(sfxinfo_t)];
    }
  }

  fprintf(stdout, " pre-cached all sound data\n");

  // Now initialize mixbuffer with zero.
  //   for ( i = 0; i< MIXBUFFERSIZE; i++ )
  //     mixbuffer[i] = 0;

  // Finished initialization.
  fprintf(stdout, "I_InitSound: sound module ready\n");
}

void I_InitMusic(void) {}

//
// Starting a sound means adding it
//  to the current list of active sounds
//  in the internal channels.
// As the SFX info struct contains
//  e.g. a pointer to the raw data,
//  it is ignored.
// As our sound handling does not handle
//  priority, it is ignored.
// Pitching (that is, increased speed of playback)
//  is set, but currently not used by mixing.
//
int I_StartSound(int id, int vol, int sep, int pitch, int priority) {

  // UNUSED
  priority = 0;

  // Debug.
  // fprintf( stderr, "starting sound %d", id );

  // Returns a handle (not used).
  id = addsfx(id, vol, steptable[pitch], sep);

  // fprintf( stderr, "/handle is %d\n", id );

  return id;
}

void I_StopSound(int handle) {
  // You need the handle returned by StartSound.
  // Would be looping all channels,
  //  tracking down the handle,
  //  an setting the channel to zero.

  // UNUSED.
  handle = 0;
}

int I_SoundIsPlaying(int handle) {
  // Ouch.
  return gametic < handle;
}

//
// This function loops all active (internal) sound
//  channels, retrieves a given number of samples
//  from the raw sound data, modifies it according
//  to the current (internal) channel parameters,
//  mixes the per channel samples into the global
//  mixbuffer, clamping it to the allowed range,
//  and sets up everything for transferring the
//  contents of the mixbuffer to the (two)
//  hardware channels (left and right, that is).
//
//  allegro does this now
//
void I_UpdateSound(void) {}

//
// This would be used to write out the mixbuffer
//  during each game loop update.
// Updates sound buffer and audio device at runtime.
// It is called during Timer interrupt with SNDINTR.
// Mixing now done synchronous, and
//  only output be done asynchronous?
//

void I_SubmitSound(void) {
  // this should no longer be necessary cuz allegro is doing all the sound mixing now
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch) {
  // I fail too see that this is used.
  // Would be using the handle to identify
  //  on which channel the sound might be active,
  //  and resetting the channel parameters.

  // UNUSED.
  handle = vol = sep = pitch = 0;
}

void I_ShutdownSound(void) {
  // Wait till all pending sounds are finished.
  int done = 0;
  int i;

  // FIXME (below).
  fprintf(stderr, "I_ShutdownSound: NOT finishing pending sounds\n");
  fflush(stderr);

  while (!done) {
    for (i = 0; i < 8 && !channels[i]; i++)
      ;

    // FIXME. No proper channel output.
    // if (i==8)
    done = 1;
  }

  // remove_sound();
  // Done.
  return;
}

void I_PauseSong(int handle) {}
void I_ResumeSong(int handle) {}
void I_SetMusicVolume(int volume) {}
void I_PlaySong(int handle, int looping) {}
void I_StopSong(int handle) {}
int I_RegisterSong(void *data) { return 1; }
void I_UnRegisterSong(int handle) {}
void I_ShutdownMusic(void) {}

