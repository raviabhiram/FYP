#include<iostream>
#include<stdlib.h>
#include<stdio.h>

#define MAX 100//maximum number of nodes that can be created
#define INF 9999//virtual value of infinity
#define interval 0.01//the interval at which packets are created
#define delay 0.005//propagation delay over the links
#define pktSize 1024//packet size used to compute transmition delay

using namespace std;

double am[MAX][MAX],timeElapsed=0,totTime,pStartTime;
int population,tfront=-1,trear=-1;
int s0,i0,r0,sn,in,rn;
double k1,k2,k3,k4,i1,i2,i3,i4,g1,g2,g3,g4;
double alpha=0.00005,beta=0.1;
int icount=0,scount=0,rcount=0;
int startFlag=0,computeParamFlag=0;
int buffList[MAX],rank[MAX];

class packet//we will be creating objects of this class for our packets
{
	public:
	int pktId,srcNode,currNode,nextHop,des;
	packet();
	packet(int pktId,int srcNode,int currNode,int nextHop,int des);
};

packet::packet()//default constructor
{
}

packet::packet(int pktId,int srcNode,int currNode,int nextHop,int des)//packet constructor to set values to packets
{
	this->pktId=pktId;//every packet has a unique ID to identify it
	this->srcNode=srcNode;//the source at which the packet is created
	this->currNode=currNode;//the node at which the packet is currently at
	this->nextHop=nextHop;//the next hop the packet needs to go to in order to reach the destination
	this->des=des;//the eventual destination of the packet
}

packet *tempQ[MAX];

struct path//the node propertires
{
	int dist[MAX],nextHop[MAX],buffSize,buffAvailable;
	int pktId,destination;
	bool isSource;
	packet *q[MAX];//the queue which stores the packets. Buffer in real life terms
	int rear,front;//queue iterators
	char state;//used to store the state of the node (S,I,R)
	void push(packet *p, int isOld)//take a packet as parameter to push into the queue
	{
		if(p->currNode==p->des)
		{
			cout<<"r\t"<<timeElapsed<<"\t"<<p->pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet enqueue statement
			return;
		}
		if(buffAvailable == 0)
		{
			cout<<"d\t"<<timeElapsed<<"\t"<<p->pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet drop statement
			if(startFlag==0)
			{
				startFlag=1;
				pStartTime=timeElapsed;
				s0=scount;
				i0=icount;
				r0=rcount;
			}
		}
		else//if queue isn't full, add the packet to the queue
		{
			if(isOld)
			{
				rear=(rear+1)%buffSize;
				q[rear]=p;
				buffAvailable--;
				cout<<"+\t"<<timeElapsed<<"\t"<<p->pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet enqueue statement
			}
			else
			{
				rear=(rear+1)%buffSize;
				q[rear]=p;
				buffAvailable--;
				cout<<"a\t"<<timeElapsed<<"\t"<<p->pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet enqueue statement
			}
		}
	}
	packet *pop()//return a packet if the queue is not empty
	{
		if((buffAvailable==buffSize))
		{
			return NULL;//queue is empty, no packets to send. Do nothing
		}
		else
		{
			front=(front+1)%buffSize;
			buffAvailable++;
			packet *p=q[front];
			cout<<"-\t"<<timeElapsed<<"\t"<<p->pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet dequeue statement
			return q[front];
		}
	}
}nodes[MAX];

void printPath(int src,int des)//traverse using next hop of each node for a particular source,destination pair
{
	int i=src;
	do
	{
		cout<<i<<"-->";
		i=nodes[i].nextHop[des];
	}while(i!=des);
	cout<<des<<endl;
}

void findPath()//implementation of Bellman-Ford algorithm
{
	int i,j,k,stopFlag=0;
	do{
		stopFlag=0;
		for(i=0;i<population;i++)
			for(j=0;j<population;j++)
				if(i!=j)
					for(k=0;k<population;k++)
					{
						if(nodes[i].dist[j]>nodes[i].dist[k]+nodes[k].dist[j] && nodes[k].state!='I')
						{
							nodes[i].dist[j]=nodes[i].dist[k]+nodes[k].dist[j];
							nodes[i].nextHop[j]=k;
							stopFlag++;
						}
					}
	}while(stopFlag!=0);
}

