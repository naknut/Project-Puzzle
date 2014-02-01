#include "structure.h"

#ifndef _C_STRUCTURE
#define _C_STRUCTURE

char * getMapFile(int i)
{
	
}

void mapSave(world * gameWorld, char * map)
{
	FILE *save_map;

	printf("saving map\n");

	save_map = fopen(map, "wb");
	fwrite(&gameWorld->objects, sizeof(object), NO_OBJECTS, save_map);
	fclose(save_map);
}

int mapLoad(world * gameWorld, char * map)
{
	FILE *load_map;

	printf("Loading map\n");

	load_map = fopen(map, "rb");
	fread(gameWorld->objects, sizeof(object), NO_OBJECTS, load_map);
	fclose(load_map);

	int i;
	for(i = 0; i < NO_OBJECTS; i++)
	{
		if (gameWorld->objects[i].property == 0)
		{
			return i - 1;
		}
	}
}
#endif
