#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED
// headers from main.cpp
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif


#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Box2D/Box2D.h>
#include <stdio.h>

void initActor();

Uint8 * keystate;

b2World *world;
b2Body *body;
b2Body *actor;
b2Vec2 points[4];
Uint32 start;




#endif // MAIN_HPP_INCLUDED
