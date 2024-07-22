#include "stdio.h"
#include "math.h"
#include "windows.h"
#include "star.h"
#include "time.h"
#define PlanetNum 10
void main()
{
	//char ch;
	planet *p[PlanetNum];
	star *s[3];
	HWND hwnd;
	HDC hdc;
	hwnd=GetConsoleWindow();
	hdc=GetDC(hwnd);
	system("color 0d");
    srand((int)time(0));
	
	//for(int i=0;i<=2;i++)
	//{
	//  s[i]=new star(hdc);
	//}
	//s[0]=new star(hdc,RGB(0,0,255));
	//s[1]=NULL;
	//s[2]=NULL;
	//s[1]=new star(hdc,RGB(0,255,0));
	while(1)
	{
	s[0]=new star(hdc,RGB(255,0,0));
	s[1]=new star(hdc,RGB(0,255,0));
	s[2]=new star(hdc,RGB(0,0,255));
	//s[1]=NULL;
	//s[2]=NULL;
//	s[0]->x=200*scale;
//	s[0]->y=100*scale;
//	s[0]->z=0;

	/*s[0]->vtx=1.205680397;
	s[0]->vty=2.70710653;
	s[0]->vtz=33.87062;
	s[0]->mass=500000000;*/
	//s[0]->Alpha=45*PI/180;
//	s[0]->Alpha=0;
//	s[0]->Beta=0;
//	s[0]->mass=500000000;

	//s[1]->y=300*cos(15*PI/180)*scale+100;
//	s[1]->y=200*scale;
	//s[1]->x=300*sin(15*PI/180)*scale+100;
//	s[1]->x=200*scale;
//	s[1]->z=0;

    /*s[1]->vtx=42.475365316;
	s[1]->vty=0.741624;
	s[1]->vtz=65.41631;
	s[1]->mass=600000000;*/

	//s[1]->Alpha=165*PI/180;
//	s[1]->Alpha=PI;
//	s[1]->Beta=0;
//	s[1]->mass=500000000;


	//s[2]->y=300*sin(15*PI/180)*scale+100;
//	s[2]->y=650*scale;
	//s[2]->x=300*cos(15*PI/180)*scale+100;
//	s[2]->x=200*scale;
//	s[2]->z=1800;
//	s[2]->z=0;

	/*s[2]->vtx=42.7152;
	s[2]->vty=41.24916161624;
	s[2]->vtz=26.43785;
	s[2]->mass=200000000;*/

	//s[2]->Alpha=285*PI/180;
//	s[2]->Alpha=0;
	//s[2]->Beta=PI/2;
//	s[2]->Beta=0;
//	//s[2]->mass=500000000;
//	s[2]->mass=10;
	//s[2]->v0=3500;
//	s[2]->v0=sqrt(2000000);
	//s[2]->mass=0;

	s[0]->move(s[1],s[2]);
	s[1]->move(s[0],s[2]);
	s[2]->move(s[0],s[1]);
	IsClose=0;
	while(1)
	{
		Sleep(10);
		if(s[0]->IsDead&&s[1]->IsDead&&s[2]->IsDead)
		{
			delete s[0];
			delete s[1];
			delete s[2];
			s[0]=NULL;
			s[1]=NULL;
			s[2]=NULL;
			system("color 0d");
			Sleep(1000);
			break;
		}
	}
	}
	/*
while(1)
{	
	for(int j=0;j<=PlanetNum-1;j++)
	{
	  p[j]=new planet(hdc,s);
	}
	//planet *p=new planet(hdc,s);
	for(j=0;j<=PlanetNum-1;j++)
	{
	  p[j]->motion(p,j,PlanetNum);
	}
	
	while(1)
	{
	  //printf("%d\n",g_Dead);
	  if(g_Dead==PlanetNum)
	  {
         IsClose=1;
		 break;
	  }	  
	  Sleep(10);
	}
	Sleep(1000);
    g_Dead=0;
    IsClose=0;
	//delete[] p;
	for(j=0;j<=PlanetNum-1;j++)
	{
		delete p[j];
	}
	s[0]->IsDead=true;
	Sleep(1000);
	delete s[0];
	s[0]=new star(hdc,RGB(255,0,0));
}*/	
	while(1)
	{
	  char ch=getchar();
	  if(ch=='q')
	  {
		  IsClose=1;
		  break;
	  }
	  Sleep(10);
	}
	//getchar();
	//getchar();
	//delete []s;
	//delete []p;
	/*
    for(j=0;j<=PlanetNum-1;j++)
	{
	  delete p[j];
	}
	for(j=0;j<=2;j++)
	{
		delete s[j];
	}*/
	//delete []s;
	//printf("%d\n",g_FreeNum);
	//getchar();
	//getchar();

}