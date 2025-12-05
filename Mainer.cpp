#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "Shape.h"

using namespace std;

Shape shp;

int windowWidth = 800;
int windowHeight = 600;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(10, 10, 10,  
              0, 0, 0,     
              0, 1, 0);   
    

    shp.drawShape(0, 0, 0);
    
    glutSwapBuffers();
}


void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat lightPos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Tetris 3D - Day 2: Basic Shape");
    
    shp.createShape();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    initGL();
    
    glutMainLoop();
    
    return 0;
}
