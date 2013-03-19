#pragma once

#ifndef M_PI
#define M_PI 4.0 * atanf(1.0f)
#endif

#include <GL/gl.h>
#include <Box2D/Box2D.h>

#include "levels.h"

class Ball {
	float radius;
	b2Body *body;
	public:
		Ball(b2World *, level_t *, float, float);
		void render(void);
};

class Polygon {
	b2Body *body;
	b2PolygonShape *shape;
	public:
		Polygon(b2World *, poly_t *, int, float);
		~Polygon(void);
		void render(void);
		b2PolygonShape *GetShape(void){
			return shape;
		};
};
