#include<GL/glut.h>
//#include<cmath>
#include<math.h>
#include<stdlib.h>
#include <stdio.h>
#include<algorithm>
#include<iostream>
using namespace std;

int width=500;
int height=500;
float image[500][500][3];

struct Vector
{
	double x,y;
};
Vector P[10];
Vector save_P[10]={0};
Vector P_blen[2];
int keyboard_ctrl=4;

struct Color
{
	 float red,green,blue;
};

int get_fac(int n)
{
	return (n==1||n==0)?1:n*get_fac(n-1);
}

double get_bkn(int n,int k,double t)
{
	return(get_fac(n)/(get_fac(k)*get_fac(n-k))*pow(1-t,n-k)*pow(t,k));
}

void Blending_func(int point_num)
{
	double t=0,x,y;
	int p_x=0,p_y=0,k=0;
	for(t=0;t<1;t+=0.01)
	{
		x=0;
		y=0;
		for(k=0;k<=point_num-1;k++)
		{
			//cout<<"x,y:"<<p[k].x<<p[k].y<<endl;
			//cout<<k<<endl;
			x+=P[k].x*get_bkn(point_num-1,k,t);
			y+=P[k].y*get_bkn(point_num-1,k,t);
		}
		//cout<<k<<endl;
		p_x=(x+1)*250;
		p_y=(y+1)*250;
		//cout<<p_x<<p_y<<endl;
		image[p_y][p_x][0]=1.0;
		image[p_y][p_x][1]=1.0;
		image[p_y][p_x][2]=0.0;
		//glDrawPixels(width,height,GL_RGB,GL_FLOAT,image);
	}
}

void de_Casteljau(int num_left,double t)
{
	//Vector p[10];//]=P[point_num];
	int i;
	if(num_left>1)
	{
		for(i=0;i<=num_left-2;i++)
		{
			P[i].x=P[i].x*(1-t)+P[i+1].x*t;//_*de_Casteljau(point_num,t,x,y).x+(1-t)*de_Casteljau(point_num-1,t,x,y).x;
			P[i].y=P[i].y*(1-t)+P[i+1].y*t;//p.y=t*de_Casteljau(point_num-1,n+1,t,x,y).y+(1-t)*de_Casteljau(point_num-1,n,t,x,y).y;
			//cout<<p[i].x<<p[i].y<<endl;
		}
		de_Casteljau(num_left-1,t);	
	}
	//return p[0];
}

void Subdivision(int num_left,double t)
{
	int i;
	char sub_choice='n';
	
	if(num_left==1)
	{
		glPointSize(7.0);
		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(P[0].x,P[0].y);
		glEnd(); 
	}
	else
	{
		for(i=0;i<=num_left-2;i++)
		{
			P[i].x=P[i].x*(1-t)+P[i+1].x*t;
			P[i].y=P[i].y*(1-t)+P[i+1].y*t;
			P_blen[i%2].x=P[i].x;
			P_blen[i%2].y=P[i].y;
			
			if(i>0)
			{
				glPointSize(5.0);
				glColor3f(0.0, 0.7, 1.0);
				glBegin(GL_POINTS);
				glVertex2f(P_blen[0].x,P_blen[0].y);
				glVertex2f(P_blen[1].x,P_blen[1].y);
				glEnd(); 
				glLineWidth(0.5); 
				glColor3f(0.0, 1.0, 0.0);
				glBegin(GL_LINES);
				glVertex2f(P_blen[0].x,P_blen[0].y);
				glVertex2f(P_blen[1].x,P_blen[1].y);
				glEnd();
				glFlush();
			}
		}
		Subdivision(num_left-1,t);
	}
}

