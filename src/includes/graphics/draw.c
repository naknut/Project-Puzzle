#ifndef _C_WORLD
#define _C_WORLD

#include "draw.h"
#include "structure.h"

levels level;
camera cam;
SDL_Surface *letters = NULL;
SDL_Surface *ipfield = NULL;
SDL_Rect abc[95];
textures texture;

void init_options()
{
    texture.background = load_imageRGBA("menu/main_menu_background.png");
}
void init_draw()
{
    char * load_scr     = "menu/loading_screen.png";
    char * team1_win    = "lag1.png";
    char * team2_win    = "lag2.png";
	char * couch        = "object/soffa.png";
	char * wall         = "object/wall.png";
	char * floor        = "object/floor.png";
	char * table        = "object/table.png";
	char * stair        = "object/stair.png";
	char * fence        = "object/fence.png";
	char * box          = "object/box.png";
	char * finish       = "object/finish.png";
	char * start        = "object/start.png";
	char * player_1     = "player/player1.png";
	char * player_2     = "player/player2.png";
	char * player_3     = "player/player3.png";
	char * player_4     = "player/player4.png";
	char * player_5     = "player/player5.png";
	char * player_6     = "player/player6.png";

	printf("before Load images\n");

    texture.load_scr    = load_imageRGBA(load_scr);
    texture.team1_win   = load_imageRGBA(team1_win);
    texture.team2_win   = load_imageRGBA(team2_win);
	texture.couch       = load_imageRGBA(couch);
	texture.wall        = load_imageRGBA(wall);
	texture.floor       = load_imageRGBA(floor);
	texture.table       = load_imageRGBA(table);
	texture.stair       = load_imageRGBA(stair);
	texture.fence       = load_imageRGBA(fence);
	texture.box         = load_imageRGBA(box);
	texture.finish      = load_imageRGBA(finish);
	texture.start       = load_imageRGBA(start);
	texture.player_1    = load_imageRGBA(player_1);
	texture.player_2    = load_imageRGBA(player_2);
	texture.player_3    = load_imageRGBA(player_3);
	texture.player_4    = load_imageRGBA(player_4);
	texture.player_5    = load_imageRGBA(player_5);
	texture.player_6    = load_imageRGBA(player_6);
}
void init_font()
{
    letters = IMG_Load("textures/font.png");
    int i;
    for(i=0;i<95;i++)
    {
        abc[i].x = 9 * i;
        abc[i].y = 0;
        abc[i].w = 9;
        abc[i].h = 9;
    }
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;
    //Get offsets
    offset.x = x;
    offset.y = y;
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void apply_letter(char *string)
{
    ipfield = IMG_Load("textures/ipfield.png");
    //    printf("String in applyletter (%s)\n",string);
    int i;
    for(i=0;i<strlen(string);i++)
    {
        if(string[i] != '\0')
        {
	        //printf("applying char (%c)\n",string[i]);
            apply_surface(90 + (i*9), 3, letters, ipfield, &abc[string[i]-32]);
	        //printf("successeding to apply (%c)\n",string[i]);

        }
        else
            break;
    }
}
void apply_texture()
{
    if(ipfield)
    {
      //  printf("applying surfaces to texture\n");
        //SDL_SetAlpha(ipfield,SDL_SRCALPHA,0);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glGenTextures(1, &texture.ip_texture);
        glBindTexture(GL_TEXTURE_2D, texture.ip_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ipfield->w, ipfield->h,
        0, GL_RGBA, GL_UNSIGNED_BYTE, ipfield->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	printf("texture was successfully created\n");
    }
    else
    {
        printf("could not make texture (ip_texture)\n");
        return;
    }
}
///**
// * Author: Robin
// * Description: Initializing player, texture, camera position, gravity.
// *
// */
//void player_init(char *file)
//{
//	gplayers.char_texture = load_imageRGBA(file);
//    //gplayers.font_texture = load_text("Player 1");
//}

GLuint load_imageRGBA(char *file)
{
	char * string = malloc(sizeof(char) * 1500);
	sprintf(string, "%s%s", "textures/", file);
	printf("Try to load texture %s\n", string);
	SDL_Surface *tex = IMG_Load(string); //textures/file.png

    GLuint texture = 0;

    if(tex)
    {
        //SDL_SetAlpha(tex,SDL_SRCALPHA,0);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, tex->format->BytesPerPixel, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_FreeSurface(tex);
    }
    else
    {
        printf("Texture couldn't load (%s)\n",string);
        return -1;
    }
    
    free(string);
    
    //printf("Texture loaded correctly %s\n",file);
    return texture;
}

void load_level()
{
    char * background = "level_background.png";
    texture.bg_texture  = load_imageRGBA(background);
}

void camera_move(player * players, int local_player_id, int screen_max_x, int screen_max_y)
{
    if( (players[local_player_id].box.x + players[local_player_id].box.width + cam.x) > (screen_max_x-100) )
    {
        cam.x++;
    }
    if( (players[local_player_id].box.x + cam.x) < 100 )
    {
        cam.x--;
    }
    if( (players[local_player_id].box.y + players[local_player_id].box.height + cam.y) > (screen_max_y-50) )
    {
        cam.y++;
    }
    if( (players[local_player_id].box.y + cam.y) < 50 )
    {
        cam.y--;
    }
}
void bind_texture(int object_id, world *gameWorld)
{
    switch(gameWorld->objects[object_id].property)
    {
        case 1:
            glBindTexture(GL_TEXTURE_2D, texture.couch);
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, texture.floor);
            break;
        case 3:
            glBindTexture(GL_TEXTURE_2D, texture.wall);
            break;
        case 4:
            glBindTexture(GL_TEXTURE_2D, texture.stair);
            break;
        case 5:
            glBindTexture(GL_TEXTURE_2D, texture.fence);
            break;
        case 6:
            glBindTexture(GL_TEXTURE_2D, texture.table);
            break;
        case 7:
            glBindTexture(GL_TEXTURE_2D, texture.box);
            break;
        case 8:
            glBindTexture(GL_TEXTURE_2D, texture.wall);
            break;
        case 'e':
            glBindTexture(GL_TEXTURE_2D, texture.finish);
            break;
        case 's':
            glBindTexture(GL_TEXTURE_2D, texture.start);
            break;
        default:
            glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
            break;
    }
}

void player_texture(player * p)
{
    switch(p->id)
    {
        case 1:
            glBindTexture(GL_TEXTURE_2D, texture.player_1);
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, texture.player_2);
            break;
        case 3:
            glBindTexture(GL_TEXTURE_2D, texture.player_3);
            break;
        case 4:
            glBindTexture(GL_TEXTURE_2D, texture.player_4);
            break;
        case 5:
            glBindTexture(GL_TEXTURE_2D, texture.player_5);
            break;
        case 6:
            glBindTexture(GL_TEXTURE_2D, texture.player_6);
            break;
        default:
          glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
            break;
    }
}

