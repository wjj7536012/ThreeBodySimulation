#include "stdlib.h"
#define PI 3.1415926
#define G 6.67259
#define scale 1
extern  "C" WINBASEAPI HWND WINAPI GetConsoleWindow();
//int gPlanetNum=0;
int g_FreeNum=0;
bool IsClose=0;
int g_Dead=0;
float param_Kp = 50.0;   
float param_Ki = 0.20;   
typedef struct direction3D
{
	float roll;//z
	float pitch;//x
	float yaw;//y
}direction3D;
typedef struct quaterInfo_t
{
	float q0;
	float q1;
	float q2;
	float q3;
}quaterInfo_t;
int random(int a,int b)
{
	int z;
	z=rand()%(b-a)+a;
	return z;
}
void circle(HDC hdc,int x,int y,int r,COLORREF c)
{
    HGDIOBJ pBrush;	
	pBrush=CreateSolidBrush(c);
	SelectObject(hdc,pBrush);
	Ellipse(hdc,x-r,y-r,x+r,y+r);	
	DeleteObject(pBrush);	
}
class star
{ 
public:
	FILE *fp;
	HDC m_hdc;
	star *o_star1;
	star *o_star2;
	char *Name;
	double x;
	double y;
	double z;
	int r;
	float g,t;
	double v0;
	float direction;
	double mass;
	double Alpha;
	double Beta;
	COLORREF m_color;
	bool IsDead;
	static DWORD __stdcall StarDrawThread(LPVOID lpPara)
	{
	  star *s=(star*)lpPara;
	  while(1)
	  {
	    circle(s->m_hdc,s->x,s->y,s->r,s->m_color); 
		if(s->IsDead)
		{   circle(s->m_hdc,s->x,s->y,s->r,RGB(0,0,0));
			break;
		}
		Sleep(10);
	  }
      return 0;
	}
	static DWORD __stdcall StarMoveThread(LPVOID lpPara)
	{
	  star *s=(star*)lpPara;
	  s->motion();	 
      return 0;
	}
	star(HDC hdc,COLORREF color)
	{
		int i,rnd;
		char str[25];
		char FileName[64];
		x=random(0,600*scale);
		y=random(0,390*scale);
		z=random(0,600*scale);
	//	r=random(5,10);
		r=10;
	//	v0=129.099444873581;
		//v0=1290.994448735806;
		//v0=sqrt(50000/3);
		//v0=408.24829;
		//v0=sqrt(5000000/3);
		//v0=sqrt(5000000/3);
		//v0=sqrt(25000000/4);
	//	v0=sqrt(5000000/2);
		v0=random(10000,100000);
		//v0=0;
		Alpha=(float)random(0,360)*PI/180;
		Beta=(float)random(0,360)*PI/180;
		direction=(float)random(0,360)*PI/180;
		mass=random(1,10)*scale*scale*10000000000*r;
    	//mass=200000*scale*scale;
		t=(float)random(1,10)/10;
		g=random(0,20)+t;
		m_hdc=hdc;
		m_color=color;
        IsDead=false;
		i=10;
		char *c="star-";
		Name=(char*)malloc(i*26+5+1);
		memset(Name,0,i*26+5+1);
		memset(FileName,0,sizeof(FileName));
		strcpy(Name,c);
		for(i=10;i>0;i--)
		{
			//r=rand();
			rnd=random(i*10,100*i);
			memset(str,0,sizeof(str));
		    itoa(rnd,str,10);
			strcat(Name,str);
			if(i>1)
			{
			   strcat(Name,"-");
			}
		}
		strcpy(FileName,Name);
		strcat(FileName,".txt");
	    fp=fopen(FileName,"a+");
		if(fp==NULL)
		{
			exit(0);
		}
		//move(NULL,NULL);
        //circle(hdc,x,y,r,RGB(255,0,0));
		//CreateThread(NULL,0,StarDrawThread,this,0,0); 
		//CreateThread(NULL,0,StarMoveThread,this,0,0); 
	}
	void move(star *s1,star *s2)
	{
	   o_star1=s1;
	   o_star2=s2;
       CreateThread(NULL,0,StarMoveThread,this,0,0); 
	}
private:
	void LogFile(double v,int flag)
	{
		//char FileName[64];
		//strcpy(FileName,Name);
		//strcat(FileName,".txt");
		if(fp==NULL)
			return;
		//fseek(fp,0,0);
		if(flag==0)
		{
          fprintf(fp,"\n");
		}
		else if(flag==1)
		{
		  fprintf(fp,"%lf ",v);
		}
		else if(flag==2)
		{
			fprintf(fp,"(");
		}
		else if(flag==3)
		{
			fprintf(fp,",");
		}
		else if(flag==4)
		{
			fprintf(fp,")");
		}
		else if(flag==5)
		{
			fprintf(fp,"********************************************************************************\n");
		}
		else if(flag==6)
		{
            fprintf(fp,"%lf",v);
		}
		
		//fclose(fp);
	}
	void motion()
	{
        double lx1,ly1,lz1,lx2,ly2,lz2,angle1,AngleAlpha1,AngleBeta1,AngleAlpha2,AngleBeta2,angle2,d1,d2;
		double g1,g2,gx1,gx2,gy1,gy2,gz1,gz2,vt,vtx,vty,vtz,dt,dx,dy,dz;
		double ang1,ang2,AngAlpha1,AngBeta1,AngAlpha2,AngBeta2,i;
		
		/*
		if(o_star1==NULL||o_star2==NULL)
		{
            CreateThread(NULL,0,StarDrawThread,this,0,0); 
			return;
		}*/
       	lx1=o_star1->x-x;
		ly1=-(o_star1->y-y);
		lz1=o_star1->z-z;
		lx2=o_star2->x-x;
		ly2=-(o_star2->y-y);
		lz2=o_star2->z-z;
        if(lx1!=0)
		{
		  angle1=atan(ly1/lx1);
		  AngleAlpha1=atan(ly1/lx1);
		  AngleBeta1=atan(lz1/sqrt(lx1*lx1+ly1*ly1));
		}
		else
		{
		  angle1=0;
          AngleAlpha1=0;
          if(ly1!=0)
		  {
			  AngleBeta1=atan(lz1/ly1);
		  }
		  else
		  {
               if(lz1>0)
			   {
                 AngleBeta1=PI/2;
			   }
			   else
			   {
				 AngleBeta1=PI/2+PI;
			   }
		  }
		}
		if(lx2!=0)
		{
		  angle2=atan(ly2/lx2);
		  AngleAlpha2=atan(ly2/lx2);
		  AngleBeta2=atan(lz2/sqrt(lx2*lx2+ly2*ly2));
		}
		else
		{
		  angle2=0;
          AngleAlpha2=0;
          if(ly2!=0)
		  {
			  AngleBeta2=atan(lz2/ly2);
		  }
		  else
		  {
               if(lz2>0)
			   {
                 AngleBeta2=PI/2;
			   }
			   else
			   {
				 AngleBeta2=PI/2+PI;
			   }
		  }

		}

		ang1=180*angle1/PI;
		AngAlpha1=180*AngleAlpha1/PI;
		AngBeta1=180*AngleBeta1/PI;

		ang2=180*angle2/PI;
		AngAlpha2=180*AngleAlpha2/PI;
		AngBeta2=180*AngleBeta2/PI;


		if(o_star1->x<x)
		{
			angle1+=PI;
		    ang1+=180;
			AngleAlpha1+=PI;
			AngAlpha1+=180;
		}
		if(o_star2->x<x)
		{
			angle2+=PI;
		    ang2+=180;
			AngleAlpha2+=PI;
			AngAlpha2+=180;

		}

		//g1=o_star1->g;
		//g2=o_star2->g;

		d1=sqrt(lx1*lx1+ly1*ly1+lz1*lz1);
		d2=sqrt(lx2*lx2+ly2*ly2+lz2*lz2);

		//d1*=10;
		//d2*=10;

		g1=o_star1->mass/(d1*d1);
		g2=o_star2->mass/(d2*d2);

	//	printf("%f\n",g1);
	//	printf("%f\n",g2);
		gz1=g1*sin(AngleBeta1);
		gx1=g1*cos(AngleBeta1)*cos(AngleAlpha1);
		gy1=g1*cos(AngleBeta1)*sin(AngleAlpha1);

        gz2=g2*sin(AngleBeta2);
		gx2=g2*cos(AngleBeta2)*cos(AngleAlpha2);
		gy2=g2*cos(AngleBeta2)*sin(AngleAlpha2);

		/*gx1=g1*cos(angle1);
		gx2=g2*cos(angle2);

		gy1=g1*sin(angle1);
		gy2=g2*sin(angle2);*/

		i=0.0f;

		//v0=60;

        /*direction=angle1+PI/2;

		vtx=v0*cos(direction);
		vty=v0*sin(direction);*/
		//Alpha=AngleAlpha1+PI/2;
		//Beta=AngleBeta1;

		if(Beta==PI/2)
		{
			//vtx=v0;
			vty=0;
			//vtz=0;
			vtz=v0;
		}
		else if(Beta==0)
		{
			vtx=v0*cos(Alpha);
			vty=v0*sin(Alpha);
			vtz=0;
		}
		else
		{

		  vtx=v0*cos(Beta)*cos(Alpha);
		  vty=v0*cos(Beta)*sin(Alpha);
          vtz=v0*sin(Beta);
		}
        //vtx=0;
		//vty=0;
		//vtz=0;
		vt=sqrt(vtx*vtx+vty*vty+vtz*vtz);
		
		LogFile(0,2);
		LogFile(x,6);
		LogFile(0,3);
        LogFile(y,6);
		LogFile(0,3);
		LogFile(z,6);
		LogFile(0,3);
		LogFile(mass,6);
		LogFile(0,3);
        LogFile(v0,6);
		LogFile(0,3);
        LogFile(Alpha,6);
		LogFile(0,3);
		LogFile(Beta,6);
		LogFile(0,4);
		LogFile(0,0);
		LogFile(0,5);

		for(dt=0.0001;i<=10000.0f;i+=0.1)
		{
		   vtx+=(gx1+gx2)*dt;
		   vty+=(gy1+gy2)*dt;
		   vtz+=(gz1+gz2)*dt;
		   dx=vtx*dt;
		   dy=vty*dt;
		   dz=vtz*dt;
		   x+=dx;
		   y-=dy;
		   z+=dz;

		   //circle(m_hdc,(int)x/Scale,(int)y/Scale,r,m_color);
		    circle(m_hdc,(int)x/scale,(int)y/scale,r,m_color);

		   lx1=o_star1->x-x;
		   ly1=-(o_star1->y-y);
		   lz1=o_star1->z-z;
		   lx2=o_star2->x-x;
		   ly2=-(o_star2->y-y);
		   lz2=o_star2->z-z;
		   

		   //angle1=atan(ly1/lx1);
		   //angle2=atan(ly2/lx2);
		   if(lx1!=0)
		{
		  angle1=atan(ly1/lx1);
		  AngleAlpha1=atan(ly1/lx1);
		  AngleBeta1=atan(lz1/sqrt(lx1*lx1+ly1*ly1));
		}
		else
		{
		  angle1=0;
          AngleAlpha1=0;
          if(ly1!=0)
		  {
			  AngleBeta1=atan(lz1/ly1);
		  }
		  else
		  {
               if(lz1>0)
			   {
                 AngleBeta1=PI/2;
			   }
			   else
			   {
				 AngleBeta1=PI/2+PI;
			   }
		  }
		}
		if(lx2!=0)
		{
		  angle2=atan(ly2/lx2);
		  AngleAlpha2=atan(ly2/lx2);
		  AngleBeta2=atan(lz2/sqrt(lx2*lx2+ly2*ly2));
		}
		else
		{
		  angle2=0;
          AngleAlpha2=0;
          if(ly2!=0)
		  {
			  AngleBeta2=atan(lz2/ly2);
		  }
		  else
		  {
               if(lz2>0)
			   {
                 AngleBeta2=PI/2;
			   }
			   else
			   {
				 AngleBeta2=PI/2+PI;
			   }
		  }

		}

		   //ang1=180*angle1/PI;
		   //ang2=180*angle2/PI;
		ang1=180*angle1/PI;
		AngAlpha1=180*AngleAlpha1/PI;
		AngBeta1=180*AngleBeta1/PI;

		ang2=180*angle2/PI;
		AngAlpha2=180*AngleAlpha2/PI;
		AngBeta2=180*AngleBeta2/PI;

		   if(o_star1->x<x)
		   {
			angle1+=PI;
		    ang1+=180;
			AngleAlpha1+=PI;
			AngAlpha1+=180;
		   }
		   if(o_star2->x<x)
		   {
			angle2+=PI;
		    ang2+=180;
			AngleAlpha2+=PI;
			AngAlpha2+=180;
		   }

           vt=sqrt(vtx*vtx+vty*vty+vtz*vtz);

           d1=sqrt(lx1*lx1+ly1*ly1+lz1*lz1);
		   d2=sqrt(lx2*lx2+ly2*ly2+lz2*lz2);

		   //d1*=10;
		   //d2*=10;

           g1=o_star1->mass/(d1*d1);
		   g2=o_star2->mass/(d2*d2);

           /*gx1=g1*cos(angle1);
		   gx2=g2*cos(angle2);

		   gy1=g1*sin(angle1);
		   gy2=g2*sin(angle2);*/
		   gz1=g1*sin(AngleBeta1);
		   gx1=g1*cos(AngleBeta1)*cos(AngleAlpha1);
		   gy1=g1*cos(AngleBeta1)*sin(AngleAlpha1);

           gz2=g2*sin(AngleBeta2);
		   gx2=g2*cos(AngleBeta2)*cos(AngleAlpha2);
		   gy2=g2*cos(AngleBeta2)*sin(AngleAlpha2);

		   /*direction=angle1+PI/2;

		   vtx=vt*cos(direction);
		   vty=vt*sin(direction);*/
		   //Alpha=AngleAlpha1+PI/2;
		   //Beta=AngleBeta1;
		   /*if(Beta==PI/2)
		   {
		    Alpha=(float)random(0,360)/180*PI;
			vtx=vt*cos(Alpha);
			vty=vt*sin(Alpha);
			vtz=0;
		   }
		else if(Beta==0)
		{
			vtx=vt*cos(Alpha);
			vty=vt*sin(Alpha);
			vtz=0;
		}
		else
		{
		   vtx=vt*cos(Beta)*cos(Alpha);
		   vty=vt*cos(Beta)*sin(Alpha);
           vtz=vt*sin(Beta);
		}*/

           Sleep(10);
		   //system("color 0d");
           circle(m_hdc,(int)x/scale,(int)y/scale,r,RGB(0,0,0));
		   //Sleep(10);

		   //d1/=10;
		   //d2/=10;
		   
           LogFile(x,1);
		   LogFile(y,1);
		   LogFile(z,1);
		   LogFile(vt,1);
		   LogFile(g1,1);
		   LogFile(g2,1);
		   //LogFile(g1,1);
		   //LogFile(g2,1);

		   LogFile(d1,1);
		   LogFile(d2,1);
		   //LogFile(d1,1);
		   //LogFile(d2,1);

		   //LogFile(ang,1);
		   //LogFile(ang1,1);
		   //LogFile(ang2,1);
		   LogFile(AngAlpha1,1);
           LogFile(AngBeta1,1);

		   LogFile(AngAlpha2,1);
           LogFile(AngBeta2,1);

		   LogFile(0,0);
		   if(d1<(o_star1->r+r)||d1>=2000*scale)
		   {
			   
			   //printf("crash! ");
			   //mass=0;
			   //o_star1->mass=0;
			   fclose(fp);
			   IsDead=true;
			   break;
		   }
		    if(d2<(o_star2->r+r)||d2>=2000*scale)
		   {
			  
			  // printf("crash! ");
			   //mass=0;
			   //o_star2->mass=0;
			   fclose(fp);
			   IsDead=true;
			   break;
		   }

		  //circle(m_hdc,x,y,r,RGB(0,0,0));
		  if(IsDead)
		  {
			  fclose(fp);
			  break;
		  }
		}
	}
	
};
class planet
{
public:
	FILE *fp;
    planet **m_p;
	planet *new_p;
	HDC m_hdc;
	char *Name;
	star *m_star;
	star *m_star1;
	star *m_star2;
	double x;
	double y;
	double z;
	float Alpha;
	float Beta;
	int r;
	double v0;
	float direction;
	direction3D D3d;
	direction3D eulerAngle;
	quaterInfo_t Q_info;
	float gravity_acc[3];
	double mass;
	COLORREF color;
	int SequenceNumber;
	int PlanetAmount;
	bool IsDead;
	planet(HDC hdc,star **s)
	{
		int i,rnd;
		char str[25];
		char FileName[64];
        x=random(0,600);
		y=random(0,390);
		z=random(0,600);
		r=random(5,9);
		v0=random(0,80);
		mass=random(50,500);
        Alpha=(float)random(0,360)/180*PI;
		Beta=(float)random(0,360)/180*PI;
		D3d.yaw=Alpha+PI/2;
		D3d.pitch=Beta;
		D3d.roll=(float)random(0,360)/180*PI;
		direction=(float)random(0,360)/180*PI;
		//direction.row=(float)random(0,360)/180*PI;
		//color=RGB(random(0,255),random(0,255),random(0,255));
		color=RGB(0,0,255);
		m_hdc=hdc;
		m_star=s[0];
		m_star1=s[1];
		m_star2=s[2];
		i=10;
		char *c="planet-";
		Name=(char*)malloc(i*26+5+1);
		memset(Name,0,i*26+5+1);
		memset(FileName,0,sizeof(FileName));
		strcpy(Name,c);
		for(i=10;i>0;i--)
		{
			//r=rand();
			rnd=random(i*10,100*i);
			memset(str,0,sizeof(str));
		    itoa(rnd,str,10);
			strcat(Name,str);
			if(i>1)
			{
			   strcat(Name,"-");
			}
		}
		strcpy(FileName,Name);
		strcat(FileName,".txt");
	    fp=fopen(FileName,"a+");
		if(fp==NULL)
		{
			exit(0);
		}
		//circle(hdc,x,y,r,RGB(0,0,255));
		//DeleteFile("log.txt");
		new_p=NULL;
		IsDead=0;
		//CreateThread(NULL,0,PlanetMoveThread,this,0,0); 
	}
	void motion(planet **p,int j,int n)
	{
	   m_p=p;
       SequenceNumber=j;
       PlanetAmount=n;
       CreateThread(NULL,0,PlanetMoveThread,this,0,0); 
	}
  float invSqrt(float x) 
  {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
  }
  void IMU_AHRSupdate_withMagnetic(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz,float delta_T)
  {
    float norm;
    float halfT = 0.5 * delta_T;
    float hx, hy, hz;
    float bx, bz;
    float vx, vy, vz;
    float wx, wy, wz;
    float ex, ey, ez;
    float delta_2 = 0;
    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;
    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;
	float I_ex=0;
	float I_ey=0;
	float I_ez=0;
 
    norm = invSqrt(ax*ax + ay*ay + az*az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
 
    norm = invSqrt(mx*mx + my*my + mz*mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;
 
    // compute reference direction of flux 通量的计算参考方向
    hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
    hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
    hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz;     
 
    // estimated direction of gravity and flux (v and w)
    vx = 2*(q1q3 - q0q2);				  	// 估计方向的重力
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
    wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
    wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
 
    // error is sum of cross product between reference direction of fields and direction measured by sensors
 
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);
 
    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
 
        I_ex += delta_T * ex;	// integral error scaled by Ki
        I_ey += delta_T * ey;
        I_ez += delta_T * ez;
        //exdev=(ex[1]-ex[0]) / halfT;
        //eydev=(ey[1]-ey[0]) / halfT;
        // ezdev=(ez[1]-ez[0]) / halfT;
        // adjusted gyroscope measurements
 
        gx = gx+ param_Kp*ex + param_Ki*I_ex;//+ Kd*exdev;
        gy = gy+ param_Kp*ey + param_Ki*I_ey;//+ Kd*eydev;
        gz = gz+ param_Kp*ez + param_Ki*I_ez;//+ Kd*ezdev;
    }
    delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);
    // 整合四元数率    四元数微分方程  四元数更新算法，二阶毕卡法
    q0 = (1-delta_2/8)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT;			
    q1 = (1-delta_2/8)*q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = (1-delta_2/8)*q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = (1-delta_2/8)*q3 + (q0*gz + q1*gy - q2*gx)*halfT;	
    // normalise quaternion
    norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;
  }
  void IMU_quaterToEulerianAngles()
  {
    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;
    eulerAngle.pitch = asin(-2*q1*q3 + 2*q0*q2) * 180/PI; // pitch
    eulerAngle.roll = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) * 180/PI; // roll
    eulerAngle.yaw = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1) * 180/PI; // yaw
 
