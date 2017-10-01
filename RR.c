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

void sort(struct process list[], int n)
{
	int i=0,j=0;
	struct process temp;
	for(i=0;i<n-1;i++)
		for(j=0;j<n-i-1;j++)
			if(list[j].AT>list[j+1].AT)
			{
				temp = list[j];
				list[j]=list[j+1];
				list[j+1]=temp;
			}
}
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
		list[i].WT=list[i].BT;
		list[i].TAT=0;
	}
	//sort(list,n);
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
	int i=0, x=0;
	while(i<(*listn))
	{
		if(list[i].AT<=ctr)
		{
			Q[*qn]=list[i];
			(*qn)++;
			moveleft(list,listn,i);
			(*listn)--;
			i=0;
		}
		else i++;
	}
}

int isempty(struct process Q[], int qn)
{
	int i=0, flag=0;
	for(;i<qn;i++)
		if(Q[i].BT==0)
			flag++;
	if(flag==qn)
		return 1;
	else return 0;
}

int GanttChart(struct process list[],struct process Q[])
{
	int listn, qn=0, ctr=0, i=0, TQ=1;
	listn = readlist(list);
	printf("\n Enter the Time Quantum:\t");
	scanf(" %d",&TQ);
	clock_t start,end;
	start = clock();
	while(listn>0 || !isempty(Q,qn))
	{
		
		processincome(list,Q,ctr,&listn,&qn);
		if(qn>0)
			i=i%qn;
		else{
			ctr+=TQ;
			continue;
		}
		if(Q[i].BT>0)
		{
			if(Q[i].BT<=TQ)
			{
				ctr+= Q[i].BT;
				Q[i].TAT = ctr - Q[i].AT;
				Q[i].WT = Q[i].TAT - Q[i].WT;
				Q[i].BT=0;
				totalTAT += Q[i].TAT;
				totalwait += Q[i].WT;
			}
			else{
				Q[i].BT -= TQ;
				ctr+= TQ;
			}
		}
		
		//printf("\n %d\t%d\t",ctr,Q[i].pid);  // Can be used to print the process in CPU at the end of each quantum
		i++;
		
	}
	end= clock();
	printf("\n PT = %lf",(double)(end-start)/CLOCKS_PER_SEC);
	return qn;
}
				
void main(int argc, char *argv[])
{
	struct process Q[100000],list[100000];
	int qn, i=0;
	//ps=atoi(argv[1]);
	qn = GanttChart(list,Q);
	
	printf("\n\nWT = %lf\t",totalwait/ps); 
	printf("\n TAT = %lf\n",totalTAT/ps);
	
}
