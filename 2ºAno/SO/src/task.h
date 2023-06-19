#include <stdio.h>
#include <stdbool.h>
#ifndef TASK_H
#define TASK_H

struct task {
    int id;
    size_t nOfOperations;
    size_t originPathSize;
    size_t destinyPathSize;
    int usedTransf[7];
    int routerid;  
    bool inExecution;
    char * operations; // 1-nop, 2-bcompress, 3-bdecompress, 4-gcompress, 5-gdecompress, 6-encrypt, 7-decrypt
    char * originPath;
    char * destinyPath;
};

int TransfN (char *transf);

struct task *make_task (int id, int routerid, char *operations,char *originPath, char*destinyPath);

void printTask (struct task* t);

void freeTask(struct task *t);

#endif
