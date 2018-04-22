#include <iostream>
//// ------------------- OpenMesh
#include<windows.h>//eclipse上一定要有这句话
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#include<string>
#include<vector>
#include<iostream>
using namespace std;

// Windows include files

#ifdef _WIN32
#include <windows.h>
#endif



// OpenGL include files

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

typedef struct Vertex//点的结构
{
	float x,y,z;
}Vertex;

typedef struct Facep//面的结构
{
	int num;
	int order[3];//面会有3个点
}Facep;
static char *filename="D:\\FC\\work\\ex2ply\\src\\bunny.ply";
Vertex ver[100000];
Facep fap[200000];
int i,j,k,n_node,n_face,n_edge;
float scale=1,spin=0;
static int window_height = 800;
static int window_width = 800;
float xx=0.0,yy=0.0,zz=1.0;
float wx=0.0,wy=0.0,wz=0.0;
bool xsame=false,ysame=false,zsame=false;
//bool move=false;
int readoff(const char* filename)
{
//	FILE *fp;
	FILE* fp = fopen(filename,"r");
//	if(!(fp=fopen(filename,"r")))
//	{
//		fprintf(stderr,"load失败");
//		return 0;
//	}
    char buffer[1024];
    	fseek(fp,0,SEEK_END);                              //把文件内部指针移到末尾，offset为0，故指针停在文件末尾处

        long fileSize = ftell(fp);                         //记录文件长度，file指针相对于文件首的偏移量

    	fseek(fp,0,SEEK_SET);                              //指针移到文件开头位置

           if (fp)
           {



        	   char buffer[1000];


        	   fgets(buffer,300,fp);			                //读取ply中数据,每次读一行到buffer中.

        	   while (  strncmp( "element vertex", buffer,strlen("element vertex")) != 0  )//读取的这一行如果没有element vertex字符串，则继续读下一行，若有则跳出循环进行处理
        	   {
        		   fgets(buffer,300,fp);
        	   }
        	   strcpy(buffer, buffer+strlen("element vertex"));   //把是vertex的那一行的后面的点的总数记录下来
        	   sscanf(buffer,"%d", &n_node);                    //从buffer中读数据，以十进制的形式存到vertexlist中
    	  //sscanf(buffer,"%d", &totalnor);

        	   fseek(fp,0,SEEK_SET);
        	   while (  strncmp( "element face", buffer,strlen("element face")) != 0  )
        	   {
        		   fgets(buffer,300,fp);
        	   }
        	   strcpy(buffer, buffer+strlen("element face"));
        	   sscanf(buffer,"%d", &n_face);

        	   while (  strncmp( "end_header", buffer,strlen("end_header")) != 0  )
        	   {
        		   fgets(buffer,300,fp);
        	   }



        	   for(i=0;i<n_node;i++)//读入多少个点信息
    				{
    					fgets(buffer,1023,fp);
    					sscanf(buffer,"%f%f%f",&ver[i].x,&ver[i].y,&ver[i].z);//点
    				}
    			for(i=0;i<n_face;i++)//读入面，面有几个顶点，顶点1的索引、顶点2的索引、顶点3的索引。
    			{
    				fgets(buffer,1023,fp);
    				sscanf(buffer,"%d%d%d%d",&fap[i].num,&fap[i].order[0],&fap[i].order[1],&fap[i].order[2]);
    			}//面信息



    			fclose(fp);
    			printf("%s Loaded!\n",filename);

           }else
           {
          printf("File can't be opened\n");
           }

}
void redraw(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
//由于模型和视图的变换都通过矩阵运算来实现，在进行变换前，应先设置当前操作的矩阵为“模型
//	视图矩阵”。设置的方法是以 GL_MODELVIEW 为参数调用 glMatrixMode 函数，
	glMatrixMode(GL_MODELVIEW);//通常，我们需要在进行变换前把当前矩阵设置为单位矩阵。这也只需要一行代码
	glLoadIdentity();
//	glPopMatrix();
	glScalef(scale,scale,scale);//缩放函数,把当前矩阵和一个表示缩放物体的矩阵相乘。x,y,z 分别表示在该方向上的缩放比例。
	cout<<"旋转点为：x-"<<xx<<";y-"<<yy<<";z-"<<zz<<endl;
	glRotatef(spin,xx,yy,zz);//旋转,物体将绕着(0,0,0)到(x,y,z)的直线以逆时针旋转，参数 angle 表示旋转的角度
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(5.0,5.0,5.0);
	glTranslatef(wx, wy, wz);//把当前矩阵和一个表示移动物体的矩阵相乘。三个参数分别表示了在三个坐标上的位移值。
//	glPushMatrix();
	glBegin(GL_TRIANGLES);//基本图元
//	if(move==true){
//		move=false;
//	}
	for(i=0;i<n_face;i++)
	{
		int count=fap[i].order[0];//找第一个点索引
		glVertex3f(ver[count].x,ver[count].y,ver[count].z);//对应点
		count=fap[i].order[1];//第二个点索引
		glVertex3f(ver[count].x,ver[count].y,ver[count].z);
		count=fap[i].order[2];//第三个点索引
		glVertex3f(ver[count].x,ver[count].y,ver[count].z);
	}

	glEnd();
	glFlush();
	glutSwapBuffers();

}
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	   case 'p':glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);glutPostRedisplay();break;//point
       case 'l':glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);glutPostRedisplay();break;//line
       case 'f':glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);glutPostRedisplay();break;//fill
       case 'e':scale*=1.1;glutPostRedisplay();break;
       case 'r':scale*=0.9;glutPostRedisplay();break;
       case 'z':
    	   if(!zsame)zz+=1.0;zsame=true;xsame=false;ysame=false;
    	   spin=spin+1;
    	   glutPostRedisplay();
    	   glFlush();
    	   break;
