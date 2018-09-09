#include<bits/stdc++.h>
#include<unistd.h>
#include <iomanip>//保留俩位小数，导入库

using namespace std;

const double eps = 1e-8;
int dcmp(const double &x){ if (fabs(x) < eps) return 0; return x < 0 ? -1 : 1; }

#define DEFAULT_CACHE_SIZE 123
#define DEFAULT_ALGORITHM 0

typedef pair<int, string> _algorithm;

int cache_size, working_algorithm, hit, miss;
vector<_algorithm> algorithms;
vector<int> pages;

void init();
void print_menu();
void set_cache_size();
void print_algorithms();
void set_algorithm();
void add_an_algorithm();
void generate_input();
void generate_randomly(int n);
void generate_by_hand(int n);
void print_pages();
void FIFO_algorithm();
void run_algorithm();
void read_pages();
void LRU_algorithm();
void Min_algorithm();
int caculate_distance(int i,int index);
void Clock_algorithm();
void Second_chance_algorithm();

int main(){
    //freopen("1.in", "r", stdin);
    init();
    set_cache_size();
    set_algorithm(); 
    read_pages();
    run_algorithm();
    return 0;
}

void print_pages(){
    cout << "pages number is: ";
    cout << pages.size() << endl;
    cout << "pages are: ";
    for (auto &x : pages){
        cout << x << " ";
    }
    cout << endl;
}

void generate_randomly(int n){
    cout << n << endl;
    for (int i = 0; i < n; i++){
        cout << rand() * 233 % cache_size << " ";
    }
    cout << endl;
}

void generate_by_hand(int n){
    cout << n << endl;
    for (int i = 0; i < n; i++){
        int x; cin >> x; cout << x << " ";
    }
    cout << endl;
}

void generate_input(){
    pages.clear();
    //cout << "Please input the number of pages: ";
    int n, op;
    cin >> n;
    cout << "1) Generate randomly" << endl;
    cout << "2) Generate by hand" << endl;
    cin >> op;
    freopen("1.in", "w", stdout);
    switch (op){
        case 1: generate_randomly(n); break;
        case 2: generate_by_hand(n); break;
        default: break;
    }
}

void init(){
    srand((unsigned int)time(0));
    algorithms.clear(); pages.clear();
    cache_size = DEFAULT_CACHE_SIZE;
    string s = "FIFO";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM, s));
    s = "LRU";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 1, s));
    s = "Min";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 2, s));
    s = "Clock";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 3, s));
    s = "Second chance";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 4, s));
    working_algorithm = 0;
}

void read_pages(){
    pages.clear();
    int n; cin >> n;
    for (int i = 0; i < n; i++){
        int x; cin >> x; pages.push_back(x);
    }
}

void set_cache_size(){
    //cout << "Please input the new cache size: ";
    int x;
    cin >> x;
    if (x <= 0){
        cerr << "Invalid cache size" << endl;
        return;
    }
    cache_size = x;
    //cout << "set cache size successfully, new cache size is: " << cache_size << endl;
}

void print_algorithms(){
    cout << "All algorithms are: " << endl;
    for (auto &x : algorithms){
        cout << x.first << " : " << x.second << endl;
    }
}

void set_algorithm(){
    //print_algorithms();
    unsigned int id;
    cin >> id;
    working_algorithm = id;
    if (id >= algorithms.size()){
        cerr << "Algorithm does not exists" << endl;
        return;
    }
}

int caculate_distance(int i,int index){
    int pointer=i+1;
    while(pointer<pages.size()){
        if(index==pages[pointer]){
            return pointer;
        }
        ++pointer;
    }
    return pages.size();
}

void Min_algorithm(){
    hit=miss=0;
    list<int> l;
    l.clear();
    int i=-1;
    map<int, int> map_long_time;
    map<int, int>::iterator iter;

    for(auto &x:pages){
        ++i;
        if(l.empty()){
            miss++;
            l.push_front(x);
            map_long_time[x]=caculate_distance(i,x);
            continue;
        }
        bool flag=false;
        for(auto &y:l){
            if(y==x){
                flag=true;
                break;
            }
        }

        if(flag){
            hit++; 
            map_long_time[x]=caculate_distance(i,x);
        }else{
            miss++;
            if(l.size()==cache_size){
                int max_long=0;
                iter = map_long_time.begin();
                while(iter != map_long_time.end()) {
                    if(iter->second>map_long_time[max_long]){
                        max_long=iter->first;
                    }
                    iter++;
                }

                l.remove(max_long);
                map_long_time.erase(max_long);
            }
            l.push_back(x);
            map_long_time[x]=caculate_distance(i,x);
        }   
    }
}

