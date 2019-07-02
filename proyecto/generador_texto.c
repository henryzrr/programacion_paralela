#include<stdio.h>
#include<math.h>
int main(){
	int r;
	for(int i=0;i<200;i++){
		for(int j=0;j<8000000;j++){
			r=(rand()%('z'-'a'+1))+'a';
			printf("%c",r);	
		}
	}
}