void de_Casteljau1(int point_num)
{
	double t=0;
	int i,j;
	int p_x=0,p_y=0,k=0;
	//for(i=0;i<=9;i++)
		//p[i]=P[i];
	for(t=0;t<1;t+=0.0001)
	{
		de_Casteljau(point_num,t);
		p_x=(P[0].x+1)*250;
		p_y=(P[0].y+1)*250;	
		//cout<<p_x<<p_y<<endl;
		image[p_y][p_x][0]=1.0;
		image[p_y][p_x][1]=1.0;
		image[p_y][p_x][2]=0.0;
		//return P[point_num];
	}
}


void drawBezierCurve(Vector Points[10], int sizeCtrlPoints)
{
	GLfloat ctrlPoints[10][3],v;
	int i=0;
	for(i=0;i<=sizeCtrlPoints-1;i++)
	{
		ctrlPoints[i][0]=Points[i].x;
		ctrlPoints[i][1]=Points[i].y;
		ctrlPoints[i][2]=0;
	}
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, sizeCtrlPoints, &ctrlPoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_LINE_STRIP); 
		for(v=0;v<=1;v+=0.01 ) 
			glEvalCoord1f(v);
	glEnd();
	//glDisable(GL_MAP1_VERTEX_3);
	glPointSize(5.0);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POINTS);
		for(i=0;i<=sizeCtrlPoints-1;i++ )
			glVertex3f(ctrlPoints[i][0],ctrlPoints[i][1],ctrlPoints[i][2]);
	glEnd(); 
	//glFlush();
}

void drawBezierSurface()
{
	GLfloat ctrlpoints[4][4][3] = {
		{{-0.6, -0.6, 1.0}, {-0.2, -0.6, 0.5}, 
			{0.2, -0.6, -0.25}, {0.6, -0.6, 0.5}}, 
		{{-0.6, -0.2, 0.25}, {-0.2, -0.2, 0.75}, 
			{0.2, -0.2, 0.0}, {0.6, -0.2, -0.25}}, 
		{{-0.6, 0.2, 1.0}, {-0.2, 0.2, 0.0}, 
			{0.2, 0.2, 0.75}, {0.6, 0.2, 1.0}}, 
		{{-0.6, 0.6, -0.5}, {-0.2, 0.6, -0.5}, 
			{0.2, 0.6, 0.0}, {0.6, 0.6, -0.25}}
	};
   int i, j;
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   //glShadeModel(GL_FLAT);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 0.0, 1.0);
   glPushMatrix ();
   glRotatef(85.0, 1.0, 1.0, 1.0);
   for (j=0; j<=10; j++) {
      glBegin(GL_LINE_STRIP);
      for (i=0; i<=50; i++)
         glEvalCoord2f((GLfloat)i/50.0, (GLfloat)j/10.0);
      glEnd();
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 50; i++)
         glEvalCoord2f((GLfloat)j/10.0, (GLfloat)i/50.0);
      glEnd();
   }
   glPopMatrix ();
   glFlush();
}

