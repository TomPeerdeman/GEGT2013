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

void WinObject::render(void) {
	if(win){
		glColor3f(1.0f, 0, 0);
		glBegin(GL_QUADS);
			glVertex2f(2.5, 2);
			glVertex2f(2.5, 4);
			glVertex2f(5.5, 4);
			glVertex2f(5.5, 2);
		glEnd();
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		//glTranslatef(3.0f, 2.5f, 0.0f);
		glRasterPos2f(3.5f, 3.0f);
		char *msg = (char *) "YOU WIN!";
		for(unsigned int i = 0; i < strlen(msg); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
		}
		//glutBitmapString(GLUT_STROKE_ROMAN, msg);
		glPopMatrix();
	
	}
}

void WinObject::BeginContact(b2Contact* contact) {
	void *userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	
	if(userDataA != NULL && userDataB != NULL){
		Circle *A = static_cast<Circle *> (userDataA);
		Circle *B = static_cast<Circle *> (userDataB);
		
		if(A->getType() + B->getType() == 3){
			win = 1;
		}
	}
}

void WinObject::EndContact(b2Contact* contact) {
}
