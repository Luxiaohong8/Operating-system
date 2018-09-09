#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

vector<int> tracks;
int S,M,N;
vector<int> sorted_tracks;
vector<int> T;
vector<int>::iterator p;
/*S 表示开始磁道的位置， 
M 表示当前磁盘可以容纳的最大磁道数， N 表示需要访问的磁道数*/

int my_distance=0;

void init();
void read_tracks();
void FCFS();
void SSTF();
void SCAN();
void print_T();
void scan_from_to(vector<int>::iterator s,vector<int>::iterator e,bool direction);
void CSCAN();
void LOOK();
void CLOOK();
void InsertSort();

int main(){
    init();
    FCFS();  
    SSTF();
    p=sorted_tracks.begin();
    int bb=-1;
    while(*p!=S){
        p++;
    }
    SCAN();
    CSCAN();
    LOOK();
    CLOOK();
    return 0;
}

void init(){
    int x,y,z;
    cin >> x;
    cin >> y;
    cin >> z;
    if (x < 0||x>=y){
        cerr << "Invalid Start(S)" << endl;
        return;
    }
    if(y<=0||y>65536){
        cerr << "Invalid Track Size(M)" << endl;
        return;
    }
    if(z<=0||z>=y){
        cerr << "Invalid the size of N" << endl;
        return;
    }

    S=x;
    M=y;
    N=z;

    read_tracks();

    for(int i = 0; i < N; i++)
    {
        sorted_tracks.push_back(tracks[i]);
    }
    sorted_tracks.push_back(S);
    InsertSort();
}

void read_tracks(){
    tracks.clear();
    for (int i = 0; i < N; i++){
        int x; 
        cin >> x;        
        if (x>=0&&x<M) {
            tracks.push_back(x);
        }else{
            return;
        }
    }
}

void InsertSort(){    
    for(int i= 1; i<N+1; i++){    
        if(sorted_tracks[i] < sorted_tracks[i-1]){                   
            int j= i-1;    
            int x = sorted_tracks[i];            
            sorted_tracks[i] = sorted_tracks[i-1];              
            while(j>=0 && x < sorted_tracks[j]){  
                sorted_tracks[j+1] = sorted_tracks[j];    
                j--;          
            }    
            sorted_tracks[j+1] = x;       
        }    
    }  
}   

void FCFS(){
    my_distance=0;
    int last=S;

    cout<<"FCFS"<<endl;
    cout<<S;
    for(auto &x:tracks){
        cout<<" -> "<<x;
        my_distance=my_distance+abs(last-x);
        last=x;
    }
    cout<<endl;
    cout<<my_distance<<endl;
}

typedef struct node{
    int number;
    int valid;
}node;

//SFTF: number表示磁道号，valid表示是否被访问
void SSTF(){
    cout<<"SSTF"<<endl;
    my_distance=0;

    list<node> the_sort_list;
    list<node>::iterator pointer;
    int i=0;

    for(int i = 0; i < N+1; i++)
    {
        node *n=new node;
        n->number=sorted_tracks[i];
        n->valid=1;
        the_sort_list.push_back(*n);
    }

    pointer=the_sort_list.begin();
    while(pointer->number!=S){
        pointer++;
    }
    list<node>::iterator last=pointer;
    list<node>::iterator next=pointer;
    while(i<N){
        cout<<pointer->number<<" -> ";
        pointer->valid = 0;

        while(last->valid!=1&&last!=the_sort_list.begin()){
            last--;
        }
        while(next->valid!=1&&next->number!=the_sort_list.back().number){
            next++;
        }

        if((pointer->number-last->number > next->number-pointer->number&&next->valid==1) || last->valid==0){
            my_distance=my_distance+abs(next->number-pointer->number);
            pointer=next;
        }
        else if ((pointer->number-last->number <= next->number-pointer->number&&last->valid==1) || next->valid==0){
            my_distance=my_distance+abs(pointer->number-last->number);
            pointer=last;
        }
        ++i;
    }

    cout<<pointer->number<<endl;
    cout<<my_distance<<endl;
}

