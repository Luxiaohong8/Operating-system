#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;
int main(int argc, char **argv) {
    map<int, int> m;
    float hit = 0;
    float miss = 0;
    for (;;){
        string input;
        int x;
        stringstream ss;
        float hit_ratio;
        cin >> input;
        if (input.compare("END") == 0){
            hit_ratio = hit / (hit + miss) * 100;
            cout << "Hit ratio = ";
            cout << setprecision(4) << hit_ratio;
            cout << "%\n";
            break;
        }
        ss << hex << input;
        ss >> x;
        int tag_index = x / 16;
        int tag = tag_index / 64;
        int index = tag_index - tag * 64;
        if (m.find(index) == m.end()) {
            miss++;
            cout << "Miss" << endl;
        }else{
            if (m[index] == tag){
                hit++;
                cout << "Hit" << endl;
            }else{
                miss++;
                cout << "Miss" << endl;
            }
        }
        m[index] = tag;
    }
}
