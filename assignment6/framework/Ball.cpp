#include "objects.h"

Ball::Ball(b2World *world, level_t *level, float r, float d){
	radius = r;

	b2BodyDef bodyDef;

	bodyDef.position.Set(level->start.x, level->start.y);
	bodyDef.type = b2_dynamicBody;

	body = world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = r;

	body->CreateFixture(&shape, d);
}

void Ball::render(void){
	b2Vec2 position = body->GetPosition();

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x, position.y);
	
	/*
	 * Source:
	 * http://stackoverflow.com/questions/5094992/c-drawing-a-2d-disk-in-opengl
	 */
	for(int i = 0; i <= 100; i++){
	    float const t = 2 * M_PI * (float) i / 100.0f;
	    glVertex2f(position.x + sin(t) * radius, position.y + cos(t) * radius);
	}

	glEnd();
}

