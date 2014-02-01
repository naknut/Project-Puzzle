/**
 Author: 	Joel Denke
 Description: 	Circular buffer
 Created:	5 maj 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _CBUFFER_H
#define _CBUFFER_H

/* Buffer slot */
typedef struct { char * value; } slot;

/* Circular buffer struct */
typedef struct {
	int		size;
	int		head;
	int		index;
	char		**slots;
} cBuffer;

/**
 Author: 	Joel Denke
 Description: 	Clean slots in circular buffer
 Params: 	cBuffer *	Contains data of the circular buffer
 */
void cleanSlots(cBuffer *);

/**
 Author: 	Joel Denke
 Description: 	Initiate circular buffer with fixed buffer size
 Params: 	cBuffer *	Contains data of the circular buffer
		int		Size of buffer slot and to allocate memory
 */
void initSlots(cBuffer *, int);

/**
 Author: 	Joel Denke
 Description: 	Check if all slots are taken or not
 Params: 	cBuffer *	Contains data of the circular buffer
 Return value:  0 if not full and 1 if it is
 */
int slotsFull(cBuffer *);

/**
 A u*thor: 	Joel Denke
 Description: 	Check if all slots are open or not
 Params: 	cBuffer *	Contains data of the circular buffer
 Return value:  0 if not empty and 1 if it is
 */
int slotsEmpty(cBuffer *);

/**
 Author: 	Joel Denke
 Description: 	Write data to a slot 
 Params: 	cBuffer *	Contains data of the circular buffer
		char *		Data to store in the slot
 */
void writeSlot(cBuffer *, char *);

/**
 Author: 	Joel Denke
 Description: 	Read from current slot index
 Params: 	cBuffer *	Contains data of the circular buffer
 Return value:  The slot
 */
char * readSlot(cBuffer *);

/**
 Author: 	Joel Denke
 Description: 	Create slot element
 Params: 	char *	Data to store in the slot element
 Return value:  The slot
 */
slot createSlot(char *);

#endif