void drawLoadScr(int screen_max_x, int screen_max_y)
{
    glTranslatef(0, 0, 0);

    glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture.load_scr);
        glTranslatef(0,0,0);
        glBegin( GL_QUADS );
            //Draw options
            glTexCoord2f(0.0f, 0.0f); glVertex2f(            0,            0);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( screen_max_x,            0);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( screen_max_x, screen_max_y);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(            0, screen_max_y);
        glEnd();
    glPopMatrix();
    glLoadIdentity();
}

void drawWinScr(int screen_max_x, int screen_max_y, int team)
{
    glTranslatef(0, 0, 0);

    glPushMatrix();
	switch (team) {
		case 1 :
			glBindTexture(GL_TEXTURE_2D, texture.team1_win);
			break;
		case 2 :
			glBindTexture(GL_TEXTURE_2D, texture.team2_win);
			break;
		default :
			printf("No one wins\n");
			break;
	}
        glTranslatef( (screen_max_x-160)/2,200,0);
        glBegin( GL_QUADS );
            //Draw options
            glTexCoord2f(0.0f, 0.0f); glVertex2f(   0,  0);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( 160,  0);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( 160, 28);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(   0, 28);
        glEnd();
    glPopMatrix();
    glLoadIdentity();
}

void drawOptions(int screen_max_x, int screen_max_y)
{
     //Move to offset
    glTranslatef(0, 0, 0);

    glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture.background);
        glTranslatef(0,0,0);
        glBegin( GL_QUADS );
            //Draw options
            glTexCoord2f(0.0f, 0.0f); glVertex2f(            0,            0);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( screen_max_x,            0);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( screen_max_x, screen_max_y);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(            0, screen_max_y);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture.ip_texture);
        glTranslatef( (screen_max_x - ipfield->w)/2, screen_max_y/2, 0 );
        glBegin( GL_QUADS );
            //Draw options
            glTexCoord2f(0.0f, 0.0f); glVertex2f(          0,         0);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( ipfield->w,         0);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( ipfield->w, ipfield->h);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(          0, ipfield->h);
        glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
    //Reset
    glLoadIdentity();
}
void drawPlayers(player * players, int noPlayers, SDL_Color * color)
{
	int i;

	for (i = 0; i < noPlayers; i++) {
	    player_texture(&players[i]);
		drawBox(&players[i].box, color);
	}
}

