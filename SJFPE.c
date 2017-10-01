#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct process
{
	int pid;
	int BT;
	int AT;
	int WT;
	int TAT;
};

double totalwait=0, totalTAT=0, ps=0;

int readlist(struct process list[])
{
	int n=ps,i;
	printf("\n Enter the numberof processes\t");
	scanf(" %d", &n);
	srand(time(NULL));
	for(i=0;i<n;i++)
	{
		//printf("\n Enter %d pid:\t", i+1);
		//scanf(" %d", &list[i].pid);
		list[i].pid = i+1;
		printf("\n Enter %d Burst time:\t",i+1);
		scanf(" %d", &list[i].BT);
		//list[i].BT = (rand()%32000);
		printf("\n Enter %d Arrival time:\t",i+1);
		scanf(" %d", &list[i].AT);
		//list[i].AT = (rand()%32000);
		list[i].WT=list[i].TAT=0;
	}
	printf("PID\tBT\tAT\n");
	for(i=0;i<n;i++)
	{
		printf("\n%d\t%d\t%d\n\n",list[i].pid,list[i].BT,list[i].AT);
	}
	ps=n;
	return n;
}

void moveleft(struct process list[], int *listn, int i)
{
	int j;
	for(j=i;j<(*listn);j++)
		list[j]=list[j+1];  
		
}

void processincome(struct process list[],struct process Q[], int ctr, int *listn, int *qn)
{
	int i=0, flag=0;
	for(i=0;i<(*listn);)
	{
		
		if(list[i].AT==ctr)
		{
			Q[*qn]=list[i];
			(*qn)++;
			moveleft(list,listn,i);
			(*listn)--;
			flag=1;
		}
		else flag=0;
		i++;
		if(flag==1)
			i=0;
	}
}

void sort(struct process Q[], int qn)
{
	int i=0, j=0;
	struct process temp;
	for(;i<qn-1;i++)
		for(j=0;j<qn-1-i;j++)
			if(Q[j].BT>Q[j+1].BT)
			{
				temp=Q[j];
				Q[j]=Q[j+1];
				Q[j+1]=temp;
			}
}

void shortest(struct process Q[], int qn)
{
	int i=0, pos=i;
	struct process min , temp;
	min= Q[0];
	for(i=0;i<qn;i++)
		if(Q[i].BT<min.BT)
		{
			min = Q[i];
			pos=i;
		}
		
	temp=Q[0];
	Q[0]=Q[pos];
	Q[pos]=temp;
}

int GanttChart(struct process list[],struct process Q[])
{
	int listn, qn=0, ctr=0, i=0;
	listn = readlist(list);
	sort(list,listn);
	clock_t start,end;
	start = clock();
	while(listn>0 || qn>0)
	{
		processincome(list,Q,ctr++,&listn,&qn);
		shortest(Q,qn);
		Q[0].BT--;
		Q[0].WT++;
		printf("\n %d\t%d\t%d",Q[0].pid,Q[0].BT,Q[0].WT);
		if(Q[0].BT==0)
		{
			Q[0].TAT= ctr-Q[0].AT;
			Q[0].WT= Q[0].TAT-Q[0].WT;
			totalwait+=Q[0].WT;
			totalTAT+=Q[0].TAT;
			moveleft(Q,&qn,0);
			qn--;
		}	
	}
	end= clock();
	printf("\n PT = %lf",(double)(end-start)/CLOCKS_PER_SEC);
	return qn;
}
				
void main(int argc, char *argv[])
{
	struct process Q[100000],list[100000];
	int qn, i=0;
	//ps = atoi(argv[1]);
	
	qn = GanttChart(list,Q);
	
	printf("\n\n WT = %lf\t",totalwait/ps);
	printf("\n TAT = %lf\t",totalTAT/ps);
}
