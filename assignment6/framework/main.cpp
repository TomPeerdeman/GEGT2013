/* Computer Graphics and Game Technology, Assignment Box2D game
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <cstdio>

#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <Box2D/Box2D.h>

#include "levels.h"
#include "objects.h"

unsigned int reso_x = 800, reso_y = 600; // Window size in pixels
const float world_x = 8.f, world_y = 6.f; // Level (world) size in meters

int last_time;
int timebase;
int frame_count;

// information for user input
int mousecounter = 0;
int mousevert_x[4];
int mousevert_y[4];
float worldvert_x[4];
float worldvert_y[4];
float world_mouse_x;
float world_mouse_y;
int dpolylist_length;
int possible_totalpoly = 10;
Polygon **dpolygons = new Polygon *[possible_totalpoly];

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
level_t *levels;

unsigned int current_level;

b2World *world;
Ball *ball;
EndPoint *endPoint;
Ground *ground;
// Static polygons
int spolylist_length;
Polygon **spolygons;

Timer timer;

WinObject winObject(&timer);

/*
 * Load a given world, i.e. read the world from the `levels' data structure and
 * convert it into a Box2D world.
 */
void load_world(unsigned int level)
{
    if (level >= num_levels) {
        // Note that level is unsigned but we still use %d so -1 is shown as
        // such.
        printf("Warning: level %d does not exist.\n", level);
        return;
    }
	
	// Unload previous world
	if(world != NULL) {
		delete world;
		if(ball != NULL) {
			delete ball;
			ball = NULL;
		}
		if(ground != NULL) {
			delete ground;
			ground = NULL;
		}

		if(endPoint != NULL) {
			delete endPoint;
			endPoint = NULL;
		}
		
		if(spolygons != NULL){
			for(int i = 0; i < spolylist_length; i++) {
				if(spolygons[i] != NULL) {
					delete spolygons[i];
					spolygons[i] = NULL;
				}
			}
			spolylist_length = 0;
			delete[] spolygons;
			spolygons = NULL;
		}
		
		if(dpolygons != NULL){
			for(int i = 0; i < dpolylist_length; i++) {
				if(dpolygons[i] != NULL) {
					delete dpolygons[i];
					dpolygons[i] = NULL;
				}
			}
			dpolylist_length = 0;
		}

		world = NULL;
	}
	
	current_level = level;
	winObject.reset();

    // Create a Box2D world and populate it with all bodies for this level
    // (including the ball).
	
	b2Vec2 gravity(0.0f, -9.81f);
	
	// Create the world
	world = new b2World(gravity);
	
	ball = new Ball(world, &levels[level], 0.3f, 1.0f);
	endPoint = new EndPoint(world, &levels[level], 0.05f);
	ground = new Ground(world);
	
	world->SetContactListener(&winObject);
	
	// Static polygon's
	spolylist_length = levels[level].num_polygons;
	spolygons = new Polygon *[spolylist_length];
	for(int i = 0; i < spolylist_length; i++){
		// Static so immovable, set density to 0 for no gravity effect.
		spolygons[i] = new Polygon(world, &levels[level].polygons[i], 0, 0.0f);
	}

	timer.Reset();
}


/*
 * Called when we should redraw the scene (i.e. every frame).
 * It will show the current framerate in the window title.
 */
