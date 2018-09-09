#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;
#define FILENAME_LEN 20
#define COMMAND_LEN 15
#define INPUT_LEN 100
#define PATH_LEN 80
#define COMD_NUM 35
#define DIR_DEEPTH 20

//记录所有的命令，查找命令，获得索引
char comdname_all[][COMMAND_LEN] = {"mkdir","ls","lsp","cd","cdp","pwd","mkdirp",\
            "exit","regusr","disusr","delusr","login","create","createp","open","chmod",\
            "chmodp","lsfile","write","writep","cat","catp","reposition","repositionp",\
            "close","closep","search","truncate","truncatep","delete","openp","dedir",\
            "deletep","dedirp","man"};
//记录每个命令对应的参数的长度
int comdname_len[]={2,1,2,2,2,1,3,1,3,1,\
                    2,2,3,4,2,3,4,1,4,5,\
                    2,3,3,4,2,3,2,3,4,2,\
                    3,2,3,3,1};
//目录的结构体
typedef struct DNode{
    char filename[FILENAME_LEN];
    DNode *parent,*Dchild;
    struct FNode *Fchild;
    DNode *left,*right;
    char path[PATH_LEN];
}DNode;
//普通文件的结构体
typedef struct FNode{
    char filename[FILENAME_LEN];
    DNode *parent;
    FNode *right;
    int opfprotect;//文件权限
    char text[INPUT_LEN];//文件内容
    int pointer;//文件指针
    bool isOpen;
    struct user *createuser;
}FNode,*fnode;
//用户的结构体
typedef struct user{
    char username[FILENAME_LEN];
    char password[FILENAME_LEN];
}user,*User;

DNode *root_global;//记录根目录
DNode *current_dir_global;//记录每次工作的当前目录
char current_dir_name[PATH_LEN];//记录当前文件夹名
char *input;//记录命令行输入
char *param_global[6];//将命令行输入转化为的参数列表
char *cmd_global;
char *path_global;//记录第二个参数
char * g_filename[10];
int current_cmd_index;//记录当前命令的索引
char *file_deal;//第三个参数
User current_user;//记录当前的使用用户
vector<User> all_user_list;//该vector储存了所有用户
vector<fnode> all_open_file;//存储所有打开的文件
vector<fnode> all_file;

void init();
DNode *createDNode(char *name);
FNode *createFNode(char *name);
int GetInput();
void find_command();
bool deal_param(int i);
void mkdir();
void dedir();
void dedirp();
void dedirnode(DNode *d);
void MdComdCase(DNode * root, char *filename[], size_t filename_len);
void InsertNode(DNode *parent, DNode *child);
DNode  *FindFile(DNode * node, const char * filename);
bool CheckPath(DNode * root,DNode ** desk);
void p_type_commd(void (*func)(void));
void ls();
void lsp();
void cdp();
void pwd();
void cd();
void mkdirp();
void exit();
void regusr();
void disusr();
void delusr();
int find_user();
void login();
void create();
FNode *findFNode(DNode * node, const char * filename);
void createp();
void open();
void openp();
void chmod();
void chmodp();
void lsfile();
void write();
void write_to(fnode file,char *buff,int wmode);
void writep();
void cat();
bool can_read(fnode f);
bool can_write(fnode f);
void catp();
void reposition();
void repositionp();
void close();
void closep();
void search();
void truncate();
void truncatep();
void delete_cmd();
void delele_filenode(fnode file);
void deletep_cmd();
void man();

int main(){
    init();

    void(*fun_table[])() ={mkdir,ls,lsp,cd,cdp,pwd,mkdirp,exit,regusr,disusr,delusr,\
                    login,create,createp,open,chmod,chmodp,lsfile,write,writep,cat,\
                    catp,reposition,repositionp,close,closep,search,truncate,truncatep,\
                    delete_cmd,openp,dedir,deletep_cmd,dedirp,man};

    while(1){
        int cmd_l=GetInput();
        find_command();
        if(current_cmd_index==-1){
            printf("没有这个指令\n");
        }else{
            bool a = deal_param(cmd_l);
            if(a==true)
                fun_table[current_cmd_index]();
        }
    }    
    exit(0);
}

//处理带路径的命令，比如lsp 等
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

//初始化
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

//用户注册
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

//从用户列表寻找对应用户的索引
int find_user(){
    for(unsigned int i = 0; i < all_user_list.size(); ++i)  
    {  
        if(!strcmp(param_global[1],all_user_list[i]->username)){
            return i;
        }
    }
    return -1;
}

//删除用户
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

//输出所有用户
void disusr(){
    if(strcmp(current_user->username,"root")){
        printf("permission denied\n");
        return;
    }
    for(unsigned int i = 0; i < all_user_list.size(); ++i)  
    {  
        printf("%s\n",all_user_list[i]->username);
    }
}

//用户登录
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

//从命令列表找到对应的命令
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

