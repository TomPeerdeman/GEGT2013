#pragma once

#ifndef M_PI
#define M_PI 4.0 * atanf(1.0f)
#endif

#include <cstdio>

#include <GL/gl.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>

#include "levels.h"

class BodyObject {
	protected:
		b2Body *body;
	public:
		virtual void render(void){
		};
		virtual int getType(void){
			return 0;
		};
};

class Circle : public BodyObject {
	protected:
		float radius;
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
		void unlockMovement(void){
			body->SetAwake(true);
			body->SetGravityScale(1.0f);
		};
};

class EndPoint : public Circle {
	public:
		EndPoint(b2World *, level_t *, float);
		int getType(void){
			return 1;
		};
};

class Polygon : public BodyObject {
	b2PolygonShape *shape;
	b2Fixture *fixture;
	public:
		Polygon(b2World *, poly_t *, int, float);
		~Polygon(void);
		int intersects(float x, float y);
		void render(void);
		void print(){
			b2Vec2 pos = body->GetPosition();
			printf("POS x %f; y %f; Mass %f \n", pos.x, pos.y, body->GetMass());
		};
		void destroy(b2World *world){
			world->DestroyBody(body);
		};
};

class Ground : public BodyObject {
	public:
		Ground(b2World *);
		int getType(void){
			return 4;
		};
};

class WinObject : public b2ContactListener {
	int win;
	int lose;
	public:
		WinObject(void){
			win = 0;
			lose = 0;
		};
		void reset(void){
			win = 0;
			lose = 0;
		};
		int hasWon(void){
			return win;
		};
		int hasLost(void){
			return lose;
		};
		void render(void);
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
};
