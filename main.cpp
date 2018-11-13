#include <iostream>
#include "src/utils.h"
#include "src/game_map.h"


game_map * gameMap;

void initGL(){
    glClearColor(1.0f,1.0f,1.0f,1.0f);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    // Define shapes enclosed within a pair of glBegin and glEnd
    gameMap->draw();

    glFlush();  // Render now
}

int main(int argc, char** argv) {
    game_map game_map1 = game_map(30,100);
    gameMap = &game_map1;
    glutInit(&argc, argv);          // Initialize GLUT
    glutCreateWindow("Vertex, Primitive & Color");  // Create window with the given title
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(1);
    glutInitWindowSize(200, 200);   // Set the window's initial width & height
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the event-processing loop
    return 0;
}