//       case 'd':if(!zz==1.0){zz=1.0;}spin=spin-1;glutPostRedisplay();glFlush();break;
//       case 'z':if(!xx==1.0){xx=1.0;yy=0.0;zz=0.0;}spin=spin+1;glutPostRedisplay();glFlush();break;
//       case 'x':if(!xx==1.0&yy==0.0&zz==0.0)){xx=1.0;yy=0.0;zz=0.0;}spin=spin-1;glutPostRedisplay();glFlush();break;
//       case 'c':if(!(xx==0.0&yy==1.0&zz==0.0)){xx=0.0;yy=1.0;zz=0.0;}spin=spin+1;glutPostRedisplay();glFlush();break;
//       case 'v':if(!(xx==0.0&yy==1.0&zz==0.0)){xx=0.0;yy=1.0;zz=0.0;}spin=spin-1;glutPostRedisplay();glFlush();break;
       case 'x':if(!zsame)zz+=1.0;zsame=true;xsame=false;ysame=false;spin=spin-1;glutPostRedisplay();glFlush();break;
       case 'c':if(!xsame)xx+=1.0;xsame=true;zsame=false;ysame=false;;spin=spin+1;glutPostRedisplay();glFlush();break;
       case 'v':if(!xsame)xx+=1.0;xsame=true;zsame=false;ysame=false;spin=spin-1;glutPostRedisplay();glFlush();break;
       case 'b':if(!ysame)yy+=1.0;ysame=true;zsame=false;xsame=false;spin=spin+1;glutPostRedisplay();glFlush();break;
       case 'n':if(!ysame)yy+=1.0;ysame=true;zsame=false;xsame=false;spin=spin-1;glutPostRedisplay();glFlush();break;
       case 'a':wx-=0.1;spin=spin+1;glutPostRedisplay();glFlush();break;
       case 'd':wx+=0.1;;spin=spin-1;glutPostRedisplay();glFlush();break;
       case 'w':wy+=0.1;spin=spin+1;glutPostRedisplay();glFlush();break;
       case 's':wy-=0.1;;spin=spin-1;glutPostRedisplay();glFlush();break;
	}
}
void mouse(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		scale*=1.1;
		glutPostRedisplay();
	}
//	if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
//	{
//
//		for(i=0;i<10;i++)
//		{
//			spin=spin+1;
//		    glutPostRedisplay();
//			glFlush();
//		}
//	}
}
void init(int *argc, char **argv)
{
  // Open window
  glutInit(argc, argv);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(window_width, window_height);

  glutCreateWindow("OFF文件读取");

  // Initialize GLUT callback functions
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //glutReshapeFunc(GLUTResize);
  glutDisplayFunc(redraw);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  //glutMotionFunc(motion);
  glutIdleFunc(0);


}

int main(int argc, char** argv)
{
	init(&argc,argv);

	readoff(filename);
	glutMainLoop();
	return 0;
}
