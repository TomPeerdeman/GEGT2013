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

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
level_t *levels;

unsigned int current_level;

b2World *world;
Ball *ball;

/*
 * Load a given world, i.e. read the world from the `levels' data structure and
 * convert it into a Box2D world.
 */
void load_world(unsigned int level)
{
    if (level >= num_levels)
    {
        // Note that level is unsigned but we still use %d so -1 is shown as
        // such.
        printf("Warning: level %d does not exist.\n", level);
        return;
    }
	
	// Unload previous world
	if(world != NULL){
		delete world;
		delete ball;
	}

    // Create a Box2D world and populate it with all bodies for this level
    // (including the ball).
	
	b2Vec2 gravity(0.0f, -10.0f);
	
	// Create the world
	world = new b2World(gravity);
	
	ball = new Ball(world, &levels[level], 0.3f, 1.0f);
	
	// making static object poylgons
  b2PolygonShape polygons[&levels[level].num_polygons];
	for(int i = 0; i < &levels[level].num_polygons; i++){
	  b2Vec2 vertices[&levels[level].polygons[i].num_verts];
	  for(int j = 0; j < &levels[level].polygons[i].num_verts; j++){
  	  vertices[j].Set(&levels[level].polygons[i].verts[j].x,
  	                  &levels[level].polygons[i].verts[j].y);
	  }
	  polygons.Set(vertices, &levels[level].polygons[i].num_verts);
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


    //
    // Do any logic and drawing here.
    //
	
	// Simulate the world
	world->Step((frametime / 1000.0f), 8, 3);

	glColor3f(1.0f, 0.0f, 0.0f);
	ball->render();
	
    // Show rendered frame
    glutSwapBuffers();

    // Display fps in window title.
    if (time - timebase >= 1000)
    {
        char window_title[128];
        snprintf(window_title, 128,
                "Box2D: %3.2f fps, level %d/%d",
                frame_count / ((time - timebase) / 1000.f), (current_level + 1), num_levels);
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
 * Called when the user clicked (or released) a mouse buttons inside the window.
 */
void mouse_clicked(int button, int state, int x, int y)
{

}

/*
 * Called when the mouse is moved to a certain given position.
 */
void mouse_moved(int x, int y)
{

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