void scan_from_to(vector<int>::iterator s,vector<int>::iterator e,bool direction){
    vector<int>::iterator i=s;
    if(direction){
        for(i = s; i != e; ++i)
        {
            T.push_back(*i);
        }
        T.push_back(*i);
    }else{
        for(i = s; i != e; --i)
        {
            T.push_back(*i);
        }
        T.push_back(*i);
    }
}

void print_T(){
    my_distance=0;
    for(int i = 0; i < T.size()-1; i++)
    {
        cout<<T[i]<<" -> ";
        my_distance=my_distance+abs(T[i+1]-T[i]);
    }
    cout<<T[T.size()-1]<<endl;
}

void SCAN(){
    cout<<"SCAN"<<endl;
    T.clear();
    bool direction=false;
    vector<int>::iterator my_p=p;

    if(sorted_tracks.back()-S < S-sorted_tracks.front()){
        direction=true;
    }else{
        while(*(my_p+1)==*my_p)
            my_p++;
    }

    if(direction){
        scan_from_to(my_p,--sorted_tracks.end(),true);
        T.push_back(M-1);
        my_p--;
        scan_from_to(my_p,sorted_tracks.begin(),false);
    }else{
        scan_from_to(my_p,sorted_tracks.begin(),false);
        T.push_back(0);
        my_p++;
        scan_from_to(my_p,--sorted_tracks.end(),true);
    }

    print_T();
    cout<<my_distance<<endl;
}

void CSCAN(){
    cout<<"C-SCAN"<<endl;
    T.clear();
    bool direction=false;
    vector<int>::iterator my_p=p;

    if(sorted_tracks.back()-S < S-sorted_tracks.front()){
        direction=true;
    }else{
        while(*(my_p+1)==*my_p)
            my_p++;
    }

    if(direction){
        scan_from_to(my_p,--sorted_tracks.end(),true);
        T.push_back(M-1);
        --my_p;
        T.push_back(0);
        scan_from_to(sorted_tracks.begin(),my_p,true);
    }else{
        scan_from_to(my_p,sorted_tracks.begin(),false);
        T.push_back(0);
        ++my_p;
        T.push_back(M-1);
        scan_from_to(--sorted_tracks.end(),my_p,false);
    }

    print_T();
    cout<<my_distance-M+1<<endl;
}

void LOOK(){
    cout<<"LOOK"<<endl;
    T.clear();
    bool direction=false;
    vector<int>::iterator my_p=p;

    if(sorted_tracks.back()-S < S-sorted_tracks.front()){
        direction=true;
    }else{
        while(*(my_p+1)==*my_p)
            my_p++;
    }

    if(direction){
        scan_from_to(my_p,--sorted_tracks.end(),true);
        my_p--;
        scan_from_to(my_p,sorted_tracks.begin(),false);
    }else{
        scan_from_to(my_p,sorted_tracks.begin(),false);
        my_p++;
        scan_from_to(my_p,--sorted_tracks.end(),true);
    }

    print_T();
    cout<<my_distance<<endl;
}

void CLOOK(){
    cout<<"C-LOOK"<<endl;
    T.clear();
    bool direction=false;
    vector<int>::iterator my_p=p;

    if(sorted_tracks.back()-S < S-sorted_tracks.front()){
        direction=true;
    }else{
        while(*(my_p+1)==*my_p)
            my_p++;
    }

    if(direction){
        scan_from_to(my_p,--sorted_tracks.end(),true);
        my_p--;
        scan_from_to(sorted_tracks.begin(),my_p,true);
    }else{
        scan_from_to(my_p,sorted_tracks.begin(),false);
        my_p++;
        scan_from_to(--sorted_tracks.end(),my_p,false);
    }
    print_T();
    cout<<my_distance-(sorted_tracks.back()-sorted_tracks.front())<<endl;
}
