/**
 Author: 	Joel Denke and Marcus Isaksson
 Description: 	Runs the client, draw graphics and handle inputs.
 Created:	13 april 2012
 */

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdlib.h>
#include <stdio.h>

#include "includes/graphics/structure.h"
#include "includes/network/udp_protocol.h"
#include "includes/cbuffer.h"
#include "includes/jukebox.h"
#include "includes/graphics/draw.h"
#include "includes/graphics/timer.h"
#include "includes/graphics/editor.h"
#include "includes/graphics.h"

#define MESSAGE_SIZE 1500
#define BUFFER_SIZE 1
#define NO_BUFFERS 3

gState state;
world gameWorld;
cBuffer * cb[NO_BUFFERS];
connection_data * connection;

SDL_Event event;
SDL_Color * pColor = NULL;
SDL_Color * oColor = NULL;
SDL_mutex  *b_lock[NO_BUFFERS];
int no_players = 6;

/**
 * Author: 	 	Joel Denke
 * Description:		Connect client to server
 * Params: 		connection	Struct to save connection data to
 * 			server_ip	Server ip
 */
int clientConnect(connection_data * connection, char * server_ip)
{
	int result1, result2;
	uint16_t server_port = 1025;

	srand(time(NULL));
	uint16_t client_port = (1026 + rand() % (5000 - 1026));

	printf("Client port: %d\n", client_port);
	printf("--------- Start trying to connect client: %p -----------\n", connection);
	printf("Server_ip: %s (%d)\n", server_ip, (int)strlen(server_ip));
	
	return connect_session(connection, server_ip, server_port, client_port);
}

/**
 * Author: 	 	Joel Denke
 * Description:		Move the player by send the data to server
 * Params: 		movement	Movement data buffer to parse
 */
void movePlayer(char * movement)
{
	char string[20];
	snprintf(string, 20, "%d,%s", connection->client_id, movement);

	send_data(connection, string);
}

/**
 * Author: 	 	Joel Denke
 * Description:		Listen for input
 * Params:		keysHeld Key statements for all buttons
 */
void listenInput(int * keysHeld)
{
	struct timer_t2 fps;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN :
				keysHeld[event.key.keysym.sym] = 1;
				break;
			case SDL_KEYUP :
				keysHeld[event.key.keysym.sym] = 0;
				break;
			case SDL_QUIT :
				state = gExit;
				break;
		}
	}

	if (keysHeld[SDLK_ESCAPE])
	{
		state = gExit;
	}

	if (keysHeld[SDLK_LEFT])
	{
		movePlayer("left");
	}

	if (keysHeld[SDLK_RIGHT])
	{
		movePlayer("right");
	}

	if (keysHeld[SDLK_UP])
	{
		movePlayer("up");
	}

	if (keysHeld[SDLK_1] || keysHeld[SDLK_2] || keysHeld[SDLK_3] || keysHeld[SDLK_m])
	{
		timer_start(&fps);
		
		if (keysHeld[SDLK_m])
		{
			pauseMusic();
		} else {
			int k;
			
			if (keysHeld[SDLK_1]) {
				k = 0;
			} else if (keysHeld[SDLK_2]) {
				k = 1;
			} else if (keysHeld[SDLK_3]) {
				k = 2;
			}
			
			playMusic(k);
		}

		if (timer_get_ticks(&fps) < 1000)
		{
			//delay the as much time as we need to get desired frames per second
			SDL_Delay((1000) - timer_get_ticks(&fps));
		}
	}
}

/**
 * Author: 	 	Joel Denke
 * Description:		Listen for data send by server and save to buffer
 * Return value: 	0 to signal game is ended
 */
int listenEventBuffer()
{
	int i, type, id;

	char * data = malloc(sizeof(char) * MESSAGE_SIZE);
	char * tmp = malloc(sizeof(char) * MESSAGE_SIZE);

	while (1) {
		if (state == gExit) {
			break;
		}

		data = (char*)recv_data(connection, 60);

		printf("Data %s\n", data);

		if (data != NULL) {
			type = atoi((char*)strsep(&data, ","));

			switch (type) {
				case 1:
				case 2:
				case 3:
					printf("Write %s to slot %d\n", data, type);
					SDL_mutexP(b_lock[type-1]);
					writeSlot(cb[type-1], data);
					SDL_mutexV(b_lock[type-1]);
					break;
			}
		}
	}

	return 0;
}