void draw(void)
{
    int time = glutGet(GLUT_ELAPSED_TIME);

    int frametime = time - last_time;
	// Fix first frame to simulate no time
	if(last_time == 0.0f) {
		frametime = 0.0f;
		timer.Reset();
	}
	last_time = time;

    frame_count++;

    // Clear the buffer
    glColor3f(0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
	
	if(world != NULL) {
		if(!winObject.hasWon() && !winObject.hasLost() && 1) {
			// Simulate the world
			world->Step((frametime / 1000.0f), 8, 3);
		}

		// Draw ball
		glColor3f(1.0f, 0.0f, 0.0f);
		if(ball != NULL)
			ball->render();
		
		glColor3f(1.0f, 1.0f, 1.0f);
		if(endPoint != NULL)
			endPoint->render();
		
		// Draw static polygons
		glColor3f(0.0f, 1.0f, 0.0f);
		for(int i = 0; i < spolylist_length; i++) {
			if(spolygons[i] == NULL)
				break;
			spolygons[i]->render();
		}
		
		// Draw dynamic polygons
		glColor3f(0.5f, 0.5f, 0.5f);
		for(int i = 0; i < dpolylist_length; i++) {
			if(dpolygons[i] == NULL)
				break;
			dpolygons[i]->render();
		}
		
		// Draw outline of object to build
		if(mousecounter >= 1){
			glColor3f(0.5f, 0.5f, 0.5f);
			
			// Draw the lines between the already placed vertices
			if(mousecounter >= 2){
				glBegin(GL_LINE_STRIP);
				for(int i = 0; i < mousecounter; i++) {
					glVertex2f(worldvert_x[i], worldvert_y[i]);
				}
				glEnd();
			}
			
			// Draw a line from the last placed vertex to the mouse position
			if(world_mouse_x > 0 && world_mouse_y > 0) {
				glBegin(GL_LINES);
					glVertex2f(worldvert_x[mousecounter - 1], 
					           worldvert_y[mousecounter - 1]);
					glVertex2f(world_mouse_x, world_mouse_y);
					// If we are placing the last vertex connect to the first 
					// to see the complete object
					if(mousecounter == 3) {
						glVertex2f(world_mouse_x, world_mouse_y);
						glVertex2f(worldvert_x[0], worldvert_y[0]);
					}
				glEnd();
			}
		}
		
		winObject.render();
	}

	timer.render();
	
    // Show rendered frame
    glutSwapBuffers();

    // Display fps in window title.
    if (time - timebase >= 1000) {
        char window_title[128];
        snprintf(window_title, 128,
                "Box2D: %3.2f fps, level %d/%d, Polygons left: %d/%d",
                frame_count / ((time - timebase) / 1000.f), 
				(current_level + 1), num_levels,
				possible_totalpoly-dpolylist_length, possible_totalpoly);
        glutSetWindowTitle(window_title);
        timebase = time;
        frame_count = 0;
    }
}

/*
 * Called when window is resized. We inform OpenGL about this, and save this
 * for future reference.
 */
void resize_window(int width, int height)
{
    glViewport(0, 0, width, height);
    reso_x = width;
    reso_y = height;
}

/*
 * Called when the user presses a key.
 */
void key_pressed(unsigned char key, int x, int y)
{
    switch (key)
    {
		case 81:
        case 27:
        case 'q':
            exit(0);
            break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			current_level = key - 49;
			load_world(current_level);
			break;
		case 32:
			if(winObject.hasWon()){
				load_world(current_level + 1);
				mousecounter = 0;
				break;
			}
			if(winObject.hasLost()){
				load_world(current_level);
				mousecounter = 0;
				break;
			}
			if(ball != NULL) {
				ball->unlockMovement();
			}
			break;
		case 26:
			if(dpolylist_length > 0 && dpolygons != NULL) {
				dpolygons[--dpolylist_length]->destroy(world);
				delete dpolygons[dpolylist_length];
				dpolygons[dpolylist_length] = NULL;
			}
			break;
		case 114:
			load_world(current_level);
			mousecounter = 0;
			break;
        // Add any keys you want to use, either for debugging or gameplay.
        default:
            break;
    }
}

/*
 * helper function to calculate a part of the alpha value
 * Source:
 * Assignment1
 * File: trirast.c
 * Made by: Tom Peerdeman & Rene Aparicio Saez
 * https://github.com/TomPeerdeman/GEGT2013/blob/master/assignment1/triangle_rasterization/trirast.c
 */
float alphacalc(float x, float x1, float x2, float y, float y1, float y2) {
	return((y1-y2)*x + (x2-x1)*y + x1*y2 - x2*y1);
}

/*
 * helper function to calculate a part of the beta value
 * Source:
 * Assignment1
 * File: trirast.c
 * Made by: Tom Peerdeman & Rene Aparicio Saez
 * https://github.com/TomPeerdeman/GEGT2013/blob/master/assignment1/triangle_rasterization/trirast.c
 */
float betacalc(float x, float x0, float x2, float y, float y0, float y2) {
	return((y2-y0)*x + (x0-x2)*y + x2*y0 - x0*y2);
}

/*
 * helper function to calculate a part of the gamma value
 * Source:
 * Assignment1
 * File: trirast.c
 * Made by: Tom Peerdeman & Rene Aparicio Saez
 * https://github.com/TomPeerdeman/GEGT2013/blob/master/assignment1/triangle_rasterization/trirast.c
 */
float gammacalc(float x, float x0, float x1, float y, float y0, float y1) {
	return((y0-y1)*x + (x1-x0)*y + x0*y1 - x1*y0);
}

/*
 * Called to check for concave points in the given polygon, if so return false
 */
bool bary_check(){
	b2Vec2 p;
	b2Vec2 p1;
	b2Vec2 p2;
	b2Vec2 p3;

	float alpha, beta, gamma;
	for(int i = 0; i < 4; i++){
	  /*alpha = alphacalc(worldvert_x[(i+3)%4], worldvert_x[(i+1)%4], worldvert_x[(i+2)%4],
	  		worldvert_y[(i+3)%4], worldvert_y[(i+1)%4], worldvert_y[(i+2)%4]);
	  beta = betacalc(worldvert_x[(i+3)%4], worldvert_x[i], worldvert_x[(i+2)%4],
	  		worldvert_y[(i+3)%4], worldvert_y[i], worldvert_y[(i+2)%4]);
	  gamma = gammacalc(worldvert_x[(i+3)%4], worldvert_x[i], worldvert_x[(i+1)%4],
	  		worldvert_y[(i+3)%4], worldvert_y[i], worldvert_y[(i+1)%4]);
	  if(alpha+beta+gamma <= 1.0f && alpha+beta+gamma >= 0.0f)
	  	return false;*/

		p.Set(worldvert_x[i], worldvert_y[i]);
		p1.Set(worldvert_x[(i + 1) % 4], worldvert_y[(i + 1) % 4]);
		p2.Set(worldvert_x[(i + 2) % 4], worldvert_y[(i + 2) % 4]);
		p3.Set(worldvert_x[(i + 3) % 4], worldvert_y[(i + 3) % 4]);		

		/*beta = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) /
		       ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
		gamma = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) /
		        ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
		alpha = 1.0f - beta - gamma;*/

		float f12 = alphacalc(p1.x, p2.x, p3.x, p1.y, p2.y, p3.y);
		float f20 = betacalc(p2.x, p1.x, p3.x, p2.y, p1.y, p3.y);
		float f01 = gammacalc(p3.x, p1.x, p2.x, p3.y, p1.y, p2.y);

		alpha = alphacalc(p.x, p2.x, p3.x ,p.y , p2.y, p3.y) / f12;
		beta = betacalc(p.x ,p1.x ,p3.x ,p.y ,p1.y ,p3.y) / f20;
		gamma = gammacalc(p.x, p1.x, p2.x, p.y, p1.y, p2.y) / f01;

		if(!(alpha >= 1.0f || alpha <= 0.0f || beta >= 1.0f || beta <= 0.0f || gamma >= 1.0f || gamma <= 0.0f)) {
			return false;
		}	
	}

	return true;
}

/*
 * Check if two given lines intersect
 * Original source-code: http://flassari.is/2008/11/line-line-intersection-in-cplusplus/
 * This is a substracted version of the original
 */
bool intersection(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y) {
	// Store the values for fast access and easy
	// equations-to-code conversion
	int x1 = p1x, x2 = p2x, x3 = p3x, x4 = p4x;
	int y1 = p1y, y2 = p2y, y3 = p3y, y4 = p4y;
	 
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return NULL;
	 
	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
	 
	// Check if the x and y coordinates are within both lines
	if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
		x < std::min(x3, x4) || x > std::max(x3, x4) ) return false;
	if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
		y < std::min(y3, y4) || y > std::max(y3, y4) ) return false;

	return true;
}

