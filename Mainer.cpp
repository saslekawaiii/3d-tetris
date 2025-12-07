#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "Shape.h"

using namespace std;

Shape shp;
int windowWidth = 800;
int windowHeight = 600;

int cameraRotation = 30;
int cameraTilt = 20;
int shapeRotation = 0;
float shapeX = 0, shapeY = 0, shapeZ = 0;

bool autoRotate = true;
bool showAxes = true;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(8, 6, 8,   
              0, 0, 0,   
              0, 1, 0);  
    
    glRotatef(cameraRotation, 0, 1, 0);
    glRotatef(cameraTilt, 1, 0, 0);
    
    if (showAxes) {
        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(-5, 0, 0);
        glVertex3f(5, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, -5, 0);
        glVertex3f(0, 5, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, -5);
        glVertex3f(0, 0, 5);
        glEnd();
    }
    
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for (int i = -5; i <= 5; i++) {
        glVertex3f(i, -2, -5);
        glVertex3f(i, -2, 5);
        glVertex3f(-5, -2, i);
        glVertex3f(5, -2, i);
    }
    glEnd();
    
    shp.drawShape(15, shapeX, shapeY, shapeZ, 
                  shapeRotation, 0, 0);
    
    glutSwapBuffers();
}

void timerFunc(int value) {
    static int frameCount = 0;
    frameCount++;
    
    if (autoRotate) {
        shapeRotation = (shapeRotation + 1) % 360;
        
        shapeX = sin(frameCount * 0.02f) * 3.0f;
        shapeZ = cos(frameCount * 0.02f) * 3.0f;
        
        shapeY = sin(frameCount * 0.03f) * 0.5f;
        
        if (frameCount % 200 == 0) {
            shp.initShape();
            cout << "Авто-генерация: создана новая фигура (" 
                 << shp.size << " кубов, цвет " << shp.color << ")" << endl;
        }
        
        if (frameCount % 400 == 0) {
            cameraRotation = (cameraRotation + 45) % 360;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timerFunc, 0);  // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case ' ': 
            shp.initShape();
            cout << "Создана случайная фигура:" << endl;
            cout << "  Количество кубов: " << shp.size << endl;
            cout << "  Цвет: " << shp.color;
            if (shp.color == 1) cout << " (желтый)" << endl;
            else if (shp.color == 2) cout << " (синий)" << endl;
            else cout << " (красный)" << endl;
            break;
            
        case 'x':
            shp.rotateShape_x();
            cout << "Поворот вокруг оси X" << endl;
            break;
            
        case 'y':
            shp.rotateShape_y();
            cout << "Поворот вокруг оси Y" << endl;
            break;
            
        case 'z': 
            shp.rotateShape_z();
            cout << "Поворот вокруг оси Z" << endl;
            break;
            
        case 'r': 
            autoRotate = !autoRotate;
            cout << "Авто-режим: " << (autoRotate ? "ВКЛ" : "ВЫКЛ") << endl;
            break;
            
        case 'a': 
            showAxes = !showAxes;
            cout << "Координатные оси: " << (showAxes ? "ПОКАЗЫВАТЬ" : "СКРЫТЬ") << endl;
            break;
            
        case '1': 
            shp.createShape();
            shp.enableCubes[1][1][1] = 1;
            shp.size = 1;
            shp.color = 1;
            cout << "Создана простая фигура (1 куб)" << endl;
            break;
            
        case '2': 
            shp.createShape();
            for (int i = 0; i < 3; i++) {
                shp.enableCubes[i][1][1] = 1;
            }
            shp.size = 3;
            shp.color = 2;
            cout << "Создана горизонтальная линия (3 куба)" << endl;
            break;
            
        case '3':  
            shp.createShape();
            shp.enableCubes[1][1][1] = 1;
            shp.enableCubes[0][1][1] = 1;
            shp.enableCubes[2][1][1] = 1;
            shp.enableCubes[1][0][1] = 1;
            shp.enableCubes[1][2][1] = 1;
            shp.size = 5;
            shp.color = 3;
            cout << "Создан крест (5 кубов)" << endl;
            break;
            
        case 'w': cameraTilt -= 5; break;
        case 's': cameraTilt += 5; break;
        case 'a': cameraRotation -= 5; break;
        case 'd': cameraRotation += 5; break;
            
        case 27: 
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}


void initGL() {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    

    GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
    GLfloat lightColor[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    

    GLfloat matSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat matShininess[] = {30.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
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
    
    shp.initShape();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timerFunc, 0);
    
    initGL();
    
    glutMainLoop();
    
    return 0;
}
