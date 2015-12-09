#include <stdio.h> 
#include <malloc.h> 
#include <math.h> 
#include <stdlib.h>  
int Cks=4; // 银行办理业务的窗口数,默认值为:4；最大值不超过20； 
int Qu;   // 客户队列数Qu=Cks  
int Khjg=5;  // 两相邻到达客户的时间间隔最大值,默认值为:5 
int Blsj=30; // 每个客户办理业务的时间最大值,默认值为:30 
  
typedef struct{     
int OccurTime;     
int NType; // 事件类型，Qu表示到达事件,0至Qu-1表示Qu个窗口的离开事件   
}Event,ElemType;   
 
typedef struct LNode{    
ElemType data;   
struct LNode *next;  
}LNode, *LinkList; 
 
typedef LinkList EventList; // 事件链表类型，定义为有序链表   

typedef struct{     
int ArrivalTime;     
int Duration;   
}QElemType;  
  
typedef struct QNode{    
QElemType data;  
struct QNode *next;  
} QNode, *Queue;    
typedef  struct { Queue head; Queue tail;} LinkQueue;
   
LinkQueue *q; // Qu个客户队列 
EventList ev;  
Event en;  
Event et;   
QElemType customer;   
int TotalTime=0,CustomerNum=0; // 累计客户逗留时间,客户数(初值为0) 
int CloseTime;  // 银行营业时间(单位是分)
   
void InitList(EventList &L){   
	L=(EventList)malloc(sizeof(LNode));  
	L->next=NULL; 
}   

int ListLength(EventList L){//求事件表的长度 
	int i=0;
	EventList p;  
	p=ev->next;   
	while(p) {p=p->next;i++;}  
	return i; 
}   

int ListEmpty(EventList ev){//判事件表是否空  
	return(ev->next==NULL); 
}   

void Gethead(EventList ev,Event &en){   
	EventList q;   
	q=ev->next;   
	en=q->data;      
	ev->next=q->next;  
	free(q); 
}   

void InitQueue( LinkQueue &Q){
   	Q.head=Q.tail=(Queue)malloc(sizeof(QNode));  
	Q.head->next=NULL; 
}  
 
int cmp(Event a, Event b){// 依事件a的发生时刻<、=或>事件b的发生时刻分别返回-1、0或1    
	if(a.OccurTime==b.OccurTime)      
	return 0;    
	else       
	return (a.OccurTime-b.OccurTime)/abs(a.OccurTime-b.OccurTime);  
}  

void Insert_EventList(EventList &L, Event a){//将事件a插入到有序的事件链表中，保持有序性不变 
	EventList p=L,q;   
	while(p->next&& cmp(p->next->data, a)<0) p=p->next;     
	q=(EventList)malloc(sizeof(LNode));  
	q->data=a;   
	q->next=p->next;     
	p->next=q; 
}   

void EnQueue(LinkQueue &Q ,QElemType a){//将一个客户a插入到队列Q中  
	Queue p;   
	p=(Queue)malloc(sizeof(QNode)); //产生客户结点  
	p->data=a;  p->next=NULL;  
	Q.tail->next=p;  
	Q.tail=p; 
}  
  
void  DelQueue(LinkQueue &Q ,QElemType &a){   
	Queue p;   
	a=Q.head->next->data;     
	p=Q.head->next;      
	Q.head->next=Q.head->next->next;  
	if(Q.tail==p) Q.tail=Q.head;  
	free(p); 
}  
 
int QueueEmpty(LinkQueue Q ){//判队列空  
  return(Q.head==Q.tail);
}   

void Random(int &d,int &i){ //产生两个随机数       
	d=rand()%Blsj+1;     
	i=rand()%Khjg+1;  
} 
  
int QueueLength(LinkQueue Q){//求队列的长度 
	int i=0;
    Queue p;
  	p=Q.head->next;
  	while(p) {p=p->next;i++;}  return i; //返回队列的长度 
}   

void GetHead_q(LinkQueue &Q,QElemType &a){//从非空队列中取一个元素 
	a=Q.head->next->data; 
}

