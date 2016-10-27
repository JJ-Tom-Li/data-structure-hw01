/*
	hw01
	系級:資工二
	學號:4104056004
	姓名:李家駿
	 
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_ROW 15 
#define MAX_COL 15 
typedef struct point{
	int x;
	int y;
	int value;
}Point;
Point stack[MAX_ROW*MAX_COL];
Point finalpath[MAX_ROW*MAX_COL];
Point nowPoint;
void stack_clear();
void push(Point);
void point0();
Point pop();
int isEmpty();
int stacksize=0;
enum direction{					//direction,0~7
	SOUTH,EAST,SEAST,NEAST,NORTH,WEST,NWEST,SWEST
};
Point direct[8]={{0,1},{1,0},{1,1},{1,-1},{0,-1},{-1,0},{-1,-1},{-1,1}};

void build_maze(int[MAX_ROW][MAX_COL]);
void show_maze(int[MAX_ROW][MAX_COL]);
void findpath(int[MAX_ROW][MAX_COL]);
int walk(int[MAX_ROW][MAX_COL]);
int outBound(int,int);
int main(int argc, char *argv[]) {
	
	/*int maze[MAX_ROW][MAX_COL]={0,1,0,2,0,1,1,0,1,0,		//test input
								1,0,0,2,1,2,1,0,2,1,
								0,1,0,1,1,2,0,2,1,0,
								1,0,2,1,0,1,2,0,2,0,
								0,2,2,0,2,1,1,0,1,0,
								0,1,1,0,2,0,2,1,2,2,
								1,2,0,1,0,1,0,1,0,0,
								0,2,0,1,1,2,1,0,1,2,
								1,0,1,0,1,2,2,0,1,2,
								2,1,2,0,2,0,1,1,2,0};*/ 
	int maze[MAX_ROW][MAX_COL];									//declaration
	int input;													//
	do
	{
		system("CLS");
		stack_clear();						//initialization
		build_maze(maze);					//
		point0();							//
		stack[0].value=maze[0][0];
		show_maze(maze);
		printf("\n");
		findpath(maze);
		
		printf("\nKey in 1 to continue,0 to terminate:");
		scanf("%d",&input);
							
	}while(input!=0);
	
	return 0;
}

void stack_clear()   //set stack empty.
{
	int i=0;
	stacksize=1;
	stack[i].x=0;
	stack[i].y=0;
	for(i=1;i<MAX_ROW*MAX_COL;i++)   
	{
		stack[i].x=-1;
		stack[i].y=-1;
	}
}

void push(Point p)
{
	stack[stacksize].x=p.x;
	stack[stacksize].y=p.y;
	stack[stacksize++].value=p.value;

}

void point0()      //set point to 0.
{
	nowPoint.x=0;
	nowPoint.y=0;
}

Point pop()
{
	//printf("pop [%d](%d,%d)\n",stacksize-1,stack[stacksize-1].x,stack[stacksize-1].y);
	return stack[--stacksize];
}

int isEmpty()
{
	if(stacksize==0) return 0;   //if returns 0 ,stack is empty.
	return 1;
}
 
void build_maze(int maze[MAX_ROW][MAX_COL])
{
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);
	int i,j;
	for(i=0;i<MAX_ROW;i++)
		for(j=0;j<MAX_COL;j++)
		{
			maze[i][j]=rand()%3;
		}
}

void show_maze(int maze[MAX_ROW][MAX_COL])
{
	int i,j;
	for(i=0;i<MAX_ROW;i++)
	{
		for(j=0;j<MAX_COL;j++)
		{
			if(maze[i][j]==-1) printf("* ");
			else printf("%d ",maze[i][j]);
		}
		printf("\n");
	}
}

void findpath(int maze[MAX_ROW][MAX_COL])
{
	int i,j,k;
//	printf("start (%d,%d)\n",nowPoint.x,nowPoint.y);
	if (maze[0][0]==1)
	{
		printf("No entry.\n");
		return ;
	}
	else if(maze[MAX_ROW-1][MAX_COL-1]==1)   //If exit==1,it's impossible to exit.
	{
		printf("No exit.\n");
		return;
	}
	
	while(isEmpty()!=0)
	{
		switch(walk(maze))
		{
			case 0:	nowPoint=pop();			
					//printf("backto (%d,%d) size=%d\n",nowPoint.x,nowPoint.y,stacksize);
					maze[nowPoint.x][nowPoint.y]=nowPoint.value;
					push(nowPoint);
					break;
			case 1: k=0;
					while(isEmpty()!=0) 
					{
						finalpath[k++]=pop();
						//printf("(%d,%d)\n",finalpath[k-1].x,finalpath[k-1].y);
					}
					printf("\n");
					break;
			default: break;
		}
	}
	
	for(i=0;i<MAX_ROW;i++)
	{
		for(j=0;j<MAX_COL;j++)
		{
			maze[i][j]=-1;
		}
	}
	
	for(i=k-1;i>=0;i--)
	{
		maze[finalpath[i].x][finalpath[i].y]=3;
	}
	if(k==0)
	{
		printf("No path.\n");
	}
	else
		show_maze(maze);
	
}

int walk(int maze[MAX_ROW][MAX_COL])     //If no way to go,return 0.If end,return 1.
{
	int i;
	if(nowPoint.x==MAX_ROW-1&&nowPoint.y==MAX_COL-1)
		return 1;
	else if(maze[nowPoint.x][nowPoint.y]==2)
	{	
		for(i=0;i<8;i++)
		{
			
			if(i!=NWEST&&i!=SWEST&&i!=NEAST&&i!=SEAST)
			if(outBound(nowPoint.x+direct[i].x,nowPoint.y+direct[i].y)!=1
				&&maze[nowPoint.x+direct[i].x][nowPoint.y+direct[i].y]!=1
				&&maze[nowPoint.x+direct[i].x][nowPoint.y+direct[i].y]!=-1)
			{
				maze[nowPoint.x][nowPoint.y]=-1;
				nowPoint.x+=direct[i].x;
				nowPoint.y+=direct[i].y;
				nowPoint.value=maze[nowPoint.x][nowPoint.y];
				push(nowPoint);
				//printf("goto (%d,%d) size=%d\n",nowPoint.x,nowPoint.y,stacksize);
				return 2;
				
			}
		}
		nowPoint.value=maze[nowPoint.x][nowPoint.y];
		maze[nowPoint.x][nowPoint.y]=-1;
		//printf("no way:");
		pop();	
		return 0;
	}
	else
	{
		for(i=0;i<8;i++)
		{
			if(outBound(nowPoint.x+direct[i].x,nowPoint.y+direct[i].y)!=1
				&&maze[nowPoint.x+direct[i].x][nowPoint.y+direct[i].y]!=1
				&&maze[nowPoint.x+direct[i].x][nowPoint.y+direct[i].y]!=-1)
			{
				maze[nowPoint.x][nowPoint.y]=-1;
				nowPoint.x+=direct[i].x;
				nowPoint.y+=direct[i].y;
				nowPoint.value=maze[nowPoint.x][nowPoint.y];
				push(nowPoint);
				//printf("goto (%d,%d) size=%d\n",nowPoint.x,nowPoint.y,stacksize);
				return 2;
			}
		}
		nowPoint.value=maze[nowPoint.x][nowPoint.y];
		maze[nowPoint.x][nowPoint.y]=-1;
	//	printf("no way:");
		pop();
		return 0;
	}

}

int outBound(int x,int y)       //If out of bound,return 1;
{
	if(y>MAX_ROW-1||x>MAX_COL-1||x<0||y<0)
		return 1;
	return 0;
}