/**
 Author: 	Joel Denke, Marcus Isaksson
 Description: 	Draw all graphics
 */
void drawGraphics()
{
	int i;
	startDraw();
	drawBackground(SCREEN_WIDTH,SCREEN_HEIGHT);
	drawObjects(&gameWorld, NO_OBJECTS, oColor);
	drawPlayers(gameWorld.players, no_players, pColor);
	endDraw();
}

/**
 Author: 	Joel Denke
 Description: 	Update coordinates of objects or players
 Params: 	type		Player or object type id
		id		Identification of player or object
		x		New x coordinate
		y		New y coordinates
 data	Event data to parse and run
 */
void updateCoordinates(int type, int id, int x, int y)
{
	int yb;

	switch (type) {
		case 1:
			printf("Change player coordinate to [%d, %d]\n", x ,y);
			gameWorld.players[id-1].box.x = x;
			gameWorld.players[id-1].box.y = y;
			break;
		case 2:
			printf("Change object coordinate to [%d, %d]\n", x ,y);
			gameWorld.objects[id].box.x = x;
			gameWorld.objects[id].box.y = y;
			break;
	}
}


/**
 Author: 	Joel Denke
 Description: 	Do action on message sent from server
 Params:	id	id of team or player
		message Message action
 */
void messageAction(int id, char * message)
{
	int i;
	
	if (!strcmp(message, "noplayers")) {
		no_players = id;
		state = gInit;
	} else if (!strcmp(message, "win")) {
		for (i = 0; i < NO_BUFFERS; i++) {
			cleanSlots(cb[i]);
		}

		startDraw();
		drawWinScr(SCREEN_WIDTH, SCREEN_HEIGHT, id);
		endDraw();

		if (id == gameWorld.players[connection->client_id-1].team) {
			printf("My team won! We are the winners ...\n");
		} else {
			printf("I loose, to bad! (My team sucks ...)\n");
		}
		
		SDL_Delay(10000);
		state = gInit;
	} else if (!strcmp(message, "quit")) {
		printf("Player %d has quit\n", id);
	}
}

/**
 Author: 	Joel Denke
 Description: 	Runs event
 Params: 	eventData	Event data to parse and run
 */
void runEvent(int type, char * data)
{
	int id, x , y;
	int xb, yb;
	char * msg = malloc(sizeof(char) * MESSAGE_SIZE);
	
	if (data != NULL) {
		id = atoi(strsep(&data, ","));

		switch (type) {
			case 1:
			case 2:
				if (data != NULL) {
					x = atoi(strsep(&data, ","));

					if (data != NULL) {
						y = atoi(strsep(&data, ","));
						updateCoordinates(type, id, x, y);
					}
				}
				break;
			case 3:
				if (data != NULL) {
					msg = strsep(&data, ",");
					messageAction(id, msg);
				}

				break;
		}
	} else {
		printf("Incomplete packet string, discarding event action\n");
	}
}

void printWorld()
{
	int i;
	player p;
	object b;

	printf("------------ Print world data ----------------\n");

	for (i = 0; i < no_players; i++) {
		p = gameWorld.players[i];
		printf("Player %d, contains [%d %d %d %d]\n", p.id, p.box.x, p.box.y, p.box.width, p.box.height);
	}

	for (i = 0; i < NO_OBJECTS; i++) {
		b = gameWorld.objects[i];
		printf("Object %d, contains [%d %d %d %d] and delta [%d, %d]\n", b.id, b.box.x, b.box.y, b.box.width, b.box.height, b.box.dx, b.box.dy);
	}

	printf("----------------- End of world data ------------------------\n");
}

/**
 Author: 	Joel Denke, Marcus Isaksson
 Description: 	Initiates a player
 Params: 	i		Element position for player
		connection	Connection data for the player
 */
void initPlayer(int i, connection_data * connection)
{
	gameWorld.players[i].id         = i+1;
	gameWorld.players[i].box.dx     = BOX_DX;
	gameWorld.players[i].box.dy     = BOX_DY;
	gameWorld.players[i].box.x      = gameWorld.objects[0].box.x;
	gameWorld.players[i].box.y      = gameWorld.objects[0].box.y;
	gameWorld.players[i].box.width  = PLAYER_WIDTH;
	gameWorld.players[i].box.height = PLAYER_HEIGHT;

	if (i % 2) {
		gameWorld.players[i].team = 0;
	} else {
		gameWorld.players[i].team = 1;
	}
}