typedef struct node{
    int number;
    int valid;
}node;

void Clock_algorithm(){
    hit=miss=0;
    list<node> l;
    l.clear();
    list<node>::iterator pointer;
    
    //initial
    for(int i = 0; i < cache_size; ++i)
    {
        node *n=new node;
        n->number=0;
        n->valid=0;
        l.push_back(*n);
    }
    
    pointer=l.begin();
    int i=1;
    for(auto &x:pages){
        bool flag=false;
        node *temp;

        for(auto &y:l){
            if(y.number==x){
                flag=true;
                temp=&y;
                break;
            }
        }

        if(flag){
            hit++;
            temp->valid=1;
        }else{
            miss++;
            while(pointer->valid!=0){
                pointer->valid=0; 
                if (i>=cache_size) {
                    pointer=l.begin();
                    i=1;
                }else{
                    pointer++;
                    ++i;
                }                
            }
            pointer->number=x;
            pointer->valid=1;
            if (i>=cache_size) {
                pointer=l.begin();
                i=1;
            }else{
                pointer++;
                ++i;
            } 
        }


    }    
}

void FIFO_algorithm(){
    hit=miss=0;
    list<int> l;
    l.clear();
    for(auto &x:pages){
        if(l.empty()){
            miss++;
            l.push_front(x);
            continue;
        }

        bool flag=false;
        for(auto &y:l){
            if(y==x){
                flag=true;
                break;
            }
        }
        if(flag){
            hit++;
        }else{
            miss++;
            if(l.size()==cache_size){
                l.pop_front();//删除最先进入的一个元素
            }
            l.push_back(x);
        }
    }
}


void LRU_algorithm(){
    hit=miss=0;
    list<int> l;
    l.clear();
    for (auto &x:pages){
        if(l.empty()){
            miss++;
            l.push_front(x);
            continue;
        }
        bool flag=false;
        for(auto &y:l){
            if(y==x){
                flag=true;
                l.remove(y);
                l.push_front(x);
                break;
            }
        }

        if(flag){
            hit++;
        }else{
            miss++;
            if(l.size()==cache_size){
                l.pop_back();//删除最后一个元素
            }
            l.push_front(x);
        }
    }
}

void Second_chance_algorithm(){
    hit=miss=0;
    list<int> FIFO;
    list<int> LRU;
    int FIFO_size=cache_size/2;
    int LRU_size=cache_size-FIFO_size;
    FIFO.clear();LRU.clear();

    for(auto &x:pages){

        if(FIFO.empty()){
            miss++;
            FIFO.push_front(x);
            continue;
        }

        bool flag1=false;
        bool flag2=false;
        for(auto &y:FIFO){
            if(y==x){
                flag1=true;
                break;
            }
        }        
        if (!flag1) {
            for(auto &y:LRU){
                if(y==x){
                    flag2=true;
                    LRU.remove(y);
                    LRU.push_front(x);
                    break;
                }
            }
        }
        
        if(flag1){
            hit++;
        }else if (flag2) {
            hit++;
            int value=LRU.front();
            LRU.pop_front();
            if (FIFO.size()==FIFO_size) {
                int a=FIFO.front();
                FIFO.pop_front();
                LRU.push_front(a);
                FIFO.push_back(value);
            }
        }else{
            miss++;

            if (FIFO.size()==FIFO_size) {
                int a=FIFO.front();
                FIFO.pop_front();
                if(LRU.size()==LRU_size){
                    LRU.pop_back();
                }
                LRU.push_front(a);
            }
            FIFO.push_back(x);
        }
    }    
}

void print_status(){
    cout.setf(ios::fixed);                      
    cout<<fixed<<setprecision(2)<< "Hit ratio = " << (double)hit * 100 / (miss + hit) << "%" <<endl;      //输出结果为1.20
}

void run_algorithm(){
    switch (working_algorithm){
        case 0: FIFO_algorithm(); break;
        case 1: LRU_algorithm(); break;
        case 2: Min_algorithm();break;
        case 3: Clock_algorithm();break;
        case 4: Second_chance_algorithm();break;
        default: break;
    }
    print_status();
}