/*
 * Called when the user clicked (or released) a mouse buttons inside the window.
 */
void mouse_clicked(int button, int state, int x, int y)
{
	// only work if left mousebutton pressed
	if(button == 0){
		// only add point when the mouse is pressed
		if(!state && !winObject.hasWon() && !winObject.hasLost() && dpolylist_length < possible_totalpoly){
			// pixel values
			mousevert_x[mousecounter] = x;
			mousevert_y[mousecounter] = y;
	
			// box2D values
			worldvert_x[mousecounter] = (float)mousevert_x[mousecounter]/100.0f;
			worldvert_y[mousecounter] = 6.0f-(float)mousevert_y[mousecounter]/100.0f;
		
			// check for concave points	
			if((mousecounter == 3 && bary_check()) || mousecounter != 3){			
				mousecounter++;
			}
		}
	
		// if mouse released draw if there are 4 vertices
		if(state){
			if(winObject.hasWon()) {
				load_world(current_level + 1);
				mousecounter = 0;
				return;
			}
			
			if(winObject.hasLost()) {
				load_world(current_level);
				mousecounter = 0;
				return;
			}
			
			if(mousecounter == 4 && dpolylist_length < possible_totalpoly){
				// fill a poly_t object
				poly_t poly1;
				poly1.num_verts = 4;
				poly1.verts = new point_t[4];

				bool intersect = intersection(
				mousevert_x[0], mousevert_y[0], 
				mousevert_x[3], mousevert_y[3], 
				mousevert_x[1], mousevert_y[1], 
				mousevert_x[2], mousevert_y[2]);
				if(intersect){
					float buf_x = worldvert_x[2];
					float buf_y = worldvert_y[2];
					worldvert_x[2] = worldvert_x[3];
					worldvert_y[2] = worldvert_y[3];
					worldvert_x[3] = buf_x;
					worldvert_y[3] = buf_y;
				}
				else{
					intersect = intersection(
					mousevert_x[0], mousevert_y[0], 
					mousevert_x[1], mousevert_y[1], 
					mousevert_x[3], mousevert_y[3], 
					mousevert_x[2], mousevert_y[2]);
					if(intersect){
						float buf_x = worldvert_x[1];
						float buf_y = worldvert_y[1];
						worldvert_x[1] = worldvert_x[2];
						worldvert_y[1] = worldvert_y[2];
						worldvert_x[2] = buf_x;
						worldvert_y[2] = buf_y;
					}
				}
				
				float sum = 0.0;
				for(int i = 0; i < 4; i++){
					sum = sum + ((worldvert_x[(i+1)%4]-worldvert_x[i]) * (worldvert_y[(i+1)%4]+worldvert_y[i]));
				}
				
				if(sum < 0){
					for(int i = 0; i < 4; i++){
						poly1.verts[i].x = worldvert_x[i];
						poly1.verts[i].y = worldvert_y[i];
					}
				}
				else{
					for(int i = 0; i < 4; i++){
						poly1.verts[i].x = worldvert_x[3-i];
						poly1.verts[i].y = worldvert_y[3-i];
					}
				}
        
				dpolygons[dpolylist_length++] = new Polygon(world, &poly1, 1, 1.0f);
				
				delete poly1.verts;
				
				mousecounter = 0;
			} else if(dpolylist_length >= 10) {
				mousecounter = 0;
			}
		}
	} else {
		if(mousecounter > 0) {
			mousecounter = 0;
		}
		/*printf("Mouse 2\n");
		for(int i = 0; i < dpolylist_length; i++){
			printf("Intersect %d %d\n", i, dpolygons[i]->intersects(x / 100.0f, 6.0f - (y / 100.0f)));
		}*/
	}
}

/*
 * Called when the mouse is moved to a certain given position.
 */
void mouse_moved(int x, int y) {
	world_mouse_x = (float) x / 100.0f;
	world_mouse_y = 6.0f-(float) y / 100.0f;
}


int main(int argc, char **argv)
{
    // Create an OpenGL context and a GLUT window.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(reso_x, reso_y);
    glutCreateWindow("Box2D");

    // Bind all GLUT events do callback function.
    glutDisplayFunc(&draw);
    glutIdleFunc(&draw);
    glutReshapeFunc(&resize_window);
    glutKeyboardFunc(&key_pressed);
    glutMouseFunc(&mouse_clicked);
    glutMotionFunc(&mouse_moved);
    glutPassiveMotionFunc(&mouse_moved);

    // Initialise the matrices so we have an orthogonal world with the same size
    // as the levels, and no other transformations.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, world_x, 0, world_y, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Read the levels into a bunch of structs.
    num_levels = load_levels(&levels);

    // Load the first level (i.e. create all Box2D stuff).
    load_world(0);

    last_time = glutGet(GLUT_ELAPSED_TIME);
    frame_count = 0;
    glutMainLoop();

    return 0;
}
