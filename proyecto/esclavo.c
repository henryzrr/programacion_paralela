#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include "pvm3.h"
#define NPROCESOS 60
#define COL 8000000

int tampatron,tamlinea,maxerror;
char linea[COL+5];
char patron[200];
FILE *fa;

int esIgual(int ini,int t){
    //int *conterror=(int*)malloc(sizeof(int));
    int conterror=0;
    for(int i=0;i<t;i++){
        if(patron[i]!=linea[ini]) conterror++;
        ini++;
    }
    
    return (conterror<=maxerror?1:0);

}

int main(){
    FILE *fp;
    int pid,i,j,inf,sup,tampatron,ocurrencias;
    

    pid = pvm_parent();
    pvm_joingroup("paralelo");
    
    pvm_freezegroup("paralelo", NPROCESOS);

    pvm_recv(-1,-1);
    pvm_upkint(&inf,1,1);
    pvm_upkint(&sup,1,1);
    pvm_upkstr(patron);
    /*fa=fopen("rata2","w");
        fputs(patron,fa);*/
    fp = fopen("archivo","r");
    tampatron=strlen(patron);

    //char a[10];
    //fa=fopen("rata2","a+");
    //sprintf(a,"%d",tampatron);

    //fputs(a,fa);

    if(fp==NULL){
        //fa=fopen("rata2","wt");
        //fputs(patron,fa);
        perror("NO SE PUEDE ABRIR EL ARCHIVO");
        return pvm_exit();
    }
    maxerror = tampatron/4;
    for(j = 0;j<inf-1 ;j++){
        fseek(fp, COL+1, SEEK_CUR);
    }
    //ocurrencias = (int*)malloc(sizeof(int));
    ocurrencias=0;
    //fa=fopen("rata2","wt");
    while(fgets(linea,8000002,fp)&&inf<=sup){
        
        //fputs(linea,fa);
        tamlinea = strlen(linea);
        //int tamlinea = strlen(linea);
        for(i=0;i<tamlinea-tampatron+1;i++){
            if(esIgual(i,tampatron)==1){
                ocurrencias=ocurrencias+1;
            }    
        }
        inf++;
    }
    //ocurrencias=1;
    pvm_initsend(PvmDataDefault);
    pvm_pkint(&ocurrencias,1,1);
    pvm_send(pid,1);
    pvm_barrier("paralelo", NPROCESOS);
    pvm_lvgroup("paralelo");
    pvm_exit();
    return 0;
}
