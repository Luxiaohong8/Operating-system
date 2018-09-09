#include <stdio.h>  
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    FILE *fp = NULL;
    fp=fopen("bab.txt","w");
    
    if (fp==NULL) {
        printf("xxx你妈");
        exit(0);
    }
    
    return 0;
}
