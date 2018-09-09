#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

#define diff(x,y) x>y?x-y:y-x

int disk_begin;
int disk_max;
int disk_visit;
int dist;
vector<int> disks;

void FCFS(){
	dist = 0;
	printf("FCFS\n");
	printf("%d", disk_begin);
	int pre = disk_begin;
	for (auto &x : disks){
		////printf("->%d", x);
		dist += diff(pre,x);
		pre = x;
	}
	printf("\n%d\n", dist);
}

void SSTF(){
	dist = 0;
	cout<<"SSTF"<<endl;
	cout<<"0000000000"<<endl;
	cout<<disk_begin<<endl;
	sort(disks.begin(),disks.end(),less<int>());
	cout<<"0000000000"<<endl;
	int pre = disk_begin;
	int x = 0;
	int now,l,r;
	for (int i = 0; i < disks.size()-1; i++){
		if(disks[i]<=pre && disks[i+1]>=pre){
			if(disks[i+1]-pre <= pre-disks[i]){
				l = i;
				now = i+1;
				r = i+2;
			}
			else{
				l = i-1;
				now = i;
				r = i+1;
			}
			////printf("->%d", disks[now]);
			dist += diff(pre,disks[now]);
			pre = disks[now];
			break;
		} 
	}
	cout<<now<<"  "<<l<<"   "<<r<<endl;	
	cout<<"0000000000"<<endl;
	//int i=0;
	while(l!=-1||r!=disks.size()){
		cout<<disks[l]<<"  "<<disks[now]<<"   "<<disks[r]<<endl;
		if(l == -1 ||((l!=-1)&&(r!=disks.size())&&disks[r]-pre <= pre-disks[l])){
			now = r;
			r++;
		}else if(r == disks.size() || ((l!=-1)&&(r != disks.size())&&disks[r]-pre > pre-disks[l])){
			now = l;
			l--;
		}			
		////printf("->%d", disks[now]);
		dist += diff(pre,disks[now]);
		pre = disks[now];
		//++i;
	}
	cout<<"0000000000"<<endl;
	cout<<dist<<endl;
}

void SCAN(){
	dist = 0;
	printf("SCAN\n");
	sort(disks.begin(),disks.end(),less<int>());
	printf("%d", disk_begin);
	int pre = disk_begin;
	int x = 0;
	int l,r;
	for (int i = 0; i < disks.size()-1; i++){
		if(disks[i]<pre && disks[i+1]>pre){
			if(disks[i+1]-pre <= pre-disks[i]){
				l = i;
				r = i+1;
			}
			else{
				l = i-1;
				r = i;
			}
			break;
		}
	}	
	
	while(l != -1){
		////printf("->%d", disks[l]);
		dist += diff(pre,disks[l]);
		pre = disks[l];
		l--;
	}
	//printf("->0");
	dist += diff(pre,0);
	pre = 0;
	while(r != disks.size()){
		////printf("->%d", disks[r]);
		dist += diff(pre,disks[r]);
		pre = disks[r];
		r++;
	}
	printf("\n%d\n", dist);
}

void CSCAN(){
	dist = 0;
	printf("C-SCAN\n");
	sort(disks.begin(),disks.end(),less<int>());
	printf("%d", disk_begin);
	int pre = disk_begin;
	int x = 0;
	int l,r;
	for (int i = 0; i < disks.size()-1; i++){
		if(disks[i]<pre && disks[i+1]>pre){
			if(disks[i+1]-pre <= pre-disks[i]){
				l = i;
				r = i+1;
			}
			else{
				l = i-1;
				r = i;
			}
			break;
		}
	}	
	while(l != -1){
		////printf("->%d", disks[l]);
		dist += diff(pre,disks[l]);
		pre = disks[l];
		l--;
	}
	//printf("->0");
	dist += diff(pre,0);
	pre = 0;
	////printf("->%d",disk_max-1);
	//dist += diff(pre,disks.size());
	pre = disk_max - 1;
	int pt = disks.size()-1;
	while(pt != r-1){
		////printf("->%d", disks[pt]);
		dist += diff(pre,disks[pt]);
		pre = disks[pt];
		pt--;
	}
	printf("\n%d\n", dist);
}

void LOOK(){
	dist = 0;
	printf("LOOK\n");
	sort(disks.begin(),disks.end(),less<int>());
	printf("%d", disk_begin);
	int pre = disk_begin;
	int x = 0;
	int l,r;
	for (int i = 0; i < disks.size()-1; i++){
		if(disks[i]<pre && disks[i+1]>pre){
			if(disks[i+1]-pre <= pre-disks[i]){
				l = i;
				r = i+1;
			}
			else{
				l = i-1;
				r = i;
			}
			break;
		}
	}	
	while(l != -1){
		////printf("->%d", disks[l]);
		dist += diff(pre,disks[l]);
		pre = disks[l];
		l--;
	}
	while(r != disks.size()){
		////printf("->%d", disks[r]);
		dist += diff(pre,disks[r]);
		pre = disks[r];
		r++;
	}
	printf("\n%d\n", dist);
}

void CLOOK(){
	dist = 0;
	printf("C-LOOK\n");
	sort(disks.begin(),disks.end(),less<int>());
	printf("%d", disk_begin);
	int pre = disk_begin;
	int x = 0;
	int l,r;
	for (int i = 0; i < disks.size()-1; i++){
		if(disks.at(i)<pre && disks.at(i+1)>pre){
			if(disks.at(i+1)-pre <= pre-disks.at(i)){
				l = i;
				r = i+1;
			}
			else{
				l = i-1;
				r = i;
			}
			break;
		}
	}	
	while(l != -1){
		////printf("->%d", disks[l]);
		dist += diff(pre,disks[l]);
		pre = disks[l];
		l--;
	}
	int pt = disks.size()-1;
	pre = disks[pt];
	////printf("->%d", disks[pt]);
	pt--;
	while(pt != r-1){
		////printf("->%d", disks[pt]);
		dist += diff(pre,disks[pt]);
		pre = disks[pt];
		pt--;
	}
	printf("\n%d\n", dist);
}

int main(int argc, char const *argv[])
{
	cin >> disk_begin;
	cin >> disk_max;
	cin >> disk_visit;

	//vector<int> disks;
	disks.clear();
	
	for (int i = 0; i < disk_visit; i++){
		int x; cin >> x; disks.push_back(x);
	}

	FCFS();
	SSTF();
	SCAN();
	CSCAN();
	LOOK();
	CLOOK();
	return 0;
}