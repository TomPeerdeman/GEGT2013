#include "objects.h"

void Circle::render(void) {
	b2Vec2 position = body->GetPosition();

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x, position.y);
	
	/*
	 * Source:
	 * http://stackoverflow.com/questions/5094992/c-drawing-a-2d-disk-in-opengl
	 */
	for(int i = 0; i <= 100; i++){
	    float const t = 2 * M_PI * (float) i / 100.0f;
		glColor3f((sin(t + body->GetAngle()) + cos(t)) / 2.0f, (cos(t + body->GetAngle()) + tan(t)) / 2.0f, (tan(t + body->GetAngle()) + sin(t)) /2.0f );
	    glVertex2f(position.x + sin(t) * radius, position.y + cos(t) * radius);
	}

	glEnd();
}

Ball::Ball(b2World *world, level_t *level, float r, float d) {
	radius = r;

	b2BodyDef bodyDef;

	bodyDef.position.Set(level->start.x, level->start.y);
	bodyDef.type = b2_dynamicBody;

	body = world->CreateBody(&bodyDef);
	
	// Test to not simulate the ball
	//body->SetAwake(0);
	//body->SetGravityScale(0.0f);

	b2CircleShape shape;
	shape.m_radius = r;

	body->CreateFixture(&shape, d);
	
	body->SetUserData(this);
}

EndPoint::EndPoint(b2World *world, level_t *level, float r) {
	radius = r;

	b2BodyDef bodyDef;

	bodyDef.position.Set(level->end.x, level->end.y);

	body = world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = r;

	body->CreateFixture(&shape, 0.0f);
	
	body->SetUserData(this);
}

Ground::Ground(b2World *world) {
	b2BodyDef bodyDef;

	bodyDef.position.Set(4.0f, 0.0f);
	
	body = world->CreateBody(&bodyDef);
	
	b2PolygonShape shape;
	shape.SetAsBox(4.0f, 0.01f);

	body->CreateFixture(&shape, 0.0f);
	
	body->SetUserData(this);
}

void WinObject::render(void) {
	char *msg = NULL;
	
	if(win) {
		msg = (char *) "YOU WIN!";
	}else if(lose) {
		msg = (char *) "YOU LOSE!";
	}
	
	if(win || lose){
		glColor3f(1.0f, 0, 0);
		glBegin(GL_QUADS);
			glVertex2f(2.5, 2);
			glVertex2f(2.5, 4);
			glVertex2f(5.5, 4);
			glVertex2f(5.5, 2);
		glEnd();
		
		glColor3f(1.0f, 1.0f, 1.0f);
		
		glRasterPos2f(3.5f, 3.0f);
		for(unsigned int i = 0; i < strlen(msg); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
		}
	}
}

void WinObject::BeginContact(b2Contact* contact) {
	void *userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	
	if(userDataA != NULL && userDataB != NULL){
		BodyObject *A = static_cast<BodyObject *> (userDataA);
		BodyObject *B = static_cast<BodyObject *> (userDataB);
		
		printf("Collision %d, %d\n", A->getType(), B->getType());
		
		if((A->getType() | B->getType()) == 3){
			win = 1;
		}else if((A->getType() | B->getType()) == 6) {
			lose = 1;
		}
	}
}

void WinObject::EndContact(b2Contact* contact) {
}
