/**
 * Jukebox library
 *
 * Author: Joel Denke
 * Created:   25 maj 2012
 */

#ifndef _JUKEBOX_H
#define _JUKEBOX_H

#define NO_SONGS 3
#define BITRATE 22050
#define CHANNELS 2
#define BUFFER 4096

#include <stdio.h>
#include <SDL_mixer.h>

void initSound();
void freeMusic();
void pauseMusic();
void playMusic(int);
void loopbackMusic();

#endif