void tpush(packet *p)
{
	trear=(trear+1)%MAX;
	tempQ[trear]=p;
}

void computeParams()
{
	double ds,di,dt;
	ds=(double)s0-(double)sn;
	di=(double)i0-(double)in;
	dt=timeElapsed-pStartTime;
	alpha=ds/(dt*(double)icount*(double)scount);
	beta=((alpha*(double)icount*(double)scount)-(di/dt))/(double)icount;
}

void rankNodes()
{
	int i,j=0,u,max;
	for(i=0;i<population;i++)
	{
		buffList[i]=nodes[i].buffAvailable;
	}
	while(j<population)
	{
		max=0;
		for(i=0;i<population;i++)
		{
			if(buffList[i]>=max)
			{
				max=buffList[i];
				u=i;
			}
		}
		buffList[u]=0;
		rank[j++]=u;
	}
}

void updateState()
{
	int i=0;
	while(i<in)
	{
		nodes[rank[i++]].state='I';
	}
	while(i-in<sn)
	{
		nodes[rank[i++]].state='S';
	}
	while(i<population)
	{
		nodes[rank[i++]].state='R';
	}
}

double s(double x,double y,double z)
{
	return -(alpha*x*y);
}

double i(double x,double y,double z)
{
	return (alpha*x*y)-(beta*y);
}

double r(double x,double y,double z)
{
	return (beta*y);
}

void rungeKutta()
{
	k1=timeElapsed*s(s0,i0,r0);
	i1=timeElapsed*i(s0,i0,r0);
	g1=timeElapsed*r(s0,i0,r0);
	k2=timeElapsed*s(s0+(k1/2),i0+(i1/2),r0+(g1/2));
	i2=timeElapsed*i(s0+(k1/2),i0+(i1/2),r0+(g1/2));
	g2=timeElapsed*r(s0+(k1/2),i0+(i1/2),r0+(g1/2));
	k3=timeElapsed*s(s0+(k2/2),i0+(i2/2),r0+(g2/2));
	i3=timeElapsed*i(s0+(k2/2),i0+(i2/2),r0+(g2/2));
	g3=timeElapsed*r(s0+(k2/2),i0+(i2/2),r0+(g2/2));
	k4=timeElapsed*s(s0+(k3/2),i0+(i3/2),r0+(g3/2));
	i4=timeElapsed*i(s0+(k3/2),i0+(i3/2),r0+(g3/2));
	g4=timeElapsed*r(s0+(k3/2),i0+(i3/2),r0+(g3/2));
	sn=s0+(k1 + (2*k2) + (2*k3) + k4)/6;
	in=i0+(i1 + (2*i2) + (2*i3) + i4)/6;
	rn=r0+(g1 + (2*g2) + (2*g3) + g4)/6;
	if(sn>population)
		sn=population;
	if(in>population)
		in=population;
	if(rn>population)
		rn=population;
	if(sn<0)
		sn=0;
	if(in<0)
		in=0;
	if(rn<0)
		rn=0;
	rankNodes();
	updateState();
}

void computeState()
{
	int i=0;
	for(i=0;i<population;i++)
	{
		if(((double)nodes[i].buffAvailable/(double)nodes[i].buffSize) > 0.4 && nodes[i].state == 'I')
		{
			icount--;
			nodes[i].state='R';
			rcount++;
		}
		if(((double)nodes[i].buffAvailable/(double)nodes[i].buffSize) > 0.2 && ((double)nodes[i].buffAvailable/(double)nodes[i].buffSize) < 0.4 && nodes[i].state == 'R')
		{
			rcount--;
			nodes[i].state='S';
			scount++;
		}
		else if(((double)nodes[i].buffAvailable/(double)nodes[i].buffSize) < 0.2 && nodes[i].state == 'S')
		{
			scount--;
			nodes[i].state='I';
			icount++;
		}
	}
	if(!computeParamFlag && timeElapsed-pStartTime>=5 && icount>0)
	{
		computeParamFlag=1;
		computeParams();
	}
}

void sendData()
{	
	int flag=0;//used to note if a new packet is to be created.
	while(timeElapsed<totTime)//simulation needs to run for a particular time interval.
	{
		while(tfront!=trear)
		{
			tfront=(tfront+1)%MAX;
			packet *p=tempQ[tfront];
			p->currNode=p->nextHop;//send the packet to the next node in the path.
			p->nextHop=nodes[p->currNode].nextHop[nodes[p->srcNode].destination];//update the next hop based on the path.
			nodes[p->currNode].push(p,1);//push the packet to the next node.
		}
		for(int i=0;i<population;i++)//at every point in time, all nodes need to be checked to send packets
		{
			findPath();
			if(nodes[i].isSource)//if it is a source node, create a new packet
			{
				if(!flag && timeElapsed>0)//data rate needs to be checked to create packets
				{
					nodes[i].pktId++;//increment packet id for the next packet
					packet *p=new packet(nodes[i].pktId,i,i,nodes[i].nextHop[nodes[i].destination],nodes[i].destination);
					nodes[i].push(p,0);//push the packet into the queue of the source
				}
				packet *p=nodes[i].pop();//check the queue to send the next packet available
				if(p!=NULL && p->currNode!=p->des)
				{
					tpush(p);
				}
			}
			else//if the node is not a source node just send packets in the queue
			{
				if(flag && timeElapsed>0)
				{
					packet *p=nodes[i].pop();
					if(p!=NULL && p->currNode!=p->des)//send the packet only if the current node isn't the destination
					{
						tpush(p);
					}
				}
			}
		}
		if(computeParamFlag)//predictive way to mark the state of nodes.
		{
			rungeKutta();
		}
		else//initial way to mark the state of nodes.
		{
			computeState();
		}
		timeElapsed+=0.005;//update time elapsed
		if(flag)
			flag=0;
		else
			flag=1;
	}
}

int main(int argc,char* argv[])
{
	int i,j,nsrc,srcCalc,desCalc;
	cout<<"Enter the population of the system:- ";
	cin>>population;
	cout<<"Enter the total time of simulation:- ";
	cin>>totTime;
	cout<<"Enter the distance matrix:-\n";
	scount=0;
	for(i=0;i<population;i++)
	{
		nodes[i].isSource=false;//initialise all nodes as intermediates and not source
		nodes[i].pktId=0;
		nodes[i].state='S';
		scount++;
		cout<<scount<<endl;
		nodes[i].rear=-1;//used as an iterator to pop packets from queues
		nodes[i].front=-1;//used as an iterator to push packets into the queue
		for(j=0;j<population;j++)
		{
			cin>>am[i][j];//input the distance from node i to node j
			if(am[i][j])//if there is a direct path from node i to node j
			{
				nodes[i].dist[j]=am[i][j];//set the distance from i to j to whatever was input
				nodes[i].nextHop[j]=j;//all nodes are 1 hop away from the given node
			}
			else//if there is no direct path from node i to node j
			{
				nodes[i].dist[j]=INF;
				nodes[i].nextHop[j]=i;
			}
		}
	}
	for(i=0;i<MAX;i++)
	{
		buffList[i]=9999;
	}
	cout<<"Enter buffer size of each node:- \n";
	for(i=0;i<population;i++)
	{
		cout<<i+1<<" = ";
		cin>>nodes[i].buffSize;//input the queue size of each node
		nodes[i].buffAvailable=nodes[i].buffSize;//initialize buffer available of each node as the queue size
	}
	cout<<"Enter number of source nodes:- ";
	cin>>nsrc;
	for(i=0;i<nsrc;i++)
	{
		do{
			srcCalc=rand()%(population);//set a random source. endPts stores source,destination pairs
		}while(nodes[srcCalc].isSource);//one node cannot be multiple sources
		do{
			desCalc=rand()%(population);
		}while(srcCalc==desCalc);//source and destination cannot be the same node
		nodes[srcCalc].isSource=true;//set the flag to show that a node is the source
		nodes[srcCalc].destination=desCalc;//destination for the particular source is set
		cout<<"Source: "<<srcCalc<<" Destination: "<<desCalc<<endl;
	}
	cout<<"Action\ttime\tpktId\tSrc\tDes\tCur\tNH\tState\n";
	sendData();//this function actually sends the packets and controls the simulation
	return 0;
}
