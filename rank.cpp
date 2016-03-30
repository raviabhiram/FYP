#include<iostream>

using namespace std;

int main()
{
	int q[100],rank[100];
	for(int i=0;i<100;i++)
		q[i]=9999;
	cout<<"Enter number of nodes:- ";
	int n;
	cin>>n;
	cout<<"Enter values:-\n";
	for(int i=0;i<n;i++)
		cin>>q[i];
	int j=0;
	while(j<n)
	{
		int max=0;
		int u;
		for(int i=0;i<n;i++)
		{
			if(q[i]>=max)
			{
				max=q[i];
				u=i;
			}
		}
		q[u]=0;
		rank[j++]=u;
	}
	for(int i=0;i<n;i++)
		cout<<"Rank "<<i<<":- "<<rank[i]<<endl;
	return 0;
}