#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <string>
#include "Shape.h"

#define KEY_ESCAPE 27
#define N 15.00

using namespace std;

typedef struct {
    int width;
    int height;
    const char* title;
    float field_of_view_angle;
    float z_near;
    float z_far;
} glutWindow;

glutWindow win;

bool fullscreen = false;

Shape shp;

int d = (int)N;

int upDown_x = 0;
int upDown_z = 0;
int leftRight_rotation = 45;
int r_speed = 7;
float down_speed = 0.02f;
int zoom = 40;
float down = d / 2 + 2;
float movex = -1;
float movez = -1;

int score = 0;
bool gameOver = false;

bool stopFlag = true;
int base[15][15][15];

void *font = GLUT_BITMAP_HELVETICA_18;

int viewMode = 0; // 0 - сбоку, 1 - сверху

void destroyer() {
    int count = 0;
    int o = 0;
    for (int k = 0; k < 15; k++)
    {
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (base[i][k][j] != 0)
                    count++;
            }
        }
        if (count == 225)
        {
            o++;
            score++;
            if (k > 0) k--;
        }
        if (o > 0)
        {
            for (int l = 0; l < 15; l++)
            {
                for (int m = 0; m < 15; m++)
                {
                    if (k + o < 15)
                        base[l][k][m] = base[l][k + o][m];
                    else
                        base[l][k][m] = 0;
                }
            }
        }
        count = 0;
    }
}

void checkSides(int undox, int undoz) {
    int i, j, k;
    bool draw = false;
    if (down > -d / 2)
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] != 0)
                    {
                        draw = true;
                        break;
                    }
                    else if ((shp.enableCubes[i][k][j] == 1 && movex + i > 7) || (shp.enableCubes[i][k][j] == 1 && movez + j > 7))
                    {
                        draw = true;
                        break;
                    }
                    else if (shp.enableCubes[i][k][j] == 1 && movex + i < -7)
                    {
                        draw = true;
                        break;
                    }
                    else if (shp.enableCubes[i][k][2 - j] == 1 && movez + 2 - j < -7)
                    {
                        draw = true;
                        break;
                    }
                }
                if (draw) break;
            }
            if (draw) break;
        }
        if (draw)
        {
            movex = undox;
            movez = undoz;
        }
    }
}

void putInBase() {
    int i, j, k;
    bool draw = false;
    bool redShape = false;
    bool blueShape = false;
    if (down > -d / 2)
    {
        for (k = 2; k >= 0; k--)
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    if (shp.color == 1)
                    {
                        if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - 1 - k][(int)movez + d / 2 + j] != 0)
                        {
                            draw = true;
                            break;
                        }
                    }
                    else if (shp.color == 3)
                    {
                        if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] != 0)
                        {
                            base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = 0;
                            score++;
                        }
                    }
                    else
                    {
                        if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - 1 - k][(int)movez + d / 2 + j] != 0
                            && base[(int)movex + d / 2 + i][(int)down + d / 2 - 2 - k][(int)movez + d / 2 + j] != 0
                            && base[(int)movex + d / 2 + i][(int)down + d / 2 - 3 - k][(int)movez + d / 2 + j] != 0)
                        {
                            blueShape = true;
                            draw = true;
                            break;
                        }
                        else
                        {
                            if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] != 0)
                            {
                                base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = 0;
                                score++;
                            }
                        }
                    }
                }
                if (draw) break;
            }
            if (draw) break;
        }
        if (draw)
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    for (k = 0; k < 3; k++)
                    {
                        if (shp.color == 1)
                        {
                            if (shp.enableCubes[i][k][j] == 1)
                                base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = shp.color;
                        }
                        else if (shp.color == 2 && blueShape)
                        {
                            if (shp.enableCubes[i][k][j] == 1)
                                base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = shp.color;
                        }
                        else if (shp.color == 3)
                        {
                            redShape = true;
                        }
                    }
                }
            }
        }
        if (draw && !redShape)
        {
            destroyer();
            if (down < d / 2 - 1)
            {
                shp.createShape();
                shp.initShape();
            }
            else
            {
                gameOver = true;
            }
            down = d / 2;
            movex = -1;
            movez = -1;
        }
    }
    if (down < 2 - d / 2)
    {
        for (k = 2; k >= 0; k--)
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    if ((shp.enableCubes[i][k][j] == 1 && (int)down + d / 2 - k == 0) || (draw && shp.enableCubes[i][k][j] == 1))
                    {
                        base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = shp.color;
                        draw = true;
                    }
                }
            }
        }
        if (draw)
        {
            destroyer();
            shp.createShape();
            shp.initShape();
            down = d / 2;
            movex = -1;
            movez = -1;
        }
    }
}

