#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
int hexto_dec(char *hex);
int chartoint(char ch);
#define SIZE 100
#define Byteselect 4

int main(int argc, char const *argv[])
{
	int all_addr[SIZE];
	int valid[SIZE];
	char *addr=(char*)malloc(10*sizeof(char));;
	scanf("%s",addr);
	int num_addr;
	int amount=0;

	while(strcmp(addr,"END")!=0&&amount<=100){
		fflush(stdin);
		num_addr=hexto_dec(addr);

		if (num_addr!=-1&&strlen(addr)==7)
		{
			all_addr[amount]=num_addr>>Byteselect;
			int t=-1;
			for (int j = 0; j < amount; ++j)
			{
				if (all_addr[j]==all_addr[amount])
					t=1;				
			}

			if (t==1)
				valid[amount]=1;
			else
				valid[amount]=-1;

			++amount;
		}
		
		scanf("%s",addr);
	}
	free(addr);

	int hit_amount=0;
	for (int j = 0; j < amount; ++j)
	{
		if (valid[j]==1){
			printf("HIT\n");
			++hit_amount;
		}
		else{
			printf("MISS\n");
		}
	}
	printf("Hit ratio = %.2f%%\n",hit_amount*100.0/amount );
	return 0;
}

//change char to integer
int chartoint(char ch){
	if (isdigit(ch))
		return ch-48;

	if (ch<'A'||(ch>'F'&&ch<'a')||ch>'f')
		return -1;

	if (isalpha(ch))
		return isupper(ch)?ch-55:ch-87;

	return -1;
}

//Convert string hexadecimal to decimal
int hexto_dec(char *hex){
	int len=strlen(hex);
	int num=0;
	int temp;
	int bits;
	for (int i = 0,temp=0; i <len; ++i,temp=0)
	{
		temp=chartoint(*(hex+i));
		if (temp==-1)
			return -1;
		bits=(len-i-1)*4;
		temp=temp<<bits;
		num+=temp;
	}
	return num;
}