//从命令行获取命令，并获得对应参数
int GetInput(){
    printf("%s$%s%%:", current_user->username, current_dir_global->path);
    size_t len;
    getline(&input, &len, stdin);
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

//处理参数列表，并判断输入参数是否合乎规范
bool deal_param(int i){
    if(i!=comdname_len[current_cmd_index]){
        printf("参数输入错误\n");
        return false;
    }else{
        if(i==2){
            path_global = param_global[1];
        }else if(i>=3){
            path_global=param_global[1];
            file_deal=param_global[2];
        }
        return true;
    }
}

//创建DNode
DNode *createDNode(char *name){
    DNode *node=(struct DNode*)malloc(sizeof(DNode));
    strcpy(node -> filename, name);
    node -> parent = NULL;
    node -> Dchild = NULL;
    node -> Fchild = NULL;
    node -> right = NULL;
    return node;
}

//创建FNode
FNode *createFNode(char *name,int opf){
    if(opf<0||opf>7){
        printf("the opfprotect should be a interger(0~7)\n");
        return (struct FNode *)0;
    }
    fnode node=(struct FNode*)malloc(sizeof(struct FNode));
    strcpy(node -> filename, name);
    node -> parent = NULL;
    node -> right = NULL;
    node->opfprotect=opf;
    node->isOpen=false;
    node->pointer=0;
    node->createuser=NULL;
    return node;
}

//创建文件夹
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

//删除文件夹
void dedir(){
    DNode *dir=current_dir_global->Dchild;

    while(dir){
        if(!strcmp(path_global,dir->filename)){
            break;
        }
        dir=dir->right;
    }
    if(!dir){
        printf("directory does not exist\n");
        return;
    }

    DNode *d=dir;
    if(dir->Dchild){
        dedirnode(dir->Dchild);
    }

    if(d->parent->Dchild==d){
        d->parent->Dchild=d->right;
    }else{
        DNode *p=d->parent->Dchild;
        while(p->right != d){
            p=p->right;
        }
        p->right=d->right;       
    }

    FNode *file=d->Fchild;
    while(file){
        delele_filenode(file);
        file=file->right;
    }
    free(d);
}

void dedirp(){
    p_type_commd(dedir);
}

//删除文件夹下的子文件夹
void dedirnode(DNode *root){
    if(root -> Dchild == NULL){
        FNode *file=root->Fchild;
        while(file){
            delele_filenode(file);
            file=file->right;
        }
        return;
    }
    DNode *node = root -> Dchild;
    FNode *file=root->Fchild;
    while(node){
        if(node){
            dedirnode(node);
            root = node;

            while(file){
                delele_filenode(file);
                file=file->right;
            }
            node = node -> right;
            free(root);
        }
    }
}

void MdComdCase(DNode * root, char *filename[], size_t filename_len){
    struct DNode * p;
    if(CheckPath(root, &p) == false){
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

//检查路径
bool CheckPath(DNode * root,DNode ** desk){
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
            return false;
        }
    }
    *desk = child;
    return true;
}

//寻找目录
DNode  *FindFile(DNode * node, const char * filename){
    if(node == NULL)
        return (struct DNode *)0;
    DNode *child = node -> Dchild;
    while(child != NULL && strcmp(child -> filename, filename) != 0)//先判断再传参
        child = child -> right;
    return child;
}

//插入
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
        printf( "\033[0;31;40m%s\033[0m\n",p->filename);
        p=p->right;
    }
    FNode *i=current_dir_global->Fchild;
    while(i){
        printf("%s\n",i->filename);
        i=i->right;
    }
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
        if(CheckPath(root_global, &p) == true){
            current_dir_global = p;
            strcpy(current_dir_name,path_global);
        }
        else{
            printf("directory does not exist\n");
        }
    }
    else{
        if(CheckPath(current_dir_global, &p) == true){
            strcat(current_dir_name, path_global);
            current_dir_global = p;
        }
        else{
            printf("directory does not exist\n");
        }
    }
}

//pwd 命令，输出当前工作目录
void pwd(){
    printf("%s\n",current_dir_name);
}

//退出主程序
void exit(){
    exit(0);
}

//创建文件
void create(){
    if (findFNode(current_dir_global,path_global)) {
        printf("file already exists\n");
        return;
    }
    int opf=atoi(param_global[comdname_len[current_cmd_index]-1]);
    FNode *file=createFNode(path_global,opf);
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
    file->parent=current_dir_global;
    file->createuser=current_user;
    all_file.push_back(file);
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

//打开文件
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

//修改文件权限
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

//显示所有打开的文件
void lsfile(){
    fnode f;
    for(unsigned int i = 0; i < all_open_file.size(); ++i)  
    {   
        f=all_open_file[i];
        printf("%s %s %s %s %d\n",f->parent->path,f->filename,f->createuser->username,current_user->username,f->opfprotect);
    }
}

//写文件
void write(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }
    if (!file->isOpen)
    {
        printf("file does not open\n");
        return;
    }
    if(!can_write(file)){
        printf("permission denied\n");
        return;
    }
    char *buff=param_global[comdname_len[current_cmd_index]-2];
    int wmode=atoi(param_global[comdname_len[current_cmd_index]-1]);
    write_to(file,buff,wmode);
    printf("write ok\n");
}

