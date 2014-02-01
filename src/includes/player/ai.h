/**
 * Contains structs and prototype functions for modifying the game world.
 *
 * @author Joel Denke      	mail@happyness.se
 * @author Marcus Isaksson	marcus.is@telia.com
 * @date   12 april 2012
 */

#include <stdio.h>
#include "../graphics/structure.h"
#include "controller.h"

#ifndef _AI_H
#define _AI_H

int initAI(world *);
void moveAI(world *);
void swithDirection(world *);

#endif