void display()
{
	int point_num,i,j=0,k=0,sum_p=0;
	int choice,t=0.5;
	char mode;
	for(i=0;i<=499;i++)
		for(j=0;j<=499;j++)
		{
			image[j][i][0]=0;
			image[j][i][1]=0;
			image[j][i][2]=0;
		}
	cout<<"[1] Blending Function"<<endl;
	cout<<"[2] De Casteljau with Subdivision"<<endl;
	cout<<"[3] OpenGL function"<<endl;
	cout<<"[4] Bazier surface"<<endl;
	cout<<"Please input your option:"<<endl;
	cin>>choice;
	if(choice==1||choice==2||choice==3)
	{
		cout<<"Please select number of control points: 6/8? (6:2 segments 8:3 segments)"<<endl;
		cin>>point_num;
		cout<<"Do you want to input the control points or use the set ones:(y/n)"<<endl;
		cin>>mode;
		if(mode=='y')
		{
			cout<<"input control points:"<<endl;
			for(i=0;i<=point_num-1;i++)
				cin>>P[i].x>>P[i].y;
		}
		else
		{
			P[0].x=-0.8;P[0].y=0.5;
			P[1].x=-0.6;P[1].y=-0.6;
			P[2].x=-0.3;P[2].y=-0.6;
			P[3].x=0.2;P[3].y=0.3;
			P[4].x=0.4;P[4].y=0.3;
			P[5].x=0.6;P[5].y=-0.5;
			P[6].x=0.7;P[6].y=-0.2;
			P[7].x=0.8;P[7].y=0.4;
		}
			
		for(j=0;j<=point_num-1;j++)
		{
			//cout<<i;
			save_P[sum_p]=P[j];
			if(sum_p!=0&&sum_p%3==0&&j<point_num-1)
			{
				save_P[sum_p].x=(P[(sum_p/3)*2].x+P[(sum_p/3)*2+1].x)/2;
				save_P[sum_p].y=(P[(sum_p/3)*2].y+P[(sum_p/3)*2+1].y)/2;
				j--;
			}
			//cout<<save_P[sum_p].x<<save_P[sum_p].y<<endl;
			//save_P[i]=P[i];
			sum_p++;
			//j++;
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor (1.0, 1.0, 1.0, 0.0);
	switch(choice)
	{
		case 1:
			{
				//cout<<"sum_p j:"<<sum_p<<j<<endl;
				//k=0;
				for(j=0;j<=sum_p-1;j++)
				{
					P[0]=save_P[j++];
					P[1]=save_P[j++];
					P[2]=save_P[j++];
					P[3]=save_P[j];
					Blending_func(4);
					glDrawPixels(width,height,GL_RGB,GL_FLOAT,image);		
					if(j<sum_p-1)
						j--;		
				}
				for(j=0;j<=sum_p-1;j++)
				{
					P[0]=save_P[j++];
					P[1]=save_P[j++];
					P[2]=save_P[j++];
					P[3]=save_P[j];
				
					//Subdivision(4,0.5);
					if(j<sum_p-1)
						j--;
				}
				break;
			}
		case 2:
			{
				for(j=0;j<=sum_p-1;j++)
				{
					P[0]=save_P[j++];
					P[1]=save_P[j++];
					P[2]=save_P[j++];
					P[3]=save_P[j];
					de_Casteljau1(4);
					glDrawPixels(width,height,GL_RGB,GL_FLOAT,image);		
					if(j<sum_p-1)
						j--;		
				}
				for(j=0;j<=sum_p-1;j++)
				{
					P[0]=save_P[j++];
					P[1]=save_P[j++];
					P[2]=save_P[j++];
					P[3]=save_P[j];
					Subdivision(4,0.5);
					if(j<sum_p-1)
						j--;			
				}
				break;
			}
		case 3:
			{
				for(j=0;j<=sum_p-1;j++)
				{
					P[0]=save_P[j++];
					P[1]=save_P[j++];
					P[2]=save_P[j++];
					P[3]=save_P[j];
					drawBezierCurve(P,4);
					//Subdivision(4,0.5);
					if(j<sum_p-1)
						j--;		
				}
				break;
			}
			case 4:drawBezierSurface();break;
			default:break;
	}

	if(choice!=4)
	{
		glPointSize(5.0);
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POINTS);
			for(i=0;i<=sum_p-1;i++)
				glVertex2f(save_P[i].x, save_P[i].y);
		glEnd(); 

		glLineWidth(0.5); 
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
		for(i=0;i<=sum_p-1;i++)
		{
			if(i<sum_p-1)
			{
				glVertex2f(save_P[i].x, save_P[i].y);
				glVertex2f(save_P[i+1].x, save_P[i+1].y);
			}
		}
		glEnd();
	}
	glutPostRedisplay();
	glutSwapBuffers();
}

int main(int argc,char *argv[])
{
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(width,height);
	glutCreateWindow("CURVE");
	glutDisplayFunc(&display);
	//glutKeyboardFunc(keyboard);
	glutMainLoop();
	system("pause");
}