void writep(){
    char *current=(char *)malloc(PATH_LEN*sizeof(char));
    strcpy(current,current_dir_name);
    cd();
    path_global=file_deal;
    write();
    path_global=current;
    cd();
    free(current);
}

bool can_write(fnode f){
    int opf=f->opfprotect;
    if(opf==2||opf==3||opf==6||opf==7)
        return true;
    return false;
}

void write_to(fnode file,char *buff,int wmode){
    if(wmode==0){
        strcat(file->text,buff);
    }else if(wmode==1){
        memcpy((file->text)+file->pointer,buff,strlen(buff));
    }else if(wmode==2){
        char *p=(char *)malloc(INPUT_LEN*sizeof(char));
        strncat(p,file->text,file->pointer);
        strcat(p,buff);
        strcat(p,file->text + file->pointer);
        strcpy(file->text,p);
        free(p);
    }
    file->pointer=strlen(file->text);
}

//输出文件内容
void cat(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }
    if(!can_read(file)){
        printf("permission denied\n");
        return;
    }
    if (!file->isOpen)
    {
        printf("file does not open\n");
        return;
    }
    printf("%s\n",file->text);
}

void catp(){
    p_type_commd(cat);
}

bool can_read(fnode f){
    int opf=f->opfprotect;
    if(opf==4||opf==5||opf==6||opf==7)
        return true;
    return false;
}

//文件重定向
void reposition(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }

    int pos=atoi(param_global[comdname_len[current_cmd_index]-1]);
    if(pos<0||pos>strlen(file->text)){
        printf("reposition failed\n");
        return;
    }
    file->pointer=pos;
    printf("reposition ok\n");
}

void repositionp(){
    p_type_commd(reposition);
}

//关闭文件
void close(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }
    file->isOpen=false;
    vector<fnode>::iterator it;
    for(it=all_open_file.begin();it!=all_open_file.end();)
    {
        if(*it==file){
            it=all_open_file.erase(it);
        }else{
            it++;
        }
    }
}

void closep(){
    p_type_commd(close);
}

//查找所有文件名为 file_name 的文件，并输出文件所在路径
void  search(){
    fnode f;
    bool find=false;
    for(unsigned int i = 0; i < all_file.size(); ++i)  
    {   
        f=all_file[i];
        if(!strcmp(f->filename,path_global)){
            printf("%s%s\n",f->parent->path,f->filename);
            find=true;
        }
    }
    if(!find)
        printf(" file does not found\n");
}

//保留当前工作目录名称为 file_name 文件的前 cnt 个字符
void truncate(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }

    if(!can_write(file)){
        printf("permission denied\n");
        return;
    }

    int cnt=atoi(param_global[comdname_len[current_cmd_index]-1]);
    if(cnt<0||cnt>strlen(file->text)){
        printf("reposition failed\n");
        return;
    }
    char *p=(char *)malloc(INPUT_LEN*sizeof(char));
    strncat(p,file->text,cnt);
    strcpy(file->text,p);
    free(p);
    file->pointer=cnt;
}

void truncatep(){
    p_type_commd(truncate);
}

//删除文件结构
void delele_filenode(fnode file){
    if(file->parent->Fchild==file){
        file->parent->Fchild=file->right;
    }else{
        FNode *p=file->parent->Fchild;
        while(p->right != file){
            p=p->right;
        }
        p->right=file->right;       
    }

    vector<fnode>::iterator it;
    for(it=all_file.begin();it!=all_file.end();)
    {
        if(*it==file){
            it=all_file.erase(it);
        }else{
            it++;
        }
    }

    if(file->isOpen){
        for(it=all_open_file.begin();it!=all_open_file.end();++it)
        {
            if(*it==file){
                it=all_open_file.erase(it);
            }else{
                it++;
            }
        }
    }
    free(file);
}

//删除文件
void delete_cmd(){
    fnode file=findFNode(current_dir_global,path_global);
    if (!file) {
        printf("file does not exist\n");
        return;
    }
    delele_filenode(file);
}

void deletep_cmd(){
    p_type_commd(delete_cmd);
}

void man(){
    printf("Manual:\n"
         "0) pwd (current work directory)\n"
         "1) mkdir(p) (path) directory_name\n"
         "2) dedir(p) (path) directory_name\n"
         "3) search file_name\n"
         "4) create(p) (path) file_name omode\n"
         "5) cat(p) (path) file_name\n"
         "6) delete(p) (path) file_name\n"
         "7) write(p) (path) file_name buff wmode\n"
         "8) reposition(p) (path) file_name pos\n"
         "9) truncate(p) (path) file_name cnt\n"
         "10) ls(p) (path)\n"
         "11) open(p) (path) file_name\n"
         "12) close(p) (path) file_name\n"
         "13) regusr user_name password\n"
         "14) delusr user_name [root only]\n"
         "15) disusr\n"
         "16) login user_name\n"
         "17) lsfile\n"
         "18) chmod(p) (path) file_name mode\n"
         "19) cd(p) (path) dir_name\n"
         "20) exit\n");
}