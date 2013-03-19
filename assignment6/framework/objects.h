#pragma once

#ifndef M_PI
#define M_PI 4.0 * atanf(1.0f)
#endif

#include <cstdio>

#include <GL/gl.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>

#include "levels.h"

class Circle {
	protected:
		float radius;
		b2Body *body;
	public:
		void render(void);
		virtual int getType(void){
			return 0;
		};
};

class Ball : public Circle {
	public:
		Ball(b2World *, level_t *, float, float);
		int getType(void){
			return 2;
		};
};

class EndPoint : public Circle {
	public:
		EndPoint(b2World *, level_t *, float);
		int getType(void){
			return 1;
		};
};

class Polygon {
	b2Body *body;
	b2PolygonShape *shape;
	public:
		Polygon(b2World *, poly_t *, int, float);
		~Polygon(void);
		void render(void);
		
};

class WinObject : public b2ContactListener {
	int win;
	public:
		WinObject(void){
			win = 0;
		};
		void reset(void){
			win = 0;
		};
		int hasWon(void){
			return win;
		};
		void render(void);
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
};
