#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

int main()
{
	int num;
	int i,j,a[50][50];
	cout<<"Enter degree of matrix:- ";
	cin>>num;
	for(i=0;i<num;i++)
	{
		for(j=0;j<num;j++)
		{
			if(i==j)
				a[i][j]=0;
			else if(i>j)
				a[i][j]=a[j][i];
			else
				a[i][j]=rand()%1000;
			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}

}
