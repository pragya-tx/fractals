// filling the area for visually pleasing structure. (gradient coloring)

#include <GLUT/glut.h>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point
{
    float x,y;
};

string sequence="FX";
int iterations=15;

vector<Point> points;

int drawIndex=0;
int segmentsPerFrame=140;

int winW=900;
int winH=900;

float step=1;
float angle=90;

struct Color
{
    float r,g,b;
};

Color lerp(Color a, Color b, float t)
{
    return {
        a.r + (b.r-a.r)*t, 
        a.g + (b.g-a.g)*t,
        a.b + (b.b-a.b)*t
    };
}

Color gradient(float t)
{
    
    Color light = {0.88f,0.93f,1.00f};   
    Color mid   = {0.40f,0.62f,1.00f};   
    Color dark  = {0.08f,0.28f,0.90f};   

    if(t < 0.5f)
        return lerp(light,mid,t*2.0f);
    else
        return lerp(mid,dark,(t-0.5f)*2.0f);
}

string generate(string s)
{
    string r;

    for(char c:s)
    {
        if(c=='X') r+="X+YF+";
        else if(c=='Y') r+="-FX-Y";
        else r+=c;
    }

    return r;
}

void buildSequence()
{
    for(int i=0;i<iterations;i++)
        sequence=generate(sequence);
}

void computePoints()
{
    float x=0,y=0,dir=0;

    points.push_back({x,y});

    for(char c:sequence)
    {
        if(c=='F')
        {
            x+=step*cos(dir*M_PI/180);
            y+=step*sin(dir*M_PI/180);

            points.push_back({x,y});
        }
        else if(c=='+') dir+=angle;
        else if(c=='-') dir-=angle;
    }
}

void normalize()
{
    float minx=1e9,maxx=-1e9,miny=1e9,maxy=-1e9;

    for(auto&p:points)
    {
        minx=min(minx,p.x);
        maxx=max(maxx,p.x);
        miny=min(miny,p.y);
        maxy=max(maxy,p.y);
    }

    float cx=(minx+maxx)/2;
    float cy=(miny+maxy)/2;

    float scale=max(maxx-minx,maxy-miny);

    for(auto &p:points)
    {
        p.x=(p.x-cx)/scale*1.9;
        p.y=(p.y-cy)/scale*1.9;
    }
}

void drawTile(Point a,Point b,float t)
{
    float dx=b.x-a.x;
    float dy=b.y-a.y;

    float len=sqrt(dx*dx+dy*dy);

    float nx=-dy/len;
    float ny= dx/len;

    float w=len*0.85f; // original width, no enlargement

    Color c = gradient(t);
    glColor3f(c.r,c.g,c.b);

    glBegin(GL_QUADS);

    glVertex2f(a.x+nx*w,a.y+ny*w);
    glVertex2f(b.x+nx*w,b.y+ny*w);
    glVertex2f(b.x-nx*w,b.y-ny*w);
    glVertex2f(a.x-nx*w,a.y-ny*w);

    glEnd();
}

void drawText(float x,float y,string text)
{
    glRasterPos2f(x,y);

    for(char c:text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,c);
}

void drawReplayButton()
{
    float x1=0.65;
    float y1=-0.95;

    float x2=0.95;
    float y2=-0.85;

    glColor3f(0.20,0.28,0.48);

    glBegin(GL_QUADS);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();

    glColor3f(1,1,1);
    drawText(0.73,-0.91,"Replay");
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for(int i=0;i<drawIndex;i++)
    {
        float t=(float)i/(points.size()-1);
        drawTile(points[i],points[i+1],t);
    }

    drawReplayButton();

    glutSwapBuffers();
}

void update(int)
{
    drawIndex+=segmentsPerFrame;

    if(drawIndex>=points.size()-1)
        drawIndex=points.size()-1;

    glutPostRedisplay();

    glutTimerFunc(16,update,0);
}

void mouse(int button,int state,int x,int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        float nx=(float)x/winW*2-1;
        float ny=1-(float)y/winH*2;

        if(nx>0.65 && nx<0.95 && ny>-0.95 && ny<-0.85)
            drawIndex=0;
    }
}

void reshape(int w,int h)
{
    winW=w;
    winH=h;

    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
}

void init()
{
    glClearColor(0.08,0.08,0.08,1);
}

int main(int argc,char**argv)
{
    buildSequence();
    computePoints();
    normalize();

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(winW,winH);
    glutCreateWindow("Dragon Curve Animation");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutTimerFunc(0,update,0);

    glutMainLoop();
}

