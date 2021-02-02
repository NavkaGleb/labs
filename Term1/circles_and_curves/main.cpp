#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "functions.h"

using namespace std;

constexpr double PI = 3.14;
constexpr int SCALE = 8;
constexpr int FPS = 200;
constexpr int WINDOW_SIZE = 900;

const char* title = "my imagination";
const float speed = 0.2;

Circle rowCircles[SCALE - 1];
Circle columnCircles[SCALE - 1];

Point rowPoints[SCALE - 1];
Point columnPoints[SCALE - 1];

Curve curves[SCALE - 1][SCALE - 1];

void set_initial_values() {

    static int cellWidth = WINDOW_SIZE / SCALE;
    static double radius = cellWidth / 2.0 - 10;
    static double start = cellWidth / 2.0;
    static double end = start + (SCALE - 1) * cellWidth;

    double changingX;
    double changingY;
    float curSpeed;

    // set default coordinates to circles and points
    for (int i = 0; i < SCALE - 1; i++) {

        changingX = start + (i + 1) * cellWidth;
        changingY = start + (SCALE - 2 - i) * cellWidth;
        curSpeed = float(i + 1) * speed;

        rowCircles[i] = Circle(start, changingY, radius, curSpeed);
        columnCircles[i] = Circle(changingX, end, radius, curSpeed);

        rowPoints[i].updateLocation(rowCircles[i]);
        columnPoints[i].updateLocation(columnCircles[i]);

        for (int j = 0; j < SCALE - 1; j++)
            curves[i][j].clear();

    }

}

void init() {

    gluOrtho2D(0, WINDOW_SIZE, 0, WINDOW_SIZE);
    glClearColor(0.2, 0.2, 0.2, 1);

    // opacity
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    set_initial_values();

}

void animation() {

    for (int i = 0; i < SCALE - 1; i++) {

        // updating angles of circles
        rowCircles[i].updateAngle();
        columnCircles[i].updateAngle();

        // updating the position of points relative to the circles
        rowPoints[i].updateLocation(rowCircles[i]);
        columnPoints[i].updateLocation(columnCircles[i]);

        // push new points to curves
        for (int j = 0; j < SCALE - 1; j++) {

            if (rowCircles[0].getAngle() >= 360 || columnCircles[0].getAngle() >= 360)
                set_initial_values();
            else
                curves[i][j].pushPoint(Point(columnPoints[j].getX(), rowPoints[i].getY()));

        }

    }

    glutPostRedisplay();

}

void timer(int) {

    animation();
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);

}

void display() {

    glClear( GL_COLOR_BUFFER_BIT );

    for (int i = 0; i < SCALE - 1; i++) {

        glColor4f(1, 1, 1, 0.4);
        glLineWidth(1);
        drawLine(0, rowPoints[i].getY(), WINDOW_SIZE, rowPoints[i].getY());
        drawLine(columnPoints[i].getX(), 0, columnPoints[i].getX(), WINDOW_SIZE);

        glColor3f(1, 1, 1);
        rowCircles[i].draw(GL_LINE_LOOP);
        columnCircles[i].draw(GL_LINE_LOOP);
        rowPoints[i].draw();
        columnPoints[i].draw();

        glColor3f(1.0, 0.7, 0.3);
        glLineWidth(2);
        for (int j = 0; j < SCALE - 1; j++)
            curves[i][j].draw();

    }

    glutSwapBuffers();

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(50,50);
    glutCreateWindow(title);

    init();

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();

    return 0;

}