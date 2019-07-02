#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "pvm3.h"
#define NPROCESOS 60
#define N_LINEAS 200
#define PATRON "timnnqgbacmiymktjhuckqqnirbcerr"

int i,mytid,info,hijos[NPROCESOS],intervalo,aux,tareasrasp,tr[200];
int nhosts,narch,where,trrasp,arr[200];
struct pvmhostinfo *hostp;
struct pvmtaskinfo *taskp;

int  datosRaspberry(int inf){
    int k;
    int aux = intervalo/5;
    //int inf=1,sup;
    int sup;
    for(k=0;k<trrasp;k++){
        pvm_initsend(PvmDataDefault);
        //inf = ((k)*aux)+1;
        sup = inf+aux-1; 
        pvm_pkint(&inf,1,1);
        pvm_pkint(&sup,1,1);
        pvm_pkstr(PATRON);
        //pvm_pkstr("aba");
        pvm_send((*(taskp+k)).ti_tid,1);
        tr[tareasrasp++]=(*(taskp+k)).ti_tid;
        inf = sup+1;
    }
    //printf("%i\n",sup);
    return sup;
}

int enviado(int id){
    int k;
    for(k=0;k<tareasrasp;k++){
        if(id==tr[k])return 1;
    }
    return 0;
}
int main(){
    pvm_joingroup("paralelo");
    pvm_spawn("esclavo",NULL,0,"",NPROCESOS,hijos);
    pvm_config(&nhosts, &narch, &hostp);
    pvm_freezegroup("paralelo", NPROCESOS);
    
    //int k;
    tareasrasp=0;
    where =-1;
    int rasp[2];
    int jj=0;
    printf("%i\n",nhosts);
    for(i=0;i<nhosts;i++){
        if(strcmp((*(hostp+i)).hi_arch,"LINUXARM")==0){
            where=(*(hostp+i)).hi_tid;
            rasp[jj++]=where;
            //break;
        }
        printf("%s\n",(*(hostp+i)).hi_arch);
    }
    aux=0;
    trrasp=0;
    if(where!=-1){
        int jk;
        intervalo = N_LINEAS/NPROCESOS;
        int inf=1;
        for(jk=0;jk<jj;jk++){
            info=pvm_tasks(rasp[jk],&trrasp,&taskp);
            aux = datosRaspberry(inf);
            inf=aux;
        }
    }
    
    //intervalo = (N_LINEAS-aux)/(NPROCESOS-trrasp));
    //aux = N_LINEAS-(NPROCESOS*intervalo);
    //printf("%i\n",trrasp);
    int nl = N_LINEAS-aux;
    //int *arr;
    int ii;
    //puts("asasdasd");
    //arr = (int*)malloc(sizeof(int)*(NPROCESOS-trrasp+1));
    //puts("rtrtrt");
    int c= nl/(NPROCESOS-trrasp);
    int flag = nl - (NPROCESOS-trrasp)*(c-1);

    for(ii=0;ii<nl;ii++){
        arr[ii]=c;
    }

    if(flag>(NPROCESOS-trrasp)){
        flag-= (NPROCESOS-trrasp);
        for(ii=0;ii<flag;ii++){
           arr[ii]+=1;   
        }
    }
    int j=0;
    int dato1=aux+1;
    //puts("pasa");
    for(i=0;i<NPROCESOS;i++){
        if(where!=-1 && enviado(hijos[i])==1)continue;
        info = pvm_initsend(PvmDataDefault);
        //printf("%i\n",info);
        //int dato1 = j *10;
        //puts("pasa");
        int dato2=dato1+arr[j]-1;
        //dato1++;
        //if(i==NPROCESOS-1) dato2+=aux;
        pvm_pkint(&dato1,1,1);
        pvm_pkint(&dato2,1,1);
        pvm_pkstr(PATRON);
        info = pvm_send(hijos[i],1);
        //printf("%i %i\n",i,info);
        //j+=2;
        dato1 = dato2+1;
        j++;
    }
    int contador=0;
    int datos[NPROCESOS];
    for(i=0;i<NPROCESOS;i++){
       // puts("asd");
        pvm_recv(hijos[i],-1);
        pvm_upkint(&datos[i],1,1);
        contador+=datos[i];
    }
    printf("La cantidad de ocurrencias de %s es:\n",PATRON);
    for(i=0;i<NPROCESOS;i++)printf("%i ",datos[i]);
    puts("");
    printf("%i\n",contador);
    pvm_barrier("paralelo", NPROCESOS);
    pvm_lvgroup("paralelo");
    pvm_exit();
    return 0;
}