/*   可以不用作姿态限度的限制
    if(eulerAngle.roll>90 || eulerAngle.roll<-90)
    {
        if(eulerAngle.pitch > 0)
        {
            eulerAngle.pitch = 180-eulerAngle.pitch;
        }
        if(eulerAngle.pitch < 0)
        {
            eulerAngle.pitch = -(180+eulerAngle.pitch);
        }
    }
    if(eulerAngle.yaw > 180)
    {
        eulerAngle.yaw -=360;
    }
    else if(eulerAngle.yaw <-180)
    {
        eulerAngle.yaw +=360;
    } 
    */
  }

/*计算向量的长度，用于判定pitch为90deg时的特殊情况*/
  float get_vector_mod(const float vector[3])
  {
    return sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
  }
  
/*通过内积法，求两个向量的夹角*/
  float makekam_vec_ang(const float vec_a[3],const float vec_b[3])/*return 0~180deg*/
  {
    float angle = 0.0f;
    float mod_of_vec_a = 0.0f;
    float mod_of_vec_b = 0.0f;
    mod_of_vec_a = get_vector_mod(vec_a);
    mod_of_vec_b = get_vector_mod(vec_b);
    if((0.001f >= mod_of_vec_a) || (0.001f >= mod_of_vec_b)){
        angle = 0.0f;/*无法计算某向量与零向量的夹角，返回0*/
    }else{
        float cos_value = (vec_a[0]*vec_b[0] + vec_a[1]*vec_b[1] + vec_a[2]*vec_b[2]) / (mod_of_vec_a * mod_of_vec_b);
        if(cos_value > 1.0f){
            cos_value=1.0f;
        }
        if(cos_value < -1.0f){
            cos_value=-1.0f;
        }
        angle = acos(cos_value);
        angle = angle * 180.0f / 3.1415f;
    }
    return angle;
  }

