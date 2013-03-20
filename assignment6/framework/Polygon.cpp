#include "objects.h"

Polygon::Polygon(b2World *world, poly_t *polys, int b_dynamic, float d, GLuint *tex) {
	tex_2d = tex;
	b2BodyDef bodyDef;
	if(b_dynamic) {
		bodyDef.type = b2_dynamicBody;
	}
	
	body = world->CreateBody(&bodyDef);

	shape = new b2PolygonShape;
	
	b2Vec2 *vertices = new b2Vec2[polys->num_verts];
	for(unsigned int j = 0; j < polys->num_verts; j++){
		vertices[j].Set(polys->verts[j].x, polys->verts[j].y);
		vertices[j] = body->GetLocalPoint(vertices[j]);
	}

	shape->Set(vertices, polys->num_verts);
	
	fixture = body->CreateFixture(shape, d);
}

int Polygon::intersects(float x, float y) {
	b2Vec2 vec(x, y);
	return fixture->TestPoint(vec);
}

Polygon::~Polygon() {
	if(shape != NULL) {
		delete shape;
	}
}

void Polygon::render() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *tex_2d);
	glBegin(GL_POLYGON);
	
	for(int i = 0; i < shape->GetVertexCount(); i++){
		b2Vec2 vertex = shape->GetVertex(i);
		b2Vec2 point = body->GetWorldPoint(vertex);
		glTexCoord2d(vertex.x / 8.0f,vertex.y / 6.0f);
		glVertex2f(point.x, point.y);
	}
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

