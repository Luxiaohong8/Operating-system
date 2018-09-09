#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存已经被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
My_algo algo[3];
int choice = 1; //当前选择的算法

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
void rearrange(int choice); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(); //使用对应的分配算法

//主函数
int main(){
	int op;
	pid = 0;
	algo[0] = make_pair(1,"First_Fit");
	algo[1] = make_pair(2,"Best_Fit");
	algo[2] = make_pair(3,"Worst_Fit");

	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	for(;;){
		sleep(1);
		display_menu();
		fflush(stdin);
		scanf("%d", &op);
		switch (op){
			case 1:{ set_mem_size(); break; }
			case 2:{ Usemy_algo();flag = 1;	break; }
			case 3:{ create_new_process(); flag = 1;break; }
			case 4:{ kill_process(); flag = 1;break; }
			case 5:{ display_mem_usage(); flag = 1;break; }
			case 233:{ puts("bye...."); sleep(1); return 0; }
			defaut: break;
		}
	}
}

void Usemy_algo(){
	for(int i = 0; i < 3; i++)
		printf("%d: %s\n",algo[i].first,algo[i].second.c_str());
	printf("DEFAULT: First Fit.\nYou can enter another choice:");
	int tmp; 
	scanf("%d",&tmp);
	if(tmp == 1 || tmp == 2 || tmp == 3){
		choice = tmp;			
	}	
	switch(choice){
		case 1:
			puts("Rearrange free blocks for First Fit");
			break;
		case 2:
			puts("Rearrange free blocks for Best Fit");
			break;
		case 3:
			puts("Rearrange free blocks for Worst Fit");
			break;
		default: break;
	}
}

allocated_block *find_process(int id){ //循环遍历分配块链表，寻找pid=id的进程所对应的块
/////////
	if(allocated_block_head == NULL)
	 	return NULL;
	allocated_block *tmp;
	tmp = allocated_block_head;	
	while(tmp != NULL){
		if(tmp->pid == id)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
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

void set_mem_size(){ //更改最大内存大小
	if(flag == false){
		puts("Memory size has been changed before.");
	}
	printf("Enter the new memory size:\n");
	int tmp_size;
	scanf("%d", &tmp_size);getchar();
	if(tmp_size < 0){
		puts("Invalid memory size!");
		return ;
	}
	mem_size = tmp_size;
	free_block_head->size = mem_size;
}

int allocate_mem(allocated_block *ab){ //为块分配内存，真正的操作系统会在这里进行置换等操作
//////////////
	free_block *p, *px;
	p = px = free_block_head;
	while(p != NULL){
		if(p->size > ab->size){
			ab->start_addr = p->start_addr;
			p->start_addr += ab->size;
			p->size -= ab->size;
			return 1;
		} else if(p->size == ab->size){
			ab->start_addr = p->start_addr;
			if(px == p){
				free_block_head = px->next;
			}
			else{	
				px->next = p->next;
				p = p->next;
  			}
			return 1;	
		} else{
			p = p->next;
		}
	}
	return -1;
}

int create_new_process(){ //创建新进程
//////////
	allocated_block *ab;
	ab = (allocated_block *)malloc(sizeof(allocated_block));

	if (!ab){
		puts("Malloc fail");	
		return 0;
	}
	ab->next = NULL;
	pid = pid+1;
	sprintf(ab->process_name, "process_%d", pid); 
	ab->pid = pid;	
	printf("Enter the size of the block %s:\n", ab->process_name);
	int size;
	scanf("%d", &size);
	getchar();
	if(size < 0){
		puts("Invalid size");
		return -1;
	}
	ab->size = size;
	int data = allocate_mem(ab);
	if(data == 1 && allocated_block_head == NULL){
		allocated_block_head = ab;
		rearrange(choice);
		return 1;	
	} else if (data == 1){
		rearrange(choice);
		ab->next = allocated_block_head;
		allocated_block_head = ab;
		return 2;
	} else{
		free(ab);
		puts("Fail to allocate");
		return -1;
	}
}

void swap(int *p, int *q){
	int *tmp;
	tmp = p;
	p = q;
	q = tmp;
	return;
}

void rearrange(int choice){ //将块按照地址大小进行排序
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	bool judge;
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			switch(choice){
				case 1:
					judge = tmpx->start_addr < tmp->start_addr;
					break;
				case 2:
					judge = tmpx->size < tmp->size;
					break;
				case 3:
					judge = tmpx->size > tmp->size;
					break;
			}
			if(judge){
				swap(&tmpx->start_addr, &tmp->start_addr);
				swap(&tmpx->size, &tmp->size);
			}
			else
	 			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	puts("Rearrange Done.");
}


int free_mem(allocated_block *ab){ //释放某一块的内存
//////////////////
	free_block *fb, *fbb;
	
	fb = (free_block *)malloc(sizeof(free_block));
	if(!fb){
		puts("Malloc fail");
		return -1;	
	}
	fb->size = ab->size;
	fb->start_addr = ab->start_addr;
	fb->next = free_block_head;
	free_block_head = fb;
	rearrange(1);
	fb = free_block_head;
	while(fb != NULL){
		fbb = fb->next;
		if(fbb != NULL){
			if(fb->size + fb->start_addr == fbb->start_addr){
				fb->size += fbb->size;
				fb->next = fbb->next;
				free(fbb);continue;	
			}
		}
		fb = fb->next;
	}
	rearrange(choice);
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
	scanf("%d", &pid);
	ab = find_process(pid);
	if (ab != NULL){
		free_mem(ab);
		dispose(ab);
	}
}



