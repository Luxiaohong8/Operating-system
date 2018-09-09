#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;
#define FILENAME_LEN 20
#define COMMAND_LEN 10
#define INPUT_LEN 100
#define PATH_LEN 80
#define COMD_NUM 18
#define DIR_DEEPTH 20
#define USER_NUM 10

//char comdname_all[][COMMAND_LEN] = {"exit", "cd", "create", "delete", "ls", "mkdir", "dedir"};
char comdname_all[][COMMAND_LEN] = {"mkdir","ls","lsp","cd","cdp","pwd","mkdirp",\
            "exit","regusr","disusr","delusr","login","create","createp","open","chmod",\
            "chmodp","lsfile"};
int comdname_len[]={2,1,2,2,2,1,3,1,3,1,2,2,3,4,2,3,4,1,4,5,2};
typedef enum {FALSE,TRUE} Bool;

typedef struct DNode{
    char filename[FILENAME_LEN];
    DNode *parent,*Dchild;
    struct FNode *Fchild;
    DNode *left,*right;
    char path[PATH_LEN];
}DNode;

typedef struct FNode{
    char filename[FILENAME_LEN];
    DNode *parent;
    FNode *right;
    int opfprotect;
    //char text[INPUT_LEN];
    int pointer;
    bool isOpen;
    struct user *createuser;
}FNode,*fnode;

typedef struct user{
    char username[FILENAME_LEN];
    char password[FILENAME_LEN];
}user,*User;

DNode *root_global;
DNode *current_dir_global;
char current_dir_name[PATH_LEN];
char *input;
char *param_global[5];
char *cmd_global;
char *path_global;
char * g_filename[10];
int current_cmd_index;//记录当前命令的索引
char *file_deal;//第三个参数
User current_user;
vector<User> all_user_list;
vector<fnode> all_open_file;

void init();
DNode *createDNode(char *name);
FNode *createFNode(char *name);
int GetInput();
void find_command();
Bool deal_param(int i);
void mkdir();
void MdComdCase(DNode * root, char *filename[], size_t filename_len);
void InsertNode(DNode *parent, DNode *child);
DNode  *FindFile(DNode * node, const char * filename);
Bool CheckPath(DNode * root,DNode ** desk);
void p_type_commd(void (*func)(void));
void ls();
void lsp();
void cdp();
void pwd();
void cd();
void mkdirp();
void exit();
char *output_s(char *p);
void regusr();
void disusr();
void delusr();
int find_user();
void login();
void create();
FNode *findFNode(DNode * node, const char * filename);
void createp();
void open();
void chmod();
void chmodp();
void lsfile();


int main(){
    init();

    void(*fun_table[])() ={mkdir,ls,lsp,cd,cdp,pwd,mkdirp,exit,regusr,disusr,delusr,\
                    login,create,createp,open,chmod,chmodp,lsfile};

    while(1){
        int cmd_l=GetInput();
        find_command();
        if(current_cmd_index==-1){
            printf("没有这个指令\n");
        }else{
            Bool a = deal_param(cmd_l);
            if(a==TRUE)
                fun_table[current_cmd_index]();
        }
    }    
    exit(0);
}

void init(){
    char root[FILENAME_LEN]="/";
    current_dir_name[0]='/';
    current_dir_name[1]='\0';
    DNode *node=createDNode(root);
    strcpy(node->path,"/");
    root_global=node;
    current_dir_global=root_global;

    User root_user=(struct user *)malloc(sizeof(user));
    strcpy(root_user -> username, "root");
    strcpy(root_user -> password, "111");
    current_user=root_user;
    all_user_list.push_back(root_user);
}

void regusr(){
    if(find_user()==-1){
        User u=(struct user *)malloc(sizeof(user));
        strcpy(u -> username, param_global[1]);
        strcpy(u -> password, param_global[2]);
        all_user_list.push_back(u);
        printf("register ok\n");
    }else{
        printf("user already exists\n");
    }
}

int find_user(){
    for(unsigned int i = 0; i < all_user_list.size(); ++i)  
    {  
        if(!strcmp(param_global[1],all_user_list[i]->username)){
            return i;
        }
    }
    return -1;
}

void delusr(){
    if(!strcmp("root",current_user->username)){
        int index=find_user();
        if(index==-1){
            printf("user does not exist\n");
        }else{
            all_user_list.erase(all_user_list.begin() + index);
        }
    }else{
        printf("permission denied\n");
    }
}

void disusr(){
    for(unsigned int i = 0; i < all_user_list.size(); ++i)  
    {  
        printf("%s\n",all_user_list[i]->username);
    }
}

void login(){
    int index=find_user();
    if(index==-1){
        printf("user does not exist\n");
    }else if(!strcmp(param_global[1],current_user->username)){
        
    }else{
        printf("password:");
        size_t len;
        getline(&input, &len, stdin);
        char delim[] = " \n";
        char * saveptr;
        char * p = strtok_r(input, delim, &saveptr);
        if(!strcmp(p,all_user_list[index]->password)){
            printf("login ok\n");
            current_user=all_user_list[index];
        }else{
            printf("password wrong\n");
        }
    }
    fflush(stdin);
}