void drawBase() {
    for (int j = 0; j < 15; j++)
    {
        for (int k = 0; k < 15; k++)
        {
            for (int l = 0; l < 15; l++)
            {
                if (base[j][k][l] != 0)
                {
                    glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glRotatef(leftRight_rotation, 0, 1, 0);
                    glRotatef(upDown_x, 1, 0, 0);
                    glRotatef(upDown_z, 0, 0, 1);
                    if (base[j][k][l] == 1)
                        glColor3f(1, 1, 0);
                    else if (base[j][k][l] == 2)
                        glColor3f(0, 0, 1);
                    else
                        glColor3f(1, 0, 0);
                    glTranslatef(j - d / 2, k - d / 2, l - d / 2);
                    glutWireCube(1);
                    glutSolidCube(0.99);
                    glPopMatrix();
                }
            }
        }
    }
}

void printTxt(float x, float y, const char *String) {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 10, 0, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.9f, 0.7f, 0.4f);
    glRasterPos2f(x, y);
    glDisable(GL_LIGHTING);
    for (const char* c = String; *c != '\0'; ++c)
        glutBitmapCharacter(font, *c);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void display() {
    float i;
    string scr = "S C O R E : ";
    scr += to_string(score);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    printTxt(1.0f, 1.0f, scr.c_str());
    glLoadIdentity();

    if (viewMode == 0)
        gluLookAt((double)zoom, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    else if (viewMode == 1)
        gluLookAt(0.0, (double)zoom, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

    glPushMatrix();
    glColor3f(0, 1, 1);
    glTranslatef(0, 0, 0);
    glRotatef(leftRight_rotation, 0, 1, 0);
    glRotatef(upDown_x, 1, 0, 0);
    glRotatef(upDown_z, 0, 0, 1);
    glColor3f(0.7f, 0.6f, 0.9f);
    for (i = -N / 2; i <= N / 2; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(i, N / 2, -N / 2);
        glVertex3f(i, -N / 2, -N / 2);
        glEnd();
    }
    for (i = -N / 2; i <= N / 2; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(N / 2, i, -N / 2);
        glVertex3f(-N / 2, i, -N / 2);
        glEnd();
    }
    for (i = -N / 2; i <= N / 2; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-N / 2, N / 2, i);
        glVertex3f(-N / 2, -N / 2, i);
        glEnd();
    }
    for (i = -N / 2; i <= N / 2; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-N / 2, i, N / 2);
        glVertex3f(-N / 2, i, -N / 2);
        glEnd();
    }
    glColor3f(0.9f, 0.9f, 0.6f);
    for (i = -N / 2; i <= N / 2; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(i, -N / 2, -N / 2);
        glVertex3f(i, -N / 2, N / 2);
        glEnd();
    }
    for (i = -N / 2; i <= N / 2; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(N / 2, -N / 2, i);
        glVertex3f(-N / 2, -N / 2, i);
        glEnd();
    }
    glPopMatrix();
    if (!gameOver)
    {
        shp.drawShape(d, movex, down, movez, leftRight_rotation, upDown_x, upDown_z);
        putInBase();
    }
    else
    {
        printTxt(4.0f, 5.0f, "G A M E  O V E R");
    }
    drawBase();
    if (!stopFlag)
    {
        down -= down_speed;
    }
    glutSwapBuffers();
    glFlush();
}

void normal_keys(unsigned char key, int x, int y) {
    if (key == KEY_ESCAPE) exit(0);
}