/**
 Author: 	Joel Denke
 Description: 	Exit the client and clean up after it
 */
void exitClient(SDL_Thread * thread)
{
	SDL_WaitThread(thread, NULL);
	clean_up();
	SDL_Quit();
	exit(0);
}


/**
 Author: 	Joel Denke
 Description: 	Init the world by loading map and players
 */
void initWorld()
{
	int i;
	
	mapLoad(&gameWorld, MAP_FILE);
	drawGraphics();
	
	for (i = 0; i < no_players; i++) {
		initPlayer(i, connection);
	}
	
	printWorld();
	state = gRunning;
}

/**
 Author: 	Joel Denke
 Description: 	Reads data from buffers and run it
 */
int runData(int i)
{
	char * elem = malloc(sizeof(char) * MESSAGE_SIZE);
	
	SDL_mutexP(b_lock[i]);
	if (!slotsEmpty(cb[i])) {
		elem = readSlot(cb[i]);
		
		printf("Get data %s from slot %d\n", elem, i+1);
		
		if (elem != NULL) {
			runEvent(i+1, elem);
		}
	}
	SDL_mutexV(b_lock[i]);
}

/**
 * Author: 	 	Joel Denke, Marcus Isaksson
 * Description:		Run the game on client
 */
int main(int argc, char *argv[])
{
	int i, j, no, yb, keysHeld[323] = {0};
	int result = 0;
	SDL_Thread * eventBuffer;
	SDL_Thread * runBuffer;
	struct timer_t2 fps;
	
	char * server_ip = malloc(sizeof(char) * 16);
	char * elem = malloc(sizeof(char) * MESSAGE_SIZE);

	pColor = malloc(sizeof(SDL_Color));
	oColor = malloc(sizeof(SDL_Color));
	connection = malloc(sizeof(connection_data));

	for (i = 0; i < NO_BUFFERS; i++) {
		cb[i] = malloc(sizeof(cBuffer));
		b_lock[i] = SDL_CreateMutex();
	}

	strcpy(server_ip, "127.0.0.1");

	pColor->r = 0;
	pColor->g = 255;
	pColor->b = 255;
	oColor->r = 0;
	oColor->g = 0;
	oColor->b = 255;

	initGraphics();
	initSound();

	printf("Render menu\n");
	graphicsMenu(&gameWorld, server_ip);

	initSlots(cb[0], BUFFER_SIZE);
	initSlots(cb[1], NO_OBJECTS);
	initSlots(cb[2], BUFFER_SIZE);

	state = gStart;

	if (clientConnect(connection, server_ip) == 0)
	{
		eventBuffer = SDL_CreateThread(listenEventBuffer, &connection);

		while (1) {
			switch (state) {
				case gStart:
					runData(2);
					break;
				case gInit:
					timer_start(&fps);
					startDraw();
					drawLoadScr(SCREEN_WIDTH, SCREEN_HEIGHT);
					endDraw();

					initWorld();
					
					if (timer_get_ticks(&fps) < 1000 / FPS)
					{
						//delay the as much time as we need to get desired frames per second
						SDL_Delay( ( 1000 / FPS ) - timer_get_ticks(&fps) );
					}
					break;
				case gRunning :
					timer_start(&fps);
					drawGraphics();
					listenInput(keysHeld);

					// i = 0: players; i = 1: objects; i = 2: messages
					for (i = 0; i < NO_BUFFERS; i++) {
						runData(i);
					}

					if (timer_get_ticks(&fps) < 1000 / FPS)
					{
						//delay the as much time as we need to get desired frames per second
						SDL_Delay( ( 1000 / FPS ) - timer_get_ticks(&fps) );
					}

					break;
				case gExit :
					//sprintf(string, "%d,quit", connection->client_id);
					printf("Freeing music now\n");
					pauseMusic();
					freeMusic();
					end_session(connection);
					printf("Player is exit game now\n");
					exitClient(eventBuffer);
					break;
				default :
					printf("test\n");
					break;
			}
		}
	} else {
		printf("Misslyckade med att kontakta servern på ip-adress: '%s'\n", server_ip);
		state = gExit;
		pauseMusic();
		freeMusic();
		exitClient(eventBuffer);
	}

	return 0;
}
