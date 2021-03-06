#include "jukebox.h"

int musicMuted = 1;
int canPlayMusic = 1;
char * songs[NO_SONGS];
char * current = NULL;
Mix_Music * nowPlaying;

void initSound()
{
	int i;
	current = malloc(sizeof(char*));
	nowPlaying = malloc(sizeof(Mix_Music*));
	
	if (Mix_OpenAudio(BITRATE, AUDIO_S16SYS, CHANNELS, BUFFER) < 0) {
		printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
		canPlayMusic = 0;
	} else {
		printf("Successfully initialized SDL mixer audio\n");
		
		for (i = 0; i < NO_SONGS; i++) {
			songs[i] = malloc(sizeof(char) * 18);
			sprintf(songs[i], "music/song%d.mp3", i+1);
			printf("Song %s\n", songs[i]);
		}
	}
}

void freeMusic()
{
	Mix_CloseAudio();
}

void pauseMusic()
{
	if (nowPlaying != NULL && !musicMuted) {
		printf("Pausing music\n");
		Mix_HaltMusic();
		Mix_FreeMusic(nowPlaying);
		musicMuted = 1;
		current = NULL;
	}
}

void playMusic(int id)
{
	if (id >= 0 && id < 3 && canPlayMusic) {
		if (songs[id] == current) {
			pauseMusic();
		} else if (musicMuted || songs[id] != current) {
			printf("Playing music\n");
			nowPlaying = Mix_LoadMUS(songs[id]);
			
			if (nowPlaying == NULL) {
				printf("Mix_LoadMUS(\"%s\"): %s\n", songs[id], Mix_GetError());
			} else {
				Mix_PlayMusic(nowPlaying, 1);
				Mix_HookMusicFinished(loopbackMusic);
				musicMuted = 0;
				current = songs[id];
			}
		}
	}
}

void loopbackMusic()
{
	if (current != NULL) {
		nowPlaying = Mix_LoadMUS(current);
		
		if (nowPlaying == NULL) {
			printf("Mix_LoadMUS(\"%s\"): %s\n", current, Mix_GetError());
		} else {
			Mix_PlayMusic(nowPlaying, 1);
			Mix_HookMusicFinished(loopbackMusic);
		}
	}
}
