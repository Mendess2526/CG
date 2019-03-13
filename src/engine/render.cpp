#include "camera.hpp"
#include "group.hpp"
#include "model.hpp"

#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

using namespace std;

namespace Render {

static float SCALE = 1;
static Group* SCENE;
static int DRAW_LEVEL = -1;

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;
    // compute window's aspect ratio
    float ratio = w * 1.0 / h;
    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    Camera::place_camera();
    // x axis
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    {
        glVertex3f(-10000, 0, 0);
        glVertex3f(10000, 0, 0);
    }
    glEnd();
    // y axis
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    {
        glVertex3f(0, -10000, 0);
        glVertex3f(0, 10000, 0);
    }
    glEnd();
    // z axis
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, -10000);
        glVertex3f(0, 0, 10000);
    }
    glEnd();
    glScalef(SCALE, SCALE, SCALE);

    SCENE->draw(DRAW_LEVEL);

    stringstream title;
    title << "CG-Engine | Draw Level: " << DRAW_LEVEL << " | Camera Mode: " << Camera::to_string(Camera::current_camera());
    glutSetWindowTitle(title.str().data());

    glutSwapBuffers();
}

void key_bindings(unsigned char key, int _x, int _y)
{
    Camera::process_key_bind(key);
    switch (key) {
    case 'v':
        Camera::toggle_cam();
        break;
    case '+':
        SCALE += 0.1;
        break;
    case '-':
        SCALE -= 0.1;
        break;
    case '[':
        if (DRAW_LEVEL > 1)
            DRAW_LEVEL--;
        break;
    case ']':
        if (DRAW_LEVEL < SCENE->levels())
            DRAW_LEVEL++;
        break;
    case 'q':
        exit(0);
    }
    renderScene();
}

int render(int argc, char** argv, Group* scene)
{
    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG-Engine");
    // Required callback registry
    SCENE = scene;
    DRAW_LEVEL = SCENE->levels();
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(key_bindings);
    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // enter GLUT's main cycle
    glutMainLoop();
    return 1;
}

}
