#include "objects.h"

Polygon::Polygon(b2World *world, poly_t *polys, int dynamic, float d) {
	shape = new b2PolygonShape;
	
	b2Vec2 *vertices = new b2Vec2[polys->num_verts];
	for(unsigned int j = 0; j < polys->num_verts; j++){
		vertices[j].Set(polys->verts[j].x, polys->verts[j].y);
	}

	shape->Set(vertices, polys->num_verts);
	
	b2BodyDef bodyDef;
	if(dynamic) {
		bodyDef.type = b2_dynamicBody;
	}
	
	body = world->CreateBody(&bodyDef);
	
	body->CreateFixture(shape, d);
}

Polygon::~Polygon() {
	if(shape != NULL) {
		delete shape;
	}
}

void Polygon::render(void) {
	

}