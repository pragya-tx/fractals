// drawing dragon curve through l-system method

#include <GLUT/glut.h>
#include <string>
#include <cmath>
#include <algorithm>

std::string sequence="FX";
int iterations=15;

int drawIndex=0;
bool running=false;

int speed=3;

int winW=800,winH=800;

float step=0.01;
float angle=90;

float startX=0,startY=0,zoom=0.6;

bool draggingSlider=false;

struct Button{
float x1,y1,x2,y2;
const char* label;
};

struct Slider{
float x1,y1,x2,y2;
float value;
};

Button startBtn,pauseBtn,replayBtn;
Slider speedSlider;

void drawText(float x,float y,const char* text)
{
    glColor3f(1,1,1);

    void* font = GLUT_BITMAP_HELVETICA_12;

    int width=0;
    for(int i=0;text[i]!='\0';i++)
        width+=glutBitmapWidth(font,text[i]);

    float offset=(width*(2.0f/winW))/2.0f;

    glRasterPos2f(x-offset,y);

    for(int i=0;text[i]!='\0';i++)
        glutBitmapCharacter(font,text[i]);
}

void generateDragon()
{
for(int i=0;i<iterations;i++)
{
std::string next;

for(char c:sequence)
{
if(c=='X') next+="X+YF+";
else if(c=='Y') next+="-FX-Y";
else next+=c;
}

sequence=next;
}
}

void computeCenter()
{
float x=0,y=0,dir=0;
float minx=0,maxx=0,miny=0,maxy=0;

for(char c:sequence)
{
if(c=='F')
{
float nx=x+step*cos(dir*M_PI/180);
float ny=y+step*sin(dir*M_PI/180);

x=nx;
y=ny;

minx=std::min(minx,x);
maxx=std::max(maxx,x);
miny=std::min(miny,y);
maxy=std::max(maxy,y);
}
else if(c=='+') dir+=angle;
else if(c=='-') dir-=angle;
}

startX=-(minx+maxx)/2;
startY=-(miny+maxy)/2;
}

void drawDragon()
{
float x=startX;
float y=startY;
float dir=0;

int count=0;

glBegin(GL_LINES);

for(char c:sequence)
{
if(count>drawIndex) break;

if(c=='F')
{
float nx=x+step*cos(dir*M_PI/180);
float ny=y+step*sin(dir*M_PI/180);

glVertex2f(x,y);
glVertex2f(nx,ny);

x=nx;
y=ny;

count++;
}
else if(c=='+') dir+=angle;
else if(c=='-') dir-=angle;
}

glEnd();
}

void drawButton(Button b)
{
glColor3f(0.2,0.4,0.9);

glBegin(GL_QUADS);
glVertex2f(b.x1,b.y1);
glVertex2f(b.x2,b.y1);
glVertex2f(b.x2,b.y2);
glVertex2f(b.x1,b.y2);
glEnd();

float cx=(b.x1+b.x2)/2;
float cy=(b.y1+b.y2)/2-0.01;

drawText(cx,cy,b.label);
}

void drawSlider()
{
glColor3f(0.5,0.5,0.5);

glBegin(GL_QUADS);
glVertex2f(speedSlider.x1,speedSlider.y1);
glVertex2f(speedSlider.x2,speedSlider.y1);
glVertex2f(speedSlider.x2,speedSlider.y2);
glVertex2f(speedSlider.x1,speedSlider.y2);
glEnd();

float knob=speedSlider.x1+(speedSlider.x2-speedSlider.x1)*speedSlider.value;

glColor3f(1,0,0);

glBegin(GL_QUADS);
glVertex2f(knob-0.01,speedSlider.y1-0.02);
glVertex2f(knob+0.01,speedSlider.y1-0.02);
glVertex2f(knob+0.01,speedSlider.y2+0.02);
glVertex2f(knob-0.01,speedSlider.y2+0.02);
glEnd();

float cx=(speedSlider.x1+speedSlider.x2)/2;

drawText(cx,speedSlider.y2+0.03,"Speed");
}

void display()
{
glClear(GL_COLOR_BUFFER_BIT);

glLoadIdentity();

glScalef(zoom,zoom,1);

glColor3f(1,1,1);
drawDragon();

glLoadIdentity();

drawButton(startBtn);
drawButton(pauseBtn);
drawButton(replayBtn);

drawSlider();

glFlush();
}

void update(int v)
{
if(running && drawIndex<sequence.size())
{
drawIndex+=speed;
glutPostRedisplay();
}

glutTimerFunc(16,update,0);
}

bool inside(Button b,float x,float y)
{
return (x>b.x1 && x<b.x2 && y>b.y1 && y<b.y2);
}

bool insideSlider(float x,float y)
{
return (x>speedSlider.x1 && x<speedSlider.x2 && y>speedSlider.y1 && y<speedSlider.y2);
}

void updateSlider(float nx)
{
float ratio=(nx-speedSlider.x1)/(speedSlider.x2-speedSlider.x1);

ratio=std::max(0.0f,std::min(1.0f,ratio));

speedSlider.value=ratio;

speed=1+ratio*15;

glutPostRedisplay();
}

void mouse(int button,int state,int x,int y)
{
if(button!=GLUT_LEFT_BUTTON) return;

float nx=(float)x/winW*2-1;
float ny=1-(float)y/winH*2;

if(state==GLUT_DOWN)
{

if(inside(startBtn,nx,ny))
running=true;

else if(inside(pauseBtn,nx,ny))
running=false;

else if(inside(replayBtn,nx,ny))
{
drawIndex=0;
running=true;
}

else if(insideSlider(nx,ny))
{
draggingSlider=true;
updateSlider(nx);
}
}

if(state==GLUT_UP)
draggingSlider=false;
}

void motion(int x,int y)
{
if(!draggingSlider) return;

float nx=(float)x/winW*2-1;

updateSlider(nx);
}

void setupUI()
{
float width=0.18;
float gap=0.04;

float sliderWidth=0.25;

float total=3*width+sliderWidth+3*gap;
float start=-total/2;

startBtn={start,-0.9,start+width,-0.8,"START"};
pauseBtn={start+width+gap,-0.9,start+2*width+gap,-0.8,"PAUSE"};
replayBtn={start+2*(width+gap),-0.9,start+3*width+2*gap,-0.8,"REPLAY"};

speedSlider={start+3*(width+gap),-0.88,start+3*(width+gap)+sliderWidth,-0.82,0.2};
}

int main(int argc,char** argv)
{
generateDragon();
computeCenter();
setupUI();

glutInit(&argc,argv);
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowSize(winW,winH);

glutCreateWindow("Dragon Curve Simulation");

glClearColor(0,0,0,1);

glutDisplayFunc(display);
glutMouseFunc(mouse);
glutMotionFunc(motion);
glutTimerFunc(16,update,0);
glutMainLoop();
}
