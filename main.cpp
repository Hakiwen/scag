#include <iostream>
#include "src/utils.h"
#include "src/game_map.h"




void initGL(){
    glClearColor(1.0f,1.0f,1.0f,1.0f);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
//	game_map game_map1 = game_map(12, 27*27);
//	emmanuel's map
	square_obstacle ob1(point(0.2,0.9),point(0.45,0.9), point(0.2,0.7),point(0.45,0.7));
	square_obstacle ob2(point(0.1,0.3),point(0.25,0.3), point(0.1,0.15),point(0.25,0.15));
	square_obstacle ob3(point(0.24,0.49),point(0.45,0.49), point(0.24,0.4),point(0.45,0.4));
	square_obstacle ob4(point(0.35,0.68),point(0.49,0.68), point(0.35,0.51),point(0.49,0.51));
	square_obstacle ob5(point(0.65,0.55),point(0.8,0.55), point(0.65,0.45),point(0.8,0.45));
	square_obstacle ob6(point(0.66,0.3),point(0.77,0.3), point(0.66,0.15),point(0.77,0.15));
	square_obstacle ob7(point(0.51,0.13),point(0.65,0.13), point(0.51, -0.01),point(0.65,-0.01));
	square_obstacle ob8(point(0.85,1.01),point(0.95,1.01), point(0.85,0.85),point(0.95,0.85));
	square_obstacle ob9(point(0.85,0.51),point(1,0.51), point(0.85,0.33),point(1,0.33));
	game_map game_map1(729, {ob1, ob2, ob3, ob4, ob5, ob6, ob7, ob8, ob9});

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


		for(;;) {
			glutMainLoopEvent();                 // Enter the event-processing loop

			std::string dont_use;
			std::cin >> dont_use;
		}
}