int Minimum(LinkQueue Q[]){ // 返回最短队列的序号  
     int l[20];
    int i,k;
     for(i=1;i<Qu+1;i++)
       l[i]=QueueLength(Q[i]);
    k=1;
     for(i=2;i<Qu+1;i++)
      if(l[i]<l[k])
      { 
        k=i;
      }
     return k;  
}   

void OpenForDay(){ //模拟银行开门的动作，即初始化操作    
    int i;
    InitList(ev); // 初始化事件链表为空     
	en.OccurTime=0; // 设定第一个客户到达事件    
	en.NType=0; //  客户到达事件     
	Insert_EventList(ev, en);//插入事件     
	q=(LinkQueue*)malloc((Qu+1)*sizeof(LinkQueue));//为队列申请Qu+1个队头指针，第0个不用     
	for(i=1;i<Qu+1;++i) // 置空队列     
	InitQueue(q[i]);  
}   

void CustomerArrived(){
	QElemType f;     
	int durtime,intertime,i;   
 	++CustomerNum;     
	Random(durtime,intertime); // 生成随机数    
	//printf("%d  %d\n",durtime,intertime);     
	et.OccurTime=en.OccurTime+intertime; // 下一客户到达时刻    
	et.NType=0; //       
	//printf("%d  %d\n",et.NType,et.OccurTime);    
	if(et.OccurTime<CloseTime)      
	Insert_EventList(ev,et);    
	i=Minimum(q);      
	f.ArrivalTime=en.OccurTime;    
	f.Duration=durtime;    
	EnQueue(q[i],f);     
	if(QueueLength(q[i])==1){       
		et.OccurTime=en.OccurTime+durtime;      
		et.NType=i;       
		Insert_EventList(ev,et); // 设定第i队列的一个离开事件并插入事件表    
	}  
}   
void CustomerDeparture(){ // 处理客户离开事件，en.NTyPe!=0    
	int i;     
	i=en.NType;     
	DelQueue(q[i],customer); // 删除第i队列的排头客户    
 	TotalTime+=en.OccurTime-customer.ArrivalTime; // 累计客户逗留时间    
	if(!QueueEmpty(q[i])){ // 设定第i队列的一个离开事件并插入事件表      
		GetHead_q(q[i],customer);       
		et.OccurTime=en.OccurTime+customer.Duration;      
		et.NType=i;       
		Insert_EventList(ev,et);    
	}  
}   

void output_ev(EventList L){   
 	EventList p;   
	p=L->next;    
	while(p){ printf("(%d ,%d)->",p->data.NType,p->data.OccurTime);p=p->next;}   
 	printf("\n"); 
}  

void output_q(LinkQueue Q){//输出队列中的元素，看各队列中元素的变化情况    
	Queue p;   
	p=Q.head->next;   
	while(p){ printf("(%d ,%d)->",p->data.ArrivalTime,p->data.Duration);p=p->next;}
    printf("\n"); 
}   

void Bank_Simulation()  
{
	OpenForDay();      
	while(!ListEmpty(ev)){    
		Gethead(ev,en); //printf("事件%d %d\n ",en.NType,en.OccurTime);       
		if(en.NType==0)CustomerArrived(); // 处理客户到达事件      
		else         
		CustomerDeparture(); // 处理客户离开事件    
	} // 计算并输出平均逗留时间    
 	printf("顾客总数:%d, 所有顾客共耗时:%d分钟, 平均每人耗时: %d分钟\n",CustomerNum,TotalTime,TotalTime/CustomerNum);  
}   
void Bank(){     
	printf("请输入银行营业时间长度(单位:分)CloseTime=");    
	scanf("%d",&CloseTime);     
	printf("请输入银行办理业务所开窗口数Cks=");    
	scanf("%d",&Cks);     
 	printf("请输入客户办理业务的最长时间Blsj=");    
	scanf("%d",&Blsj);      
	printf("请输入两个相邻客户到达银行的时间间隔的最大值Khjg=");    
	scanf("%d",&Khjg);     
	Qu=Cks; //给Qu赋值为窗口数Cks    
	Bank_Simulation(); //模拟银行业务  
}   
int main()  
{     
	Bank();  
	return 0;
}
