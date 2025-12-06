#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <cstdlib>
#include <ctime>
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
}


void Shape::initShape() {
    createShape();  
    srand((unsigned)time(0));
    
    bool hasNeighbor = false;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                // 50% шанс создать куб
                if (rand() % 100 > 50) {
                    enableCubes[i][j][k] = 1;
                    size++;
                    
                    if (size > 1) {
                        hasNeighbor = false;
                        
                        if (i > 0 && enableCubes[i-1][j][k] == 1) hasNeighbor = true;
                        if (i < 2 && enableCubes[i+1][j][k] == 1) hasNeighbor = true;
                        if (j > 0 && enableCubes[i][j-1][k] == 1) hasNeighbor = true;
                        if (j < 2 && enableCubes[i][j+1][k] == 1) hasNeighbor = true;
                        if (k > 0 && enableCubes[i][j][k-1] == 1) hasNeighbor = true;
                        if (k < 2 && enableCubes[i][j][k+1] == 1) hasNeighbor = true;
                        
                        if (!hasNeighbor) {
                            enableCubes[i][j][k] = 0;
                            size--;
                        }
                    }
                }
            }
        }
    }
    
    if (size == 0) {
        enableCubes[1][1][1] = 1;
        size = 1;
    }
    
    if (size < 4) {
        color = 1;  
    } else if (size < 7) {
        color = 2;  
    } else {
        color = 3;  
    }
    
}

void Shape::drawShape(int d, float x, float y, float z, 
                     int leftRight_rotation, int upDown_x, int upDown_z) {
    
    switch(color) {
        case 1: 
            glColor3f(1.0f, 1.0f, 0.0f);
            break;
        case 2: 
            glColor3f(0.0f, 0.0f, 1.0f);
            break;
        case 3: 
            glColor3f(1.0f, 0.0f, 0.0f);
            break;
        default:
            glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (enableCubes[i][j][k] == 1) {
                    glPushMatrix();
                    
                    glTranslatef(0, 0, 0);
                    glRotatef(leftRight_rotation, 0, 1, 0);
                    glRotatef(upDown_x, 1, 0, 0);
                    glRotatef(upDown_z, 0, 0, 1);
                    
                    glTranslatef(i + x, y - j, k + z);
                    glutSolidCube(0.95f);
                    
                    glPopMatrix();
                }
            }
        }
    }
}

void Shape::rotateShape_x() {
    int temp[3][3][3];
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                temp[i][j][k] = enableCubes[i][2-k][j];
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                enableCubes[i][j][k] = temp[i][j][k];
            }
        }
    }
    
}

void Shape::rotateShape_y() {
    int temp[3][3][3];
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                temp[i][j][k] = enableCubes[2-k][j][i];
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                enableCubes[i][j][k] = temp[i][j][k];
            }
        }
    }
    
}

void Shape::rotateShape_z() {
    int temp[3][3][3];
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                temp[i][j][k] = enableCubes[j][2-i][k];
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                enableCubes[i][j][k] = temp[i][j][k];
            }
        }
    }
    
}
