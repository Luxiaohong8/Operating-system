#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址

/*pair包含两个数值，与容器一样，pair也是一种模板类型。但是又与之前
介绍的容器不同，在创建pair对象时，必须提供两个类型名，两个对应的类型名的类型不必相同*/
typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;//初始pid
My_algo algo[123];
int algo_len=0;
int current_algo=0;


struct free_block{	//空闲数据块
	int size;
	int start_addr;
	struct free_block *next;
};

struct allocated_block{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	int *data;
	struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id); //使用对应的分配算法
void First_fit_A();//first fit最先适应算法
void add_algorithm(int a,string name);
void init_algorithm();
void set_algorithm();
int First_fit_A(allocated_block *ab);
int Best_fit_A(allocated_block *ab);
//主函数
int main(){
	int op;
	pid = 0;
	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	init_algorithm();

	for(;;){
		sleep(1);
		display_menu();
		fflush(stdin);
		scanf("%d", &op);
		//fflush(stdin);
		switch (op){
			case 1:{ set_mem_size(); break; }
			case 2:{ set_algorithm();
					printf("current_algo %d\n", current_algo); break; }
			case 3:{ create_new_process();flag=1; break; }
			case 4:{ kill_process(); flag=1;break; }
			case 5:{ display_mem_usage(); break; }
			case 233:{ puts("bye...."); sleep(1); return 0; }
			defaut: break;
		}
	}
}

allocated_block *find_process(int id){ //循环遍历分配块链表，寻找pid=id的进程所对应的块
	allocated_block *pointer=allocated_block_head;

	while(pointer!=NULL){
		if (pointer->pid==id)
		{
			return pointer;
		}
		pointer=pointer->next;
	}
	return NULL;
}

void add_algorithm(int a,string name){
	My_algo A(a,name);
	algo[algo_len]=A;
	++algo_len;
}

void init_algorithm(){
	add_algorithm(1,"First fit");
	add_algorithm(2,"Best fit");
	current_algo=1;
}

void set_algorithm(){
	int number;
	printf("Current algorithm:\n");
	for (int i = 0; i < algo_len; ++i)
	{
		cout<<algo[i].first<<": "<<algo[i].second<<endl;
	}
	fflush(stdin);
	scanf("%d", &number);

	if (number>0&&number<=algo_len)
	{	
		current_algo=number;
	}
}

free_block *init_free_block(int mem_size){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = DEFAULT_MEM_START;
	p->next = NULL;
	return p;
}

void display_menu(){
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}

//
void set_mem_size(){ //更改最大内存大小
	int size;

	if (flag)
	{
		printf("Can't set memory size again!!!\n");
		return;
	}

	printf("Please input the Total memory size:\n");
	
	fflush(stdin);
	scanf("%d",&size);
	if (size>0)
	{
		mem_size=size;
		free_block_head->size=mem_size;
		printf("Successfully! Now the total memory is: %d",size);
	}else{
		printf("illegal input!!!\n");
	}
}

void swap(int *p, int *q){
	int tmp = *p;
	*p = *q;
	*q = tmp;
	return;
}

