#include <iostream>
#include "src/utils.h"
#include "src/game_map.h"




void initGL(){
    glClearColor(1.0f,1.0f,1.0f,1.0f);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
	game_map game_map1 = game_map(30, 100);


    // Define shapes enclosed within a pair of glBegin and glEnd
   	game_map1.draw();

    glFlush();  // Render now

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);          // Initialize GLUT
		glutCreateWindow("Vertex, Primitive & Color");  // Create window with the given title
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(1);
		glutInitWindowSize(1920, 1080);   // Set the window's initial width & height
		glutDisplayFunc(display);       // Register callback handler for window re-paint event
		initGL();



		glutMainLoop();                 // Enter the event-processing loop



}