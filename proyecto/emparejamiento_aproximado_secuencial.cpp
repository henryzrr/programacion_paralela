#include<stdio.h>
#include<string.h>
#include<time.h>
clock_t ti,tf;
char patron[]="aba";
char linea[8000005];
int tampatron,tamlinea,maxerror;

int esIgual(int ini){
    int conterror=0;
    for(int i=0;i<tampatron;i++){
        if(patron[i]!=linea[ini]) conterror++;
        ini++;
    }
    return (conterror<=maxerror?1:0);
}

int main(){
	
	FILE *fp;
	fp = fopen("archivo","r");
    if(fp==NULL){
        perror("NO SE PUEDE ABRIR EL ARCHIVO");
        return -1;
    }
    
    tampatron = strlen(patron);
    maxerror = tampatron/4;

    int i,j,ocurrencias=0;
    ocurrencias=0;
    ti=clock();
    while(fgets(linea,8000002,fp)!=NULL){
		int tamlinea = strlen(linea);
        for(i=0;i<tamlinea-tampatron+1;i++){
            if(esIgual(i)==1){
                ocurrencias++;
            }
        }

	}
    tf=clock();
	printf("%i\n",ocurrencias);
	printf("%f\n",(double)(tf-ti)/CLOCKS_PER_SEC);
    return 0;
}