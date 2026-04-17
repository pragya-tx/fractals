// snowflake gradient through 4 dragon curves connected at middle from the start point of each curve.

#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const int ITERATIONS = 12;
const float SIZE = 300.0f;

const float POINT_SIZE = 2.0f;
const float ANIMATION_TIME = 2.5f;

struct Point{
    float x,y;
};

vector<vector<Point>> paths(4);

double startTime;

struct Button{
    float x,y,w,h;
};

Button replayButton = {WINDOW_WIDTH-110,20,90,35};

void generate_dragon(float x,float y,float dx,float dy,int iter,bool right,vector<Point>& path)
{
    if(iter==0)
    {
        path.push_back({x+dx,y+dy});
        return;
    }

    float nx=(dx+(right?-dy:dy))/2.0f;
    float ny=(dy+(right?dx:-dx))/2.0f;

    generate_dragon(x,y,nx,ny,iter-1,true,path);

    Point last=path.back();

    generate_dragon(last.x,last.y,dx-nx,dy-ny,iter-1,false,path);
}

void blue_white_gradient(float t,float &r,float &g,float &b)
{
    r=t;
    g=t;
    b=1.0f;
}

void drawReplayButton()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,WINDOW_WIDTH,0,WINDOW_HEIGHT,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.2f,0.3f,0.9f);

    glBegin(GL_QUADS);
        glVertex2f(replayButton.x,replayButton.y);
        glVertex2f(replayButton.x+replayButton.w,replayButton.y);
        glVertex2f(replayButton.x+replayButton.w,replayButton.y+replayButton.h);
        glVertex2f(replayButton.x,replayButton.y+replayButton.h);
    glEnd();

    glColor3f(1,1,1);

    glBegin(GL_LINE_LOOP);
        glVertex2f(replayButton.x,replayButton.y);
        glVertex2f(replayButton.x+replayButton.w,replayButton.y);
        glVertex2f(replayButton.x+replayButton.w,replayButton.y+replayButton.h);
        glVertex2f(replayButton.x,replayButton.y+replayButton.h);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

bool insideReplay(double mx,double my)
{
    return mx>=replayButton.x &&
           mx<=replayButton.x+replayButton.w &&
           my>=replayButton.y &&
           my<=replayButton.y+replayButton.h;
}

void mouse_button_callback(GLFWwindow* window,int button,int action,int mods)
{
    if(button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS)
    {
        double mx,my;
        glfwGetCursorPos(window,&mx,&my);

        my=WINDOW_HEIGHT-my;

        if(insideReplay(mx,my))
        {
            startTime=glfwGetTime();
        }
    }
}

int main()
{
    if(!glfwInit())
        return -1;

    GLFWwindow* window=glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Dragon Snowflake",NULL,NULL);

    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window,mouse_button_callback);

    glPointSize(POINT_SIZE);

    paths.assign(4,vector<Point>());

    generate_dragon(0,0,0,-SIZE,ITERATIONS,true,paths[0]);
    generate_dragon(0,0,SIZE,0,ITERATIONS,true,paths[1]);
    generate_dragon(0,0,0,SIZE,ITERATIONS,true,paths[2]);
    generate_dragon(0,0,-SIZE,0,ITERATIONS,true,paths[3]);

    size_t totalPoints=paths[0].size();

    startTime=glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-WINDOW_WIDTH/2,WINDOW_WIDTH/2,-WINDOW_HEIGHT/2,WINDOW_HEIGHT/2,-1,1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double elapsed=glfwGetTime()-startTime;

        float progress=min(1.0,elapsed/ANIMATION_TIME);

        size_t visiblePoints=progress*totalPoints;

        for(size_t i=1;i<visiblePoints;i++)
        {
            float t=(float)i/totalPoints;

            float r,g,b;

            blue_white_gradient(t,r,g,b);

            glColor3f(r,g,b);

            glBegin(GL_LINES);

            for(int c=0;c<4;c++)
            {
                glVertex2f(paths[c][i-1].x,paths[c][i-1].y);
                glVertex2f(paths[c][i].x,paths[c][i].y);
            }

            glEnd();

            glBegin(GL_POINTS);

            for(int c=0;c<4;c++)
                glVertex2f(paths[c][i].x,paths[c][i].y);

            glEnd();
        }

        drawReplayButton();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}