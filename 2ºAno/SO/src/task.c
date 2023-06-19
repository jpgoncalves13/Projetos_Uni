#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "execTransf.h"
#include "task.h"

int TransfN (char *transf) {
    int r = -1;
    if (strcmp(transf, "nop") == 0) r = 0;
    else if (strcmp(transf, "bcompress") == 0) r = 1;
    else if (strcmp(transf, "bdecompress") == 0) r = 2;
    else if (strcmp(transf, "gcompress") == 0) r = 3;
    else if (strcmp(transf, "gdecompress") == 0) r = 4;
    else if (strcmp(transf, "encrypt") == 0) r = 5;
    else if (strcmp(transf, "decrypt") == 0) r = 6;
    return r;
}

struct task *make_task (int id, int routerid, char *operations,char *originPath, char*destinyPath) {
    struct task * task = malloc(sizeof(struct task));
    task->id = id;
    task->routerid = routerid;
    task->destinyPath = strdup(destinyPath);
    task->originPath = strdup(originPath);
    task->operations = strdup(operations);
    task->originPathSize = strlen(originPath);
    task->destinyPathSize = strlen(destinyPath);
    task->inExecution = false;

    int i;
    for (i=0;i<7;i++) task->usedTransf[i]=0;
    
    i = 0;   
    while (operations[i]!='\0') {        
        task->usedTransf[((int) operations[i])-1]+=1;
        i++;
    }
    

    return task;
}

void printTask (struct task* t) {
    char buffer[4096];
    int b = snprintf(buffer,4096,"\n TASK id:%d, rid:%d\n",t->id,t->routerid);
    write(1,buffer,b);
}

void freeTask(struct task *t) {
    free(t->destinyPath);
    free(t->operations);
    free(t->originPath);
    free(t);
}


