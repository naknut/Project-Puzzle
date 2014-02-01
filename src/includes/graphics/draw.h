/**
 * Author
 * Name: Robin Jakobsson / Sarah Larsson
 * Email:    Rjak@kth.se / Saralar@kth.se
 * id:       910724-2696 / 910222-4848
 * Grupp: 2
 */

#ifndef _H_DRAW
#define _H_DRAW

#include <SDL.h>
#include <SDL_opengl.h>

#include "structure.h"
#include "editor.h"

typedef struct
{
    int x;
    int y;
} camera;

typedef struct
{
    int width;
    int height;
} levels;

//void player_init( char *file);
void init_options();
void init_font();
void init_draw();
void apply_surface();
void apply_letter();
void apply_texture();
/**
 *   Author: Robin
 *   Load up the image from any RGBA using image through SDL and convert it to openGL
 *
 *   *****IMPORTANT*****
 *   You can not use any image when using this. The image has to use RGBA setup (ALPHA technique)
 *   else the pixels will be unsynced and the image will be a completly mess.
 *
 */
GLuint load_imageRGBA(char *file);
/**
 * Author: Robin
 * Description: loads background.
 */
void load_level();
//void player_set_xy ( float x, float y);
//void player_set_velocity ( float xv, float yv);
//void player_set_jump ( int jump);

/**
 * Author: Sarah
 * Description: move the camera with the player if player trying to leave "camerabox".
 */
void camera_move(player * players, int local_player_id, int screen_max_x, int screen_max_y);
//void player_move( int screen_maxx, int screen_maxy);
//void player_jump( int screen_maxy);
void drawOptions(int screen_max_x, int screen_max_y);
void drawWinScr(int screen_max_x, int screen_max_y, int team);
void drawBox(box *, SDL_Color *);
void drawPlayers(player *, int, SDL_Color *);
void drawObjects(world * gameWorld, int, SDL_Color *);
void drawLoadScr(int screen_max_x, int screen_max_y);
//void world_show( int screen_max_x, int screen_max_y );

/**
 * Author: Robin
 * Description: Draw the players location on the progressbar.
 */
void drawProgBarPos(player * players);
void drawBackground(int screen_max_x, int screen_max_y);
void startDraw();
void endDraw();
/**
 * Author: Robin
 * Description: Frees the texture so we wont get memorey leaking.
 */
void clean_up_world();
#endif