void special_keys(int keys, int x, int y) {
    int undox = (int)movex;
    int undoz = (int)movez;

    if (!stopFlag) {
        if (viewMode == 0) { 
            int moires = leftRight_rotation < 0 ? leftRight_rotation % 360 + 360 : leftRight_rotation % 360;

            switch (keys) {
                case GLUT_KEY_LEFT:
                    if (moires <= 45 || moires > 315) { if (movez < d / 2) movez++; }
                    else if (moires > 135 && moires <= 225) { if (movez > -d / 2 - 2) movez--; }
                    else if (moires > 45 && moires <= 135) { if (movex > -d / 2 - 2) movex--; }
                    else if (moires > 225 && moires <= 315) { if (movex < d / 2) movex++; }
                    break;
                case GLUT_KEY_RIGHT:
                    if (moires <= 45 || moires > 315) { if (movez > -d / 2 - 2) movez--; }
                    else if (moires > 135 && moires <= 225) { if (movez < d / 2) movez++; }
                    else if (moires > 45 && moires <= 135) { if (movex < d / 2) movex++; }
                    else if (moires > 225 && moires <= 315) { if (movex > -d / 2 - 2) movex--; }
                    break;
                case GLUT_KEY_UP:
                    if (moires <= 45 || moires > 315) { if (movex > -d / 2 - 2) movex--; }
                    else if (moires > 135 && moires <= 225) { if (movex < d / 2) movex++; }
                    else if (moires > 45 && moires <= 135) { if (movez > -d / 2 - 2) movez--; }
                    else if (moires > 225 && moires <= 315) { if (movez < d / 2) movez++; }
                    break;
                case GLUT_KEY_DOWN:
                    if (moires <= 45 || moires > 315) { if (movex < d / 2) movex++; }
                    else if (moires > 135 && moires <= 225) { if (movex > -d / 2 - 2) movex--; }
                    else if (moires > 45 && moires <= 135) { if (movez < d / 2) movez++; }
                    else if (moires > 225 && moires <= 315) { if (movez > -d / 2 - 2) movez--; }
                    break;
            }
        } else if (viewMode == 1) {
            switch (keys) {
                case GLUT_KEY_LEFT:  if (movex > -d / 2) movex--; break;
                case GLUT_KEY_RIGHT: if (movex < d / 2) movex++; break;
                case GLUT_KEY_UP:    if (movez > -d / 2) movez--; break;
                case GLUT_KEY_DOWN:  if (movez < d / 2) movez++; break;
            }
        }
        checkSides(undox, undoz);
    }
}

void keyboard(unsigned char key, int mousePositionX, int mousePositionY)
{
    int moires = leftRight_rotation < 0 ? leftRight_rotation % 360 + 360 : leftRight_rotation % 360;

    switch (key)
    {
    case 's':
        stopFlag = !stopFlag;
        break;
    case 'i':
        if (moires <= 45 || moires > 315) upDown_z += r_speed;
        else if (moires > 135 && moires <= 225) upDown_z -= r_speed;
        else if (moires > 45 && moires <= 135) upDown_x -= r_speed;
        else if (moires > 225 && moires <= 315) upDown_x += r_speed;
        break;
    case 'k':
        if (moires <= 45 || moires > 315) upDown_z -= r_speed;
        else if (moires > 135 && moires <= 225) upDown_z += r_speed;
        else if (moires > 45 && moires <= 135) upDown_x += r_speed;
        else if (moires > 225 && moires <= 315) upDown_x -= r_speed;
        break;
    case '-':
        if (down_speed > 0.01f && !stopFlag) down_speed -= 0.005f;
        break;
    case '=':
        if (down_speed <= 0.10f && !stopFlag) down_speed += 0.005f;
        break;
    case 'f':
        if (zoom > 2) zoom -= 2;
        break;
    case 'd':
        if (zoom < 200) zoom += 2;
        break;
    case 'j':
        leftRight_rotation += r_speed;
        break;
    case 'l':
        leftRight_rotation -= r_speed;
        break;
    case 'x':
        if (!stopFlag) shp.rotateShape_x();
        break;
    case 'c':
        if (!stopFlag) shp.rotateShape_y();
        break;
    case 'v':
        if (!stopFlag) shp.rotateShape_z();
        break;
    case '1':
        viewMode = 1;
        break;
    case '0':
        viewMode = 0;
        break;
    case KEY_ESCAPE:
        exit(0);
        break;
    default:
        break;
    }
}

void initialize()
{
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, win.width, win.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspect = (GLfloat)win.width / win.height;
    gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    GLfloat amb_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat specular[] = { 0.5f, 0.5f, 0.6f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char** argv) {
    win.width = 640;
    win.height = 480;
    win.title = "Tetris 3D";
    win.field_of_view_angle = 45;
    win.z_near = 1.0f;
    win.z_far = 500.0f;
    for (int i = 0; i < d; i++)
        for (int j = 0; j < d; j++)
            for (int k = 0; k < d; k++)
                base[i][j][k] = 0;
    shp.createShape();
    shp.initShape();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win.width, win.height);
    glutCreateWindow(win.title);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    initialize();
    glutMainLoop();
    return 0;
}
