/**
 * Contains structs and prototype functions for modifying the game world.
 *
 * @author Joel Denke      	mail@happyness.se
 * @author Marcus Isaksson	marcus.is@telia.com
 * @date   12 april 2012
 */

#include <stdio.h>
#include <unistd.h>
#include "graphics/structure.h"

#ifndef _CALC_H
#define _CALC_H

/**
 Author: 	Joel Denke
 Description: 	Function that test if two boxes intersects or not
 Params: 	box		Box one to test
		box 		Box two to test
 Return value:  1 if intersects and 0 if not
 */
int isCollision(box, box);

/**
 Author: 	Joel Denke
 Description: 	Function that test if two boxes intersects or not
 Params: 	box		Properties of a box
		direction 	Direction of the box.
 Return value:  Length to move
 */
int calculateLengthToMove(box, movement);
#endif