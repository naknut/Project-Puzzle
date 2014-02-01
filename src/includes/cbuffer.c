#include "cbuffer.h"

void cleanSlots(cBuffer * cb)
{
	int i;
	
	cb->head = 0;
	cb->index = 0;

	for (i = 0; i < cb->size; i++) {
		cb->slots[i] = NULL;
	}
}

void initSlots(cBuffer * cb, int size)
{	
	int i;
	cb->size = size;
	cb->head = 0;
	cb->index = 0;
	cb->slots = (char*)calloc(size, sizeof(char *));
	
	if (cb->slots == NULL) {
		printf("Failed to allocate buffer with size %d\n", size);
	}
}

int slotsFull(cBuffer *cb)
{
	if (cb == NULL) 
	{
		return -1;
	}
	
	return cb->index == cb->size;
}

int slotsEmpty(cBuffer *cb)
{
	if (cb == NULL)
	{
		return -1;
	}
	
	return cb->index == 0;
}

/*
slot createSlot(char * data)
{
	slot elem;
	elem.value = data;
	
	return elem;
}*/

void writeSlot(cBuffer *cb, char * data)
{
	if (cb == NULL)
	{
		return;
	}
	
	int end = (cb->head + cb->index) % cb->size;
	cb->slots[end] = data;
	
	if (cb->index == cb->size) {
		cb->head = (cb->head + 1) % cb->size; /* full, overwrite */
	} else {
		++cb->index;
	}
}

char * readSlot(cBuffer *cb)
{
	if (cb == NULL)
	{
		return NULL;
	}
	
	char * elem = cb->slots[cb->head];
	cb->head = (cb->head + 1) % cb->size;
	--cb->index;
	
	return elem;
}