void find_command(){
    int flag=COMD_NUM;
    for(int i=0;i<COMD_NUM;++i){
        if(strcmp(cmd_global, comdname_all[i]) == 0){
            flag=i;
            break;
        }
    }
    current_cmd_index=(flag==COMD_NUM)?-1:flag;
}

int GetInput(){
    printf("%s$%s%%:", current_user->username, output_s(current_dir_name));
    size_t len;
    getline(&input, &len, stdin);
    //printf("len %d\n",len);
    unsigned int i = 0;
    char delim[] = " \n";
    char * saveptr;
    char * p = strtok_r(input, delim, &saveptr);
    param_global[i++] = p;
    cmd_global = p;
    while((p = strtok_r(NULL, delim, &saveptr)))
        param_global[i++] = p;
    param_global[i] = NULL;

    return i;
}

char *output_s(char *p){
    int len=strlen(p);
    if (*(p+len-1)=='/') {
        return p;
    }else{
        strcat (p,"/");
        return p;
    }
    
}

Bool deal_param(int i){
    //printf("%d          %d\n",i,comdname_len[current_cmd_index]);
    if(i!=comdname_len[current_cmd_index]){
        //printf("%d          %d\n",i,comdname_len[current_cmd_index]);
        printf("参数输入错误\n");
        return FALSE;
    }else{
        if(i==2){
            path_global = param_global[1];
        }else if(i>=3){
            path_global=param_global[1];
            file_deal=param_global[2];
        }
        return TRUE;
    }
}


DNode *createDNode(char *name){
    DNode *node=(struct DNode*)malloc(sizeof(DNode));
    strcpy(node -> filename, name);
    node -> parent = NULL;
    node -> Dchild = NULL;
    node -> Fchild = NULL;
    node -> right = NULL;
    return node;
}

FNode *createFNode(char *name,int opf){
    printf("ddd\n");
    if(opf<0||opf>7){
        printf("the opfprotect should be a interger(0~7)\n");
        return (struct FNode *)0;
    }
    fnode node=(struct FNode*)malloc(sizeof(FNode));
    strcpy(node -> filename, name);
    node -> parent = NULL;
    node -> right = NULL;
    node->opfprotect=opf;
    node->isOpen=false;
    node->pointer=0;
    node->createuser=current_user;printf("ddd\n");

    return node;
}

void mkdir(){
    char * filename[DIR_DEEPTH];
    char delim[] = "/";
    char * saveptr;
    char * tmp = (char *)malloc(sizeof(path_global));
    strcpy(tmp,path_global);
    char * ptr = strtok_r(tmp, delim, &saveptr);
    size_t i = 0;
    filename[i++] = ptr;
    while((ptr = strtok_r(NULL, delim, &saveptr)) != NULL)
        filename[i++] = ptr;

    if(*path_global == '/'){
        MdComdCase(root_global, filename, i);
    }
    else{
        MdComdCase(current_dir_global, filename, i);
    }

    free(tmp);

}

void mkdirp(){
    p_type_commd(mkdir);
}

void dedircase(DNode *root){
    DNode *p;
    if(CheckPath(root,&p)==TRUE){
        
    }
}

void dedir(){
    if (*path_global=='/') {
        dedircase(root_global);
    }else{
        dedircase(current_dir_global);
    } 
}

void MdComdCase(DNode * root, char *filename[], size_t filename_len){
    struct DNode * p;
    if(CheckPath(root, &p) == FALSE){
        size_t j;
        DNode * node, * parent = root;
        for(j = 0; j < filename_len;j++){
            node = createDNode(filename[j]);
            InsertNode(parent, node);
            parent = node;
        }
    }
    else{
        printf("directory already exists\n" );
    }
}

Bool CheckPath(DNode * root,DNode ** desk){
    char * path = (char *)malloc(sizeof(path_global));
    strcpy(path, path_global);
    char delim[] = "/";
    char * saveptr;
    char * p = strtok_r(path, delim, &saveptr);
    size_t i = 0;
    g_filename[i++] = p;
    while((p = strtok_r(NULL, delim, &saveptr)))
        g_filename[i++] = p;
    size_t j;
    struct DNode * child = root;
    for(j = 0; j < i; j++){
        if((child = FindFile(child, g_filename[j])) == NULL){
            *desk = NULL;
            return FALSE;
        }
    }
    *desk = child;
    return TRUE;
}

DNode  *FindFile(DNode * node, const char * filename){
    if(node == NULL)
        return (struct DNode *)0;
    DNode *child = node -> Dchild;
    while(child != NULL && strcmp(child -> filename, filename) != 0)//先判断再传参
        child = child -> right;
    return child;
}

void InsertNode(DNode *parent, DNode *child){
    child -> parent = parent;
    strcpy(child->path,parent->path);
    strcat (child->path,child->filename);
    strcat (child->path,"/");
    if(parent -> Dchild){
        child -> right = parent -> Dchild;
        parent -> Dchild -> left = child;
    }
    parent -> Dchild = child;
}

//ls命令
void ls(){
    DNode *p=current_dir_global->Dchild;
    while(p){
        //printf("%s  ",p->filename);
        printf( "\033[0;31;40m%s\033[0m  ",p->filename);
        p=p->right;
    }
    FNode *i=current_dir_global->Fchild;
    while(i){
        printf("%s  %d",i->filename,i->opfprotect);
        i=i->right;
    }
    printf("\n");
}

void lsp(){
    p_type_commd(ls);
}

void cd(){
    cdp();
}

//cd 命令
void cdp(){
    DNode *p;
    if(*path_global == '/'){
        if(strlen(path_global) == 1){
            current_dir_global = root_global;
            strcpy(current_dir_name,path_global);
            return;
        }
        if(CheckPath(root_global, &p) == TRUE){
            current_dir_global = p;
            strcpy(current_dir_name,path_global);
        }
        else{
            printf("CdComd : without this path\n");
        }
    }
    else{
        if(CheckPath(current_dir_global, &p) == TRUE){
            //if(current_dir_global != root_global)
                //strcat(current_dir_name, "/");//将两个char类型链接
            strcat(current_dir_name, path_global);
            current_dir_global = p;
        }
        else{
            printf("CdComd : without this path\n");
        }
    }
}

//pwd 命令
void pwd(){
    printf("%s\n",current_dir_name);
}

void exit(){
    exit(0);
}

void create(){
    if (findFNode(current_dir_global,path_global)) {
        printf("file already exists\n");
        return;
    }
    printf("dddddd\n");
    int opf=atoi(param_global[comdname_len[current_cmd_index]-1]);
    printf("%s ,%d\n",path_global,opf);
    FNode *file=createFNode(path_global,opf);
    printf("dddddd\n");
    if(!file){
        return;
    }
    if(!current_dir_global->Fchild){
        current_dir_global->Fchild = file;
    }else{
        FNode *p=current_dir_global->Fchild;
        while(p->right){
            p=p->right;
        }
        p->right=file;
    }
    //file->parent=current_dir_global;
    //file->createuser=current_user;
    printf("dddddd\n");
}

FNode  *findFNode(DNode * node, const char * filename){
    FNode *child = node -> Fchild;
    while(child != NULL && strcmp(child -> filename, filename) != 0)//先判断再传参
        child = child -> right;
    return child;
}

void createp(){
    p_type_commd(create);
}

void open(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }
    if(file->isOpen){
       printf("file already open\n"); 
    }else{
        file->isOpen=true;
        all_open_file.push_back(file);
    }
}

void openp(){
    p_type_commd(open);
}

void chmod(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }
    int opf=atoi(param_global[comdname_len[current_cmd_index]-1]);
    if(opf>=0&&opf<=7){
        file->opfprotect=opf;
    }else{
        printf("the opfprotect should be a interger(0~7)\n");
    }
}

void chmodp(){
    p_type_commd(chmod);
}

void p_type_commd(void (*func)(void)){
    char *current=(char *)malloc(PATH_LEN*sizeof(char));
    strcpy(current,current_dir_name);
    cd();
    path_global=file_deal;
    func();
    path_global=current;
    cd();
    free(current);
}

//显示所有打开的文件
void lsfile(){
    fnode f;
    for(unsigned int i = 0; i < all_open_file.size(); ++i)  
    {   
        f=all_open_file[i];
        printf("%s %s %s %s %d\n",f->parent->path,f->filename,f->createuser->username,current_user->username,f->opfprotect);
    }
}

// void write(){
//     fnode file=findFNode(current_dir_global,path_global);
//     if (!file) {
//         printf("file does not exist\n");
//         return;
//     }
//     char *buff=param_global[comdname_len[current_cmd_index]-2];
//     int wmode=atoi(param_global[comdname_len[current_cmd_index]-1]);
//     write_to(file,buff,wmode);
// }

// void writep(){
//     p_type_commd(write);
// }

// bool can_write(fnode f){
//     int opf=f->opfprotect;
//     if(opf==2||opf==3||opf==6||opf==7)
//         return true;
//     return false;
// }

// void write_to(fnode file,char *buff,int wmode){
//     if (!file->isOpen)
//     {
//         printf("file does not open\n");
//         return;
//     }
//     if(!can_write(file)){
//         printf("permission denied\n");
//     }

//     if(wmode==0){
//         strcat(file->text,buff);
//     }
// }

// void cat(){
//     fnode file=findFNode(current_dir_global,path_global);
//     if (!file) {
//         printf("file does not exist\n");
//         return;
//     }

//     printf("%s\n",file->text);
// }