void rearrange(){ //将块按照地址大小进行排序
	free_block *tmp, *tmpx;
	tmp = free_block_head;
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			if (tmpx->start_addr < tmp->start_addr){
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	//puts("Rearrange Done.");
}

//
int free_mem(allocated_block *ab){ //释放某一块的内存
	free_block *pointer,*temp;
	pointer=(free_block*)malloc(sizeof(free_block));

	if (!pointer)
	{
		return -1;
	}

	pointer->size=ab->size;
	pointer->start_addr=ab->start_addr;

	pointer->next=free_block_head;
	free_block_head=pointer;

	rearrange();
	//发现连续的空闲内存块，将他们合并
	pointer=free_block_head;
	temp=pointer->next;
	while(temp!=NULL){
		if (pointer->start_addr+pointer->size==temp->start_addr)
		{
			pointer->size=pointer->size+temp->size;
			pointer->next=temp->next;
			free(temp);
		}
		pointer=pointer->next;
		temp=pointer->next;
	}
	return 1;

}

int dispose(allocated_block *fab){ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head){
		allocated_block_head = allocated_block_head->next;
		free(fab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab){ pre = ab; ab = ab->next;}
	pre->next = ab->next;
	free(ab);
	return 2;
}

void display_mem_usage(){
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	puts("*********************Free Memory*********************");
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != NULL){
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt) puts("No Free Memory");
	else printf("Totaly %d free blocks\n", cnt);
	puts("");
	puts("*******************Used Memory*********************");
	printf("%10s %20s %10s %20s\n", "PID", "ProcessName", "start_addr", "size");
	cnt = 0;
	while (ab != NULL){
		cnt++;
		printf("%10d %20s %10d %20d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
		ab = ab->next;
	}
	if (!cnt) puts("No allocated block");
	else printf("Totaly %d allocated blocks\n", cnt);
	return;
}

void kill_process(){ //杀死某个进程
	allocated_block *ab;
	int pid;
	puts("Please input the pid of Killed process");
	fflush(stdin);
	scanf("%d", &pid);
	ab = find_process(pid);
	if (ab != NULL){
		free_mem(ab);
		dispose(ab);
	}else{
		printf("NO Process!!\n");
	}
}

//
int create_new_process(){ //创建新进程
	struct allocated_block *ab;
	int size,is_success;
	ab=(struct allocated_block*)malloc(sizeof(struct allocated_block));
	if (!ab)
	{
		exit(-1);
	}

	ab->next=NULL;
	++pid;
	sprintf(ab->process_name, "procoss%02d", pid);
	ab->pid=pid;

	printf("Set Memory for %s\n", ab->process_name);
	fflush(stdin);
	scanf("%d",&size);

	if (size>0)
	{
		ab->size=size;
	}

	is_success=allocate_mem(ab);

	if (is_success>0)
	{
		printf("Allocated Successfully!!!\n");
		if (allocated_block_head==NULL)
		{	
			allocated_block_head=ab;
			rearrange();
			return 1;
		}else{
			ab->next=allocated_block_head;
			allocated_block_head=ab;
			rearrange();
			return 1;
		}		
	}else
	{
		printf("Allocated memory failure!!!\n");
		free(ab);
		return -1;
	}

}

//
int allocate_mem(allocated_block *ab){ //为块分配内存，真正的操作系统会在这里进行置换等操作
	
	int is_success;

	switch(current_algo){
		case 1: is_success=First_fit_A(ab);
			break;
		case 2: is_success=Best_fit_A(ab);
			break;
	}
	return is_success;
}

//
int First_fit_A(allocated_block *ab){
	free_block *pointer;
	int need_size=ab->size;
	pointer=free_block_head;
	while(pointer!=NULL){
		if (pointer->size>=need_size)
		{
			ab->start_addr = pointer->start_addr;
			if (pointer->size==need_size)
			{
				free(pointer);
				if (pointer== free_block_head && pointer->next == NULL)
				{
					free_block_head=NULL;
				}
				return 1;
			}else{
				pointer->start_addr=pointer->start_addr+need_size;
				pointer->size=pointer->size - need_size;
				return 1;
			}
		}
		pointer=pointer->next;
	}
	return -1;
}

int Best_fit_A(allocated_block *ab){
	free_block *pointer,*temp;
	int need_size=ab->size;
	pointer=free_block_head;
	int min=pointer->size;
	int min_having=DEFAULT_MEM_SIZE;
	while(pointer!=NULL){
		if (pointer->size>=need_size&&(min_having>=pointer->size-need_size))
		{
			min=pointer->size;
			min_having=min-need_size;
			temp=pointer;
		}
		pointer=pointer->next;
	}

	if (temp==NULL)
	{
		return -1;
	}

	ab->start_addr = temp->start_addr;
	if (min==need_size)
	{	
		if (temp== free_block_head && temp->next == NULL)
		{
			free_block_head=NULL;
		}
		free(temp);
	}else{
		temp->start_addr=temp->start_addr+need_size;
		temp->size=temp->size-need_size;
	}

	return 1;

}




