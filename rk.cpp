#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

using namespace std;

double tt,sc,ic,rc,s0,i0,r0,k1,k2,k3,k4,i1,i2,i3,i4,g1,g2,g3,g4,l,u,b,a,sarr[100],iarr[100],rarr[100];

double s(double x,double y,double z)
{
	return -(a*x*y);
}
double i(double x,double y,double z)
{
	return (a*x*y)-(b*y);
}
double r(double x,double y,double z)
{
	return (b*y);
}
int main()
{
	double t;
	int ct;
	FILE * fx;
	fx=fopen("rk.xg","w");
	fclose(fx);
	fx=fopen("rk.xg","a");
	cout<<"Enter S0:- ";
	cin>>s0;
	cout<<"Enter I0:- ";
	cin>>i0;
	cout<<"Enter R0:- ";
	cin>>r0;
//	cout<<"Enter the time duration:- ";
//	cin>>tt;
	int total=s0+i0+r0;
//	cout<<"Enter the birth rate and death rate:- ";
//	cin>>l>>u;
	cout<<"Enter alpha:- ";
	cin>>a;
	cout<<"Enter beta:- ";
	cin>>b;
	t=0.08;
	ct=1;
	do
	{
		k1=t*s(s0,i0,r0);
		i1=t*i(s0,i0,r0);
		g1=t*r(s0,i0,r0);
		k2=t*s(s0+(k1/2),i0+(i1/2),r0+(g1/2));
		i2=t*i(s0+(k1/2),i0+(i1/2),r0+(g1/2));
		g2=t*r(s0+(k1/2),i0+(i1/2),r0+(g1/2));
		k3=t*s(s0+(k2/2),i0+(i2/2),r0+(g2/2));
		i3=t*i(s0+(k2/2),i0+(i2/2),r0+(g2/2));
		g3=t*r(s0+(k2/2),i0+(i2/2),r0+(g2/2));
		k4=t*s(s0+(k3/2),i0+(i3/2),r0+(g3/2));
		i4=t*i(s0+(k3/2),i0+(i3/2),r0+(g3/2));
		g4=t*r(s0+(k3/2),i0+(i3/2),r0+(g3/2));
		sc=s0+(k1 + (2*k2) + (2*k3) + k4)/6;
		ic=i0+(i1 + (2*i2) + (2*i3) + i4)/6;
		rc=r0+(g1 + (2*g2) + (2*g3) + g4)/6;
		cout<<"S = "<<sc/total<<endl;
		cout<<"I = "<<ic/total<<endl;
		cout<<"R = "<<rc/total<<endl;
		sarr[ct]=sc/total;
		iarr[ct]=ic/total;
		rarr[ct]=rc/total;
		t+=0.005;
		ct++;
	}while(ic>=0 && sc>=0 && rc>=0);
	tt=t;
	fprintf(fx,"\n\"S\"\n");
	for(int t=1;t<tt;t++)
		fprintf(fx,"%d\t%f\n",t,sarr[t]);
	fprintf(fx,"\n\"I\"\n");
	for(int t=1;t<tt;t++)
		fprintf(fx,"%d\t%f\n",t,iarr[t]);
	fprintf(fx,"\n\"R\"\n");
	for(int t=1;t<tt;t++)
		fprintf(fx,"%d\t%f\n",t,rarr[t]);
	return 0;
}
