#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "Shape.h"

using namespace std;

Shape::Shape() {
    size = 0;
    color = 1;
}

Shape::~Shape() {
 
}


void Shape::createShape() {
    size = 0;
    

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                enableCubes[i][j][k] = 0;
            }
        }
    }
    
    enableCubes[1][1][1] = 1;
    size = 1;
    color = 1;
    
    cout << "Shape created with " << size << " cubes" << endl;
}


void Shape::drawShape(float x, float y, float z) {

    switch(color) {
        case 1:
            glColor3f(1.0f, 1.0f, 0.0f); // Желтый
            break;
        case 2:
            glColor3f(0.0f, 0.0f, 1.0f); // Синий
            break;
        case 3:
            glColor3f(1.0f, 0.0f, 0.0f); // Красный
            break;
        default:
            glColor3f(1.0f, 1.0f, 1.0f); // Белый
    }
    

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (enableCubes[i][j][k] == 1) {
                    glPushMatrix();
                    glTranslatef(x + i - 1.5f, y + j - 1.5f, z + k - 1.5f);
                    glutSolidCube(0.9f);
                    glPopMatrix();
                }
            }
        }
    }
}
