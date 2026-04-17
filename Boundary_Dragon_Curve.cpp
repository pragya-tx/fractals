// boundary of the curve

#include <GLUT/glut.h>
#include <vector>
#include <cmath>
#include <algorithm>

const double PI = 3.141592653589793;

struct Pt { double x, y; };
std::vector<Pt> gPoints;
std::vector<Pt> pts;

size_t drawn = 0;
size_t total = 0;
int animationSteps = 0;

void dragonBorder(Pt start, double n, double deg, int level)
{
    if(n <= 0.25) { gPoints.push_back(start); return; }

    double rad = deg * PI / 180.0;

    if(level > 1)
        dragonBorder(start, n / (2*sqrt(2.0)), deg + 45, level - 1);
    else
        gPoints.push_back(start);

    Pt a;
    a.x = start.x + cos(rad) * 2 * n;
    a.y = start.y - sin(rad) * 2 * n;

    deg += 180;

    if(level > 1)
        dragonBorder(a, n / (2*sqrt(2.0)), deg, level - 1);
    else
        gPoints.push_back(a);

    deg += 135;

    dragonBorder(a, n / sqrt(2.0), deg, level);
}

void normalizePoints()
{
    double mnx = 1e9, mny = 1e9, mxx = -1e9, mxy = -1e9;
    for(auto &p : gPoints) {
        mnx = std::min(mnx, p.x);
        mny = std::min(mny, p.y);
        mxx = std::max(mxx, p.x);
        mxy = std::max(mxy, p.y);
    }

    double cx = (mnx + mxx)/2.0;
    double cy = (mny + mxy)/2.0;
    double scale = 1.8 / std::max(mxx - mnx, mxy - mny);

    for(auto &p : gPoints)
        pts.push_back({(p.x - cx)*scale, (p.y - cy)*scale});

    total = pts.size();
}

void display()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.6f,1.0f);

    for(size_t i = 0; i < drawn; i++)
        glVertex2d(pts[i].x, pts[i].y);

    glEnd();
    glutSwapBuffers();
}

void update(int value)
{
    drawn += animationSteps;
    if(drawn > total) drawn = total;

    glutPostRedisplay();

    if(drawn < total)
        glutTimerFunc(16, update, 0);
}

void initCurve()
{
    Pt origin{0,0};
    dragonBorder(origin, 220.0, 0, 12);
    normalizePoints();

    int frames = 2*60; // 2 seconds @ 60 FPS
    animationSteps = (total + frames - 1) / frames; 
}

int main(int argc, char** argv)
{
    initCurve();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000,800);
    glutCreateWindow("Dragon Curve – Full");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}