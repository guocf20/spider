#include <GL/glut.h>
#include <stdio.h>
#include<math.h>
int day = 1;
float move = 20.0;
void display(void);
void myidle(void);
void keyboard(unsigned char key, int x, int y);
int main(int argc, char **argv)
{
     glutInit(&argc, argv);
     glutInitWindowSize(800,800); 
     glutCreateWindow(argv[0]);
     glutInitWindowPosition(-100,-100);
     glutDisplayFunc(display);
     glutIdleFunc(&myidle);
     glutKeyboardFunc(keyboard);
     gluOrtho2D(10,20,10,20);
     glutMainLoop();
 
     return 0;
}
void display(void)
{
     move = move + 20;
     if (move > 200)
     {
         move = 20;
     }
     glViewport(move,2*move,400,400);
     //glOrtho(0,400,0, 400,-10,10);
     glShadeModel(GL_SMOOTH);
     glClear(GL_COLOR_BUFFER_BIT);
     glLoadIdentity();
     //gluLookAt(0.5,0,-2,0,0,-2,0,1,0); 
     if (day == 1)
     {
          glColor3f(1,0,0);
     }
     else if(day == 2)
     {
          glColor3f(0,1,0);
     }
     else
     {
          glColor3f(0,0,1); 
     }
     /*if((day - 2) < 0.001)
     {
         printf("yes\n");
     glutSolidSphere(1.0f,day,5);
     }
     else*/
     //glBegin(GL_POLYGON);
     glPointSize(2);
     glBegin(GL_POINTS);
     float i = 0;
     for (i = -2.00; (i - 2)< 0.001; i+=0.00001)
     {glVertex2f(i,sin(i));					
     glVertex2f(i,cos(i));						
     }
     for (i =-0.8; i<0.8;i+=0.00001)
     {
         glVertex2f(i,sqrt(0.64-i*i));
     }			
     for (i =0.8; i>-0.8;i-=0.00001)
     {
         glVertex2f(i,-sqrt(0.64-i*i));
     }
     for (i = -1.0; i < 1; i+=0.00001)
     {
         glVertex2f(i,i);
         glVertex2f(i,2*i*i);
     }
     for (i = -1.0; i <=1; i+=0.00001)
     {
        glVertex2f(i,0);
        glVertex2f(0,i);
     }
     glEnd();
     glPointSize(4);
     glColor3f(0xf0,0xf0,0);
     glBegin(GL_POINTS);
     glVertex2f(0.8,0);
     glVertex2f(-0.8,0);
     glVertex2f(0,-0.8);
     glVertex2f(0,0.8);
     glEnd();
     glBegin(GL_POINTS);
     for(i = -1;i< 1;i+=0.001)
     {
         glColor3f(0,fabs(i),fabs(i));
         glVertex2f(i,-i);
     }
     glEnd();
     sleep(1); 
     glFlush();
     //gluOrtho2D(10,20,10,20);
     glutSwapBuffers();
}
 

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
              exit(0);
        break;
    }
}
void myidle(void)
{
     ++day;
     if(day > 3)
     {
         day = 1;
        
     }
     display();
}
