#include <math.h>
#include <stdio.h>
#include <string.h>

#define ADDRLEN 8
#define LEN 29
#define TAGLEN 19
#define INDEXLEN 7
#define SELECTLEN 5
#define CACHELINES 64

const char *table[] = {"0000", "0001", "0010", "0011", "0100", "0101",
                       "0110", "0111", "1000", "1001", "1010", "1011",
                       "1100", "1101", "1110", "1111"};

void hex2binary(char *binaryAddr, char *addr);

int getIndex(char *index);

int main() {
    char addr[ADDRLEN] = "";
    char binaryAddr[LEN] = "";
    char tag[TAGLEN] = "";
    char index[INDEXLEN] = "";
    char cache[CACHELINES][TAGLEN];
    double miss = 0;
    double hit = 0;
    while (1) {
        scanf("%7s", addr);
        if (!strcmp(addr, "END")) {
            double ratio = hit / (hit + miss) * 100;
            printf("Hit ratio = %.2f%%\n", ratio);
            break;
        }

        memset(binaryAddr, 0, LEN);
        hex2binary(binaryAddr, addr);

        strncpy(tag, binaryAddr, TAGLEN - 1);
        strncpy(index, binaryAddr + TAGLEN - 1, INDEXLEN - 1);


        int line = getIndex(index);

        if (!strcmp(tag, cache[line])) {
            printf("%s\n", "Hit");
            hit = hit + 1;
        } else {
            printf("%s\n", "Miss");
            miss = miss + 1;
            strncpy(cache[line], tag, TAGLEN - 1);
        }
    }
    return 0;
}

void hex2binary(char *binaryAddr, char *addr) {
    char ch;
    for (int i = 0; i < 7; i++) {
        ch = addr[i];
        if (ch >= '0' && ch <= '9') {
            strncat(binaryAddr, table[ch - '0'], 4);
        }
        if (ch >= 'A' && ch <= 'F') {
            strncat(binaryAddr, table[10 + ch - 'A'], 4);
        }
    }
}

int getIndex(char *index) {
    int x = 0;
    for (int i = 0; i < INDEXLEN - 1; i++) {
        x = x + pow(2, INDEXLEN - 2 - i) * (index[i] - '0');
    }
    return x;
}
