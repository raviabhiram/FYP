#include<iostream>
#include<stdlib.h>

#define MAX 100//maximum number of nodes that can be created
#define INF 9999//virtual value of infinity
#define interval 0.01//the interval at which packets are created
#define delay 0.005//propagation delay over the links
#define pktSize 1024//packet size used to compute transmition delay

using namespace std;

double am[MAX][MAX],timeElapsed=0,totTime;
int population,tfront=-1,trear=-1;
int dropCount=0,pktCount=0,rcvCount=0;
int s0,i0,r0,sn,in,rn;
int icount=0,scount=0,rcount=0;

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
		rear=(rear+1)%buffSize;
		if(isOld && (rear==front))//if the buffer is full, start dropping packets
		{
			cout<<"d\t"<<timeElapsed<<"\t"<<pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet drop statement
			dropCount++;
			state='I';
		}
		else//if queue isn't full, add the packet to the queue
		{
			q[rear]=p;
			buffAvailable--;
			if(isOld && p->currNode != p->des)
				cout<<"+\t"<<timeElapsed<<"\t"<<pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet enqueue statement
			else
			{
				cout<<"a\t"<<timeElapsed<<"\t"<<pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet enqueue statement
			}
		}
		if(((double)buffAvailable/(double)buffSize)>0.8 && state!='I')
		{
			if(state=='R')
				rcount--;
			state='S';
			scount++;
		}
		else if(buffAvailable==0)
		{
			state='I';
			icount++;
		}
	}
	packet *pop()//return a packet if the queue is not empty
	{
		if((front==rear))
		{
			return NULL;//queue is empty, no packets to send. Do nothing
		}
		else
		{
			front=(front+1)%buffSize;
			buffAvailable++;
			if(((double)buffAvailable/(double)buffSize)<0.6)
			{
				if(state=='S')
					scount--;
				else if(state=='I')
					icount--;
				state='R';
				rcount++;
			}
			packet *p=q[front];
			if(p->currNode!=p->des)
				cout<<"-\t"<<timeElapsed<<"\t"<<pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet dequeue statement
			else
			{
				cout<<"r\t"<<timeElapsed<<"\t"<<pktId<<"\t"<<p->srcNode<<"\t"<<p->des<<"\t"<<p->currNode<<"\t"<<p->nextHop<<"\t"<<state<<endl;//packet enqueue statement
				rcvCount++;
			}
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
//	}while(nodes[i].nextHop[des]!=des);
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
						if(nodes[i].dist[j]>nodes[i].dist[k]+nodes[k].dist[j])
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

void sendData()
{	
	int flag=0;//used to note if a new packet is to be created
	while(timeElapsed<totTime)//simulation needs to run for a particular time interval
	{
		while(tfront!=trear)
		{
			tfront=(tfront+1)%MAX;
			packet *p=tempQ[tfront];
			p->currNode=p->nextHop;//send the packet to the next node in the path
			p->nextHop=nodes[p->currNode].nextHop[nodes[p->srcNode].destination];//update the next hop based on the path
			nodes[p->currNode].push(p,1);//push the packet to the next node
		}
		for(int i=0;i<population;i++)//at every point in time, all nodes need to be checked to send packets
		{
			if(nodes[i].isSource)//if it is a source node, create a new packet
			{
				if(!flag && timeElapsed>0)//data rate needs to be checked to create packets
				{
					packet *p=new packet(nodes[i].pktId,i,i,nodes[i].nextHop[nodes[i].destination],nodes[i].destination);
					nodes[i].pktId++;//increment packet id for the next packet
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
		timeElapsed+=0.005;//update time elapsed
		if(flag)
			flag=0;
		else
			flag=1;
	}
}

void throughput()
{
	double thrpt=(double)rcvCount/(double)pktCount;
	cout<<"Throughput:- "<<thrpt<<endl;
	return;
}

int main(int argc,char* argv[])
{
	int i,j,nsrc,srcCalc,desCalc;
	cout<<"Enter the population of the system:- ";
	cin>>population;
	cout<<"Enter the total time of simulation:- ";
	cin>>totTime;
	cout<<"Enter the distance matrix:-\n";
	for(i=0;i<population;i++)
	{
		nodes[i].isSource=false;//initialise all nodes as intermediates and not source
		nodes[i].pktId=0;
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
	cout<<"Enter buffer size of each node:- \n";
	for(i=0;i<population;i++)
	{
		cout<<i+1<<" = ";
		cin>>nodes[i].buffSize;//input the queue size of each node
		nodes[i].buffAvailable=nodes[i].buffSize;//initialize buffer available of each node as the queue size
		nodes[i].state='S';//all nodes are initially in SUSCEPTIBLE state
	}
	cout<<"Enter number of source nodes:- ";
	cin>>nsrc;
/*	if(nsrc>5)//restrict the number of source nodes to 5 for simplicity
		nsrc=5;
*/	for(i=0;i<nsrc;i++)
	{
		do{
			srcCalc=rand()%(population-i);//set a random source.
		}while(nodes[srcCalc].isSource);//one node cannot be multiple sources
		do{
			desCalc=rand()%(population-i);
		}while(srcCalc==desCalc);//source and destination cannot be the same node
		nodes[srcCalc].isSource=true;//set the flag to show that a node is the source
		nodes[srcCalc].destination=desCalc;//destination for the particular source is set
		cout<<"Source: "<<srcCalc<<" Destination: "<<desCalc<<endl;
	}
	findPath();//call this function to find all pairs shortest path. AODV is based on this (Bellman-Ford)
	cout<<"The routing tables are:-\n";
	for(i=0;i<population;i++)
	{
		cout<<"Routing table for node "<<i<<endl;
		cout<<"Dest\tNH\tDist\n";
		for(j=0;j<population;j++)
		{
			if(i!=j)
			{
				cout<<j<<"\t"<<nodes[i].nextHop[j]<<"\t"<<nodes[i].dist[j]<<endl;
			}
		}
	}
	for(i=0;i<population;i++)
	{
		if(nodes[i].isSource)
			printPath(i,nodes[i].destination);//print the path for each source,destination pair
	}
	cout<<"Action\ttime\tpktId\tSrc\tDes\tCur\tNH\tState\n";
	sendData();//this function actually sends the packets and controls the simulation
	pktCount=dropCount+rcvCount;
	cout<<"Total packets sent:- "<<pktCount<<endl;
	cout<<"Total packets dropped:- "<<dropCount<<endl;
	cout<<"Total packets received:- "<<rcvCount<<endl;
	throughput();
	return 0;
}