/*通过外积法，求平面两个非同向向量所在平面的法向量*/
inline void makekam_outer_product(const float vec1[3],const float vec2[3],float vecDst[3])
{
    vecDst[0] = vec1[1]*vec2[2] - vec2[1]*vec1[2];/*y1z2-y2z1*/
    vecDst[1] = vec2[0]*vec1[2] - vec1[0]*vec2[2];/*x2z1-x1z2*/
    vecDst[2] = vec1[0]*vec2[1] - vec2[0]*vec1[1];/*x1y2-x2y1*/
}
/*根据重力加速度，计算姿态角*/
void makekam_gacc_to_att(float gravity_acc[3])
{
    float roll = 0.0f;
    float pitch = 0.0f;
    float flight_z_acc[3] = {0.0f,0.0f,10.0f};
    //float flight_x_acc[3] = {10.0f,0.0f,0.0f};
    float flight_x_acc[3] = {D3d.roll,D3d.pitch,D3d.yaw};
    float plane_xog_normal_vec[3] ={0};
 
    pitch = 90.0f - makekam_vec_ang(flight_x_acc,gravity_acc);/*pitch处于-90°到+90°之间*/
    if((90.0f - fabsf(pitch)) > 0.1f){/*pitch与±90度相差大于0.1°*/
        makekam_outer_product(flight_x_acc,gravity_acc,plane_xog_normal_vec);
        roll = makekam_vec_ang(flight_z_acc,plane_xog_normal_vec) - 90.0f;
        if(gravity_acc[2] > 0.0f){
            if(roll < 0.0f){
                roll = -180.0f - roll;
            }else{
                roll = 180 - roll;
            }
        }
    }else{
        roll = 0.0f;/*判定x轴与重力方向处于一条直线，此为特殊情况，滚转角设定为0°*/
    }
    //*ret_roll = roll;
    //*ret_pitch = pitch;
	D3d.roll=roll;
	D3d.pitch=pitch;
}

	static DWORD __stdcall PlanetMoveThread(LPVOID lpPara)
	{
	  planet *p=(planet*)lpPara;
	  p->move();	 
      return 0;
	}

	void LogFile(double v,int flag)
	{
		//char FileName[64];
		//strcpy(FileName,Name);
		//strcat(FileName,".txt");
		if(fp==NULL)
			return;
		//fseek(fp,0,0);
		if(flag==0)
		{
          fprintf(fp,"\n");
		}
		else if(flag==1)
		{
		  fprintf(fp,"%lf ",v);
		}
		else if(flag==2)
		{
			fprintf(fp,"(");
		}
		else if(flag==3)
		{
			fprintf(fp,",");
		}
		else if(flag==4)
		{
			fprintf(fp,")");
		}
		else if(flag==5)
		{
			fprintf(fp,"********************************************************************************\n");
		}
		else if(flag==6)
		{
            fprintf(fp,"%lf",v);
		}
		
		//fclose(fp);
	}
	double ComputeDistance(planet *p)
	{
		double x=abs(p->x-this->x);
		double y=abs(p->y-this->y);
		double z=abs(p->z-this->z);
		double d=sqrt(x*x+y*y+z*z);
		//printf("%f\n",d);
		return d;
	}
	bool DetectDistance()
	{
		for(int i=0;i<=PlanetAmount-1;i++)
		{
			if(i!=SequenceNumber)
			{
				double d=ComputeDistance(m_p[i]);
				//printf("%f\n",d);
				if(d<m_p[i]->r+this->r)
				{
					//m_p[i]->x=0;
					//m_p[i]->y=0;
					//m_p[i]->r=0;
					MessageBox(NULL,Name,"撞上了",0);
					this->x=0;
					this->y=0;
					this->r=0;
					//this->IsDead=1;
                    m_p[i]->IsDead=1;
					return 1;
				}
			}
			else
			{
			//	printf("OK\n");
			}
		}
		return 0;
	}
	void move()
	{
		double lx,ly,lz,lx1,ly1,lx2,ly2,angle,AngleAlpha,AngleBeta,angle1,angle2,d,d1,d2;
		double g,g1,g2,gx,gx1,gy,gy1,gx2,gy2,gz,vtx,vty,vtz,vt,dx,dy,dz,dt,i;
		double ang,ang1,ang2,AngAlpha,AngBeta;
		//gPlanetNum++;
		//printf("%d ",gPlanetNum);
		if(m_star!=NULL)
		{
		  lx=m_star->x-x;
		  ly=-(m_star->y-y);
		  lz=m_star->z-z;
		}
		else
		{
			lx=0;
			ly=0;
			lz=0;
		}
		if(m_star1!=NULL)
		{
		  lx1=m_star1->x-x;
		  ly1=-(m_star1->y-y);
		}
		else
		{
			lx1=0;
			ly1=0;
		}
		if(m_star2!=NULL)
		{
		  lx2=m_star2->x-x;
		  ly2=-(m_star2->y-y);
		}
		else
		{
			lx2=0;
			ly2=0;
		}
        if(lx!=0)
		{
		  angle=atan(ly/lx);
		  AngleAlpha=atan(ly/lx);
		  AngleBeta=atan(lz/sqrt(lx*lx+ly*ly));
		}
		else
		{
          angle=0;
		  AngleAlpha=0;
          if(ly!=0)
		  {
			  AngleBeta=atan(lz/ly);
		  }
		  else
		  {
               if(lz>0)
			   {
                 AngleBeta=PI/2;
			   }
			   else
			   {
				 AngleBeta=PI/2+PI;
			   }
		  }
		}
		if(lx1!=0)
		{
		  angle1=atan(ly1/lx1);
		}
		else
		{
			angle1=0;
		}
		if(lx2!=0)
		{
		   angle2=atan(ly2/lx2);
		}
		else
		{
           angle2=0;
		}

		ang=180*angle/PI;
		AngAlpha=180*AngleAlpha/PI;
		AngBeta=180*AngleBeta/PI;
		ang1=180*angle1/PI;
		ang2=180*angle2/PI;

		if(m_star!=NULL)
		{
		  if(m_star->x<x)
		  {
			angle+=PI;
			AngleAlpha+=PI;
		    ang+=180;
			AngAlpha+=180;
		  }
		  
		}
		if(m_star1!=NULL)
		{
		  if(m_star1->x<x)
		  {
			angle1+=PI;
		    ang1+=180;
		  }
		}
		if(m_star2!=NULL)
		{
		  if(m_star2->x<x)
		  {
			angle2+=PI;
		    ang2+=180;
		  }
		}
		LogFile(0,2);
		if(m_star!=NULL)
		{
		  LogFile(m_star->x,6);
		  LogFile(0,3);
          LogFile(m_star->y,6);
		  LogFile(0,3);
		  LogFile(m_star->z,6);
          LogFile(0,3);
		  LogFile(m_star->mass,6);
		  LogFile(0,4);
		  LogFile(0,0);
		}

		if(m_star1!=NULL)
		{
		  LogFile(0,2);
		  LogFile(m_star1->x,6);
		  LogFile(0,3);
          LogFile(m_star1->y,6);
		  LogFile(0,3);
		  LogFile(m_star1->mass,6);
		  LogFile(0,4);
		  LogFile(0,0);
		}

		if(m_star2!=NULL)
		{
		  LogFile(0,2);
		  LogFile(m_star2->x,6);
		  LogFile(0,3);
          LogFile(m_star2->y,6);
		  LogFile(0,3);
	  	  LogFile(m_star2->mass,6);
		  LogFile(0,4);
		  LogFile(0,0);
		}

        LogFile(0,2);
		LogFile(x,6);
		LogFile(0,3);
        LogFile(y,6);
		LogFile(0,3);
		LogFile(z,6);
		LogFile(0,3);
		LogFile(mass,6);
		LogFile(0,4);
		LogFile(0,0);
		LogFile(0,5);
		//printf("%d\n",ang);
		if(m_star!=NULL)
		{
		  g=m_star->g;
		}
		else
		{
			g=0;
		}
		if(m_star1!=NULL)
		{
		  g1=m_star1->g;
		}
		else
		{
			g1=0;
		}
		if(m_star2!=NULL)
		{
		  g2=m_star2->g;
		}
		else
		{
			g2=0;
		}

        d=sqrt(lx*lx+ly*ly+lz*lz);
        d1=sqrt(lx1*lx1+ly1*ly1);
		d2=sqrt(lx2*lx2+ly2*ly2);
        
		if(m_star!=NULL&&d!=0)
		{
		  g=m_star->mass*G*10/(d*d);
		}
		else
		{
			g=0;
		}
		if(m_star1!=NULL&&d1!=0)
		{
		  g1=m_star1->mass*G*10/(d1*d1);
		}
		else
		{
		  g1=0;
		}
		if(m_star2!=NULL&&d2!=0)
		{
		  g2=m_star2->mass*G*10/(d2*d2);
		}
		else
		{
		  g2=0;
		}
        
        gz=g*sin(AngleBeta);
		gx=g*cos(AngleBeta)*cos(AngleAlpha);
		//gx=g*cos(angle);
        gx1=g1*cos(angle1);
		gx2=g2*cos(angle2);

		//gy=g*sin(angle);
		gy=g*cos(AngleBeta)*sin(AngleAlpha);
		gy1=g1*sin(angle1);
		gy2=g2*sin(angle2);

		i=0;
		//v0=60;
		//direction=angle-PI/2;
		//printf("%d\n",(int)(direction*180/PI));
		
		//vtx=v0*cos(direction);
		vtx=v0*cos(Beta)*cos(Alpha);
		//vty=v0*sin(direction);
		vty=v0*cos(Beta)*sin(Alpha);
        vtz=v0*sin(Beta);
		//printf("%f\n",g);
		//printf("%f\n",g1);
		//printf("%f\n",g2);
		vt=sqrt(vtx*vtx+vty*vty+vtz*vtz);
		LogFile(x,1);
		LogFile(y,1);
		LogFile(z,1);
		LogFile(vt,1);
		LogFile(g,1);
        //LogFile(g1,1);
		//LogFile(g2,1);

		LogFile(d,1);
		//LogFile(d1,1);
		//LogFile(d2,1);

		//LogFile(ang,1);
		LogFile(AngAlpha,1);
        LogFile(AngBeta,1);
		//LogFile(ang1,1);
		//LogFile(ang2,1);
		
		LogFile(0,0);
		for(dt=0.1;i<=10000;i+=0.001)
		{
           vtx+=(gx+gx1+gx2)*dt;
		   vty+=(gy+gy1+gy2)*dt;
		   vtz+=gz*dt;
		   dx=vtx*dt;
		   dy=vty*dt;
		   dz=vtz*dt;
		   x+=dx;
		   y-=dy;
		   z+=dz;
           circle(m_hdc,x,y,r,color);
		   if(m_star!=NULL)
		   {
             lx=m_star->x-x;
		     ly=-(m_star->y-y);
			 lz=m_star->z-z;
		   }
		   else
		   {
			 lx=0;
			 ly=0;
			 lz=0;
		   }
		   if(m_star1!=NULL)
		   {
		     lx1=m_star1->x-x;
		     ly1=-(m_star1->y-y);
		   }
		   else
		   {
			 lx1=0;
			 ly1=0;
		   }
		   if(m_star2!=NULL)
		   {
		     lx2=m_star2->x-x;
		     ly2=-(m_star2->y-y);
		   }
		   else
		   {
			 lx2=0;
			 ly2=0;
		   }
      
		   if(lx!=0)
		   {
		     angle=atan(ly/lx);
			 AngleAlpha=atan(ly/lx);
		     AngleBeta=atan(lz/sqrt(lx*lx+ly*ly));
			   //angle=atan(vty/vtx);
		   }
		   else
		   {
             angle=0;
			 AngleAlpha=0;
             if(ly!=0)
			 {
			  AngleBeta=atan(lz/ly);
			 }
		     else
			 {
			   if(lz>0)
			   {
                 AngleBeta=PI/2;
			   }
			   else
			   {
				 AngleBeta=PI/2+PI;
			   }
			 }
		   }
		   if(lx1!=0)
		   {
             angle1=atan(ly1/lx1);
		   }
		   else
		   {
			 angle1=0;
		   }
		   if(lx2!=0)
		   {
		     angle2=atan(ly2/lx2);
		   }
		   else
		   {
             angle2=0;
		   }

		   ang=180*angle/PI;
		   AngAlpha=180*AngleAlpha/PI;
		   AngBeta=180*AngleBeta/PI;
           ang1=180*angle1/PI;
		   ang2=180*angle2/PI;

		   if(m_star!=NULL)
		   {
		     if(m_star->x<x)
			 {
			  angle+=PI;
		      ang+=180;
			  AngleAlpha+=PI;
			  AngAlpha+=180;
			 }
			 
		   }
		   if(m_star1!=NULL)
		   {
		     if(m_star1->x<x)
			 {
			  angle1+=PI;
		      ang1+=180;
			 }
		   }
		   if(m_star2!=NULL)
		   {
		     if(m_star2->x<x)
			 {
			  angle2+=PI;
		      ang2+=180;
			 }
		   }
		   //g=m_star->g;
		   //gx=g*cos(angle);
		   //gy=g*sin(angle);
		   //direction=angle+PI/2;
		   vt=sqrt(vtx*vtx+vty*vty+vtz*vtz);
             if(vt<0)
			 {
			   MessageBox(NULL,Name,"vt出错了",0);
			 }

		   d=sqrt(lx*lx+ly*ly+lz*lz);
		   if(d<0)
		   {
			   MessageBox(NULL,Name,"d出错了",0);
		   }
           d1=sqrt(lx1*lx1+ly1*ly1);
		   d2=sqrt(lx2*lx2+ly2*ly2);

		   if(m_star!=NULL&&d!=0)
		   {
		     g=m_star->mass*G*10/(d*d);
			 if(g<0)
			 {
			   MessageBox(NULL,Name,"g出错了",0);
			 }

		   }
		   else
		   {
			 g=0;
		   }
		   if(m_star1!=NULL&&d1!=0)
		   {
		     g1=m_star1->mass*G*10/(d1*d1);
		   }
		   else
		   {
			 g1=0;
		   }
		   if(m_star2!=NULL&&d2!=0)
		   {
		     g2=m_star2->mass*G*10/(d2*d2);
		   }
		   else
		   {
			 g2=0;
		   }

		   //gx=g*cos(angle);
		   gx1=g1*cos(angle1);
		   gx2=g2*cos(angle2);

		   //gy=g*sin(angle);
		   gz=g*sin(AngleBeta);
		   gx=g*cos(AngleBeta)*cos(AngleAlpha);
		//gx=g*cos(angle);
           //gx1=g1*cos(angle1);
		   //gx2=g2*cos(angle2);

		//gy=g*sin(angle);
		   gy=g*cos(AngleBeta)*sin(AngleAlpha);
		   gy1=g1*sin(angle1);
		   gy2=g2*sin(angle2);
           gravity_acc[0]=gx;
		   gravity_acc[1]=gy;
		   gravity_acc[2]=gz;
           makekam_gacc_to_att(gravity_acc);

		   LogFile(x,1);
		   LogFile(y,1);
		   LogFile(z,1);
		   LogFile(vt,1);
		   LogFile(g,1);
		   //LogFile(g1,1);
		   //LogFile(g2,1);

		   LogFile(d,1);
		   //LogFile(d1,1);
		   //LogFile(d2,1);

		   //LogFile(ang,1);
		   //LogFile(ang1,1);
		   //LogFile(ang2,1);
		   LogFile(AngAlpha,1);
           LogFile(AngBeta,1);

		   LogFile(D3d.roll,1);
		   LogFile(D3d.pitch,1);
		   LogFile(D3d.yaw,1);

		   LogFile(0,0);
		   
		   //printf("%f ",vt);
		   //direction=angle+PI/2;
		   //vtx=vt*cos(direction);
		   //vty=vt*sin(direction);
		   Sleep(10);
           circle(m_hdc,x,y,r,RGB(0,0,0));
		   if(IsDead)
		   {
			   g_Dead++;
			   fclose(fp);
			   x=0;
			   y=0;
			   r=0;
			   break;
		   }

		   if(DetectDistance())
		   {
			   IsDead=1;
			   g_Dead++;
			   //printf("Crash!\n");
			   fclose(fp);
			   x=0;
			   y=0;
			   r=0;
			   break;
		   }
		   
		   if(d!=0)
		   {
		     if(d>=2000||d<=m_star->r+r)
			 {
			   //gPlanetNum--;
			   //printf("Planet:%s die! ", Name);
			   
			   //new_p=new planet(m_hdc,&m_star);
			   if(d<=m_star->r+r)
			   {
				   MessageBox(NULL,Name,"坠毁",0);
			   }
			   if(d>=2000)
			   {
                   MessageBox(NULL,Name,"飞出引力场",0);
			   }
			   IsDead=1;
			   g_Dead++;
			  // printf("Dead!\n");
			   fclose(fp);
			   x=0;
			   y=0;
			   r=0;
			   break;
			 }
		   }
		   if(d1!=0)
		   {
		     if(d1>=2000||d1<=m_star1->r+r)
			 {
			   
			   //new_p=new planet(m_hdc,&m_star);
			   IsDead=1;
			   g_Dead++;
			   //printf("Dead!\n");
			   fclose(fp);
			   x=0;
			   y=0;
			   r=0;
			   break;
			 }
		   }
		   if(d2!=0)
		   {
		     if(d2>=2000||d2<=m_star2->r+r)
			 {
			   
			  // new_p=new planet(m_hdc,&m_star);
			   IsDead=1;
			   g_Dead++;
			   //printf("Dead!\n");
			   fclose(fp);
			   x=0;
			   y=0;
			   r=0;
			   break;
			 }
		   }
		   if(IsClose)
		   {
			   //fclose(fp);
			   break;
		   }
		  // circle(m_hdc,x,y,r,RGB(0,0,0));
		   /*
		   if(lx>0)
		   {
		      if(x>m_star->x-m_star->r)
				   break;
		   }
		   else
		   {
		     if(x<m_star->x+m_star->r)
				   break;
		   }*/
		   
		}

		//printf("%lf\n",lx);
		//printf("%lf\n",ly);
		//printf("%d\n",ang);

	}
	~planet()
	{
		free(Name);
		//fclose(fp);
		if(new_p!=NULL)
		{
		  delete new_p;
		}
	//	printf("free!\n");
		g_FreeNum++;

	}

};