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
Polygon **dpolygons = new Polygon *[10];

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

WinObject winObject;

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
		delete ball;
		delete ground;
		
		for(int i = 0; i < spolylist_length; i++) {
			delete spolygons[i];
		}
		delete[] spolygons;
		
		for(int i = 0; i < dpolylist_length; i++) {
			delete dpolygons[i];
		}
		delete[] dpolygons;
	}
	
	current_level = level;
	winObject.reset();

    // Create a Box2D world and populate it with all bodies for this level
    // (including the ball).
	
	b2Vec2 gravity(0.0f, -10.0f);
	
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
	}
	last_time = time;

    frame_count++;

    // Clear the buffer
    glColor3f(0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
	
	if(world != NULL) {
		if(!winObject.hasWon() && !winObject.hasLost() && 0) {
			// Simulate the world
			world->Step((frametime / 1000.0f), 8, 3);
		}

		// Draw ball
		glColor3f(1.0f, 0.0f, 0.0f);
		ball->render();
		
		glColor3f(1.0f, 1.0f, 1.0f);
		endPoint->render();
		
		// Draw static polygons
		glColor3f(0.0f, 1.0f, 0.0f);
		for(int i = 0; i < spolylist_length; i++) {
			spolygons[i]->render();
		}
		
		// Draw dynamic polygons
		glColor3f(0.5f, 0.5f, 0.5f);
		for(int i = 0; i < dpolylist_length; i++) {
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
	
    // Show rendered frame
    glutSwapBuffers();

    // Display fps in window title.
    if (time - timebase >= 1000) {
        char window_title[128];
        snprintf(window_title, 128,
                "Box2D: %3.2f fps, level %d/%d",
                frame_count / ((time - timebase) / 1000.f), 
				(current_level + 1), num_levels);
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
        case 27: // Esc
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
			printf("New level: %u\n", (current_level + 1));
			break;
        // Add any keys you want to use, either for debugging or gameplay.
        default:
			printf("Pressed: %u\n", key);
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
	float alpha, beta, gamma;
	for(int i = 0; i < 4; i++){
	  alpha = alphacalc(mousevert_x[(i+3)%4], mousevert_x[(i+1)%4], mousevert_x[(i+2)%4],
	  		mousevert_y[(i+3)%4], mousevert_y[(i+1)%4], mousevert_y[(i+2)%4]);
	 // beta = betacalc(world_x[(i+3)%4], world_x[(i)%4], world_x[(i+2)%4],
	  //		world_y[(i+3)%4], world_y[(i+1)%4], world_y[(i+2)%4]);
	}

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
		if(!state && !winObject.hasWon() && !winObject.hasLost()){
			bool allowed = false;
			
			// overwrite old values
			mousecounter = mousecounter % 4;
			
			// pixel values
			mousevert_x[mousecounter] = x;
			mousevert_y[mousecounter] = y;
	
			// box2D values
			worldvert_x[mousecounter] = (float)mousevert_x[mousecounter]/100.0f;
			worldvert_y[mousecounter] = 6.0f-(float)mousevert_y[mousecounter]/100.0f;
		
			// check for concave points
			if(mousecounter == 3){
				allowed = bary_check();
			}
			
			if(allowed){			
				mousecounter++;
			}
			
			// print new values
			for(int i = 0; i < 4; i++){
				printf("mouse\tx: %u, y: %u\n",mousevert_x[i],mousevert_y[i]);
			}
			for(int i = 0; i < 4; i++){
				printf("world\tx: %g, y: %g\n",worldvert_x[i],worldvert_y[i]);
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
			
			if(mousecounter == 4 && dpolylist_length < 10){
				// fill a poly_t object
				poly_t poly1;
				poly1.num_verts = 4;
				poly1.verts = new point_t[4];
				
				/* possible intersection 1
				 *    3
				 *  2   1
				 *    0
				 */
				if(worldvert_x[1] > worldvert_x[0] &&
					 worldvert_x[1] > worldvert_x[2] &&
					 worldvert_x[2] < worldvert_x[3]){
					float buf_x = worldvert_x[3];
					float buf_y = worldvert_y[3];
					worldvert_x[3] = worldvert_x[2];
					worldvert_y[3] = worldvert_y[2];
					worldvert_x[2] = buf_x;
					worldvert_y[2] = buf_y;
				}
				/* possible intersection 2
				 *    2
				 *  0   3
				 *    1
				 */
				if(worldvert_y[1] < worldvert_y[0] &&
					 worldvert_y[1] < worldvert_y[2] &&
					 worldvert_y[2] > worldvert_y[3]){
					float buf_x = worldvert_x[3];
					float buf_y = worldvert_y[3];
					worldvert_x[3] = worldvert_x[2];
					worldvert_y[3] = worldvert_y[2];
					worldvert_x[2] = buf_x;
					worldvert_y[2] = buf_y;
				}
				/* possible intersection 3
				 *    3
				 *  1   2
				 *    0
				 */
				if(worldvert_x[1] < worldvert_x[0] &&
					 worldvert_x[1] < worldvert_x[2] &&
					 worldvert_x[2] > worldvert_x[3]){
					float buf_x = worldvert_x[3];
					float buf_y = worldvert_y[3];
					worldvert_x[3] = worldvert_x[2];
					worldvert_y[3] = worldvert_y[2];
					worldvert_x[2] = buf_x;
					worldvert_y[2] = buf_y;
				}
				/* possible intersection 4
				 *    1
				 *  3   0
				 *    2
				 */
				if(worldvert_y[1] > worldvert_y[0] &&
					 worldvert_y[1] > worldvert_y[2] &&
					 worldvert_y[2] < worldvert_y[3]){
					float buf_x = worldvert_x[3];
					float buf_y = worldvert_y[3];
					worldvert_x[3] = worldvert_x[2];
					worldvert_y[3] = worldvert_y[2];
					worldvert_x[2] = buf_x;
					worldvert_y[2] = buf_y;
				}
				
				int sum = 0;
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
    printf("Loaded %d levels.\n", num_levels);

    // Load the first level (i.e. create all Box2D stuff).
    load_world(0);

    last_time = glutGet(GLUT_ELAPSED_TIME);
    frame_count = 0;
    glutMainLoop();

    return 0;
}