void drawObjects(world * gameWorld, int noObjects, SDL_Color * color)
{
	int i,j,used_objects=0;
    for(j=0; j<NO_OBJECTS;j++)
    {
        if (gameWorld->objects[j].property == 0)
            break;
        else
            used_objects++;
    }

	for (i = 0; i < used_objects; i++) {
	    bind_texture(i,gameWorld);
	    drawBox(&gameWorld->objects[i].box, color);
	}
}

// Rita ut en box
void drawBox(box * block, SDL_Color * color)
{
	//printf("Start drawing box [%d %d %d %d]\n", block->x, block->y, block->width, block->height);
	//Move to offset
	glPushMatrix();
        glTranslatef( block->x, block->y, 0 );
        glBegin( GL_QUADS );

            glColor3f( color->r, color->g, color->b);
            //Draw Box
            glTexCoord2f(0,0); glVertex2f(            0,             0 );
            glTexCoord2f(1,0); glVertex2f( block->width,    	      0 );
            glTexCoord2f(1,1); glVertex2f( block->width, block->height );
            glTexCoord2f(0,1); glVertex2f(            0, block->height );

        glEnd();
	glPopMatrix();
	glLoadIdentity();
}

void drawProgBarPos(player * players)
{

    glPushMatrix();
        if(players->team == 1)
        {
            glColor3f(1,0,0);
        }
        if(players->team == 2)
        {
            glColor3f(0,0,1);
        }
        //glTranslatef( 20 + players->box.x - (40*(players->box.x/SCREEN_WIDTH/*for now, egentligen maplength*/)), 10, 0 );
        glBegin( GL_QUADS );
            //Draw progress bar position
            glVertex2f(  0,  0 );
            glVertex2f( -5, 10 );
            glVertex2f(  5, 10 );
        glEnd();
        glColor3f(1,1,1);
    glPopMatrix();

    glLoadIdentity();
}
void drawBackground(int screen_max_x, int screen_max_y)
{
    glPushMatrix();
        glTranslatef( 0, 0, 0 );
        glBindTexture(GL_TEXTURE_2D, texture.bg_texture);
        glBegin( GL_QUADS );
            //Draw Box
            glTexCoord2f(0,0); glVertex2f(            0,            0 );
            glTexCoord2f(1,0); glVertex2f( screen_max_x,    	    0 );
            glTexCoord2f(1,1); glVertex2f( screen_max_x, screen_max_y );
            glTexCoord2f(0,1); glVertex2f(            0, screen_max_y );

        glEnd();
	glPopMatrix();
	glLoadIdentity();
}
//Clear the screen
void startDraw()
{
//    printf("Start draw graphics\n");
	glClear( GL_COLOR_BUFFER_BIT );
}

//Update screen
void endDraw()
{
//    printf("End draw graphics\n");
	SDL_GL_SwapBuffers();
}

void clean_up_world()
{
    glDeleteTextures(1,&texture.load_scr);
    glDeleteTextures(1,&texture.couch);
    glDeleteTextures(1,&texture.wall);
    glDeleteTextures(1,&texture.floor);
    glDeleteTextures(1,&texture.table);
    glDeleteTextures(1,&texture.stair);
    glDeleteTextures(1,&texture.fence);
    glDeleteTextures(1,&texture.box);
    glDeleteTextures(1,&texture.player_1);
    glDeleteTextures(1,&texture.player_2);
    glDeleteTextures(1,&texture.player_3);
    glDeleteTextures(1,&texture.player_4);
    glDeleteTextures(1,&texture.player_5);
    glDeleteTextures(1,&texture.player_6);
}
#endif
