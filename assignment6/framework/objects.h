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

