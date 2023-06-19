#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "message.h"
#include "fifologic.h"

#include <stdio.h>

//add parent pid checking for killing router
//add kill message in switch

struct node{
    int index;
    struct node * next;
};

int handle_message_t1(struct package p, struct fifo *** openFifos, int nextFifo, int* maxFifos, struct node ** furos_memoria){
    struct m_t1 * m = deserialize_m_t1(p.raw_data);
    if (nextFifo == *maxFifos){
        *maxFifos = *maxFifos * 2;
        *openFifos = realloc(*openFifos, *maxFifos * sizeof *openFifos);
    }
    
    // printf("\nfifo_r:%s\ntype1\n\n", );
    int usedIndex;
    if (*furos_memoria)
    {
        struct node * toRemove = *furos_memoria;
        *furos_memoria = toRemove->next;
        usedIndex = toRemove->index;
        free(toRemove);   
    }
    else usedIndex = nextFifo++;
    // printf("\nm_t1:\n    id:%d\n    string size:%zu\n    string:%s\n", usedIndex, m->sizeStr, m->fifo);
    (*openFifos)[usedIndex] = open_fifo_wr(m->fifo);
    sendPackage((*openFifos)[usedIndex],0,usedIndex,-1,NULL);
    free(m->fifo);
    free(m);
    return nextFifo;
}

#define make_closed_answer struct package p1 = { \
        .source=0,\
        .destiny=p.source,\
        .type_of_message=-3,\
        .raw_data_size=sizeof("ended"),\
        .raw_data=(unsigned char *) "ended"\
    }

void handle_message_t3(struct package p, struct fifo ** openFifos, struct node ** furos_memoria){
    struct fifo * toClose = openFifos[p.source];
    openFifos[p.source] = NULL;
    
    struct node * newHole = malloc(sizeof(struct node));
    newHole->index = p.source;
    newHole->next = *furos_memoria;
    *furos_memoria = newHole;


    // devido a especificacao desta mensagem o fechar dos fifos e manual
    int talkBackFifo = toClose->fifoEscrita;
    toClose->fifoEscrita = 0;
    make_closed_answer;
    // printf("\npackage:\n    -src:%d\n    -dst:%d\n    -type:%d\n    -sizeRaw:%zu\n", p1.source, p1.destiny, p1.type_of_message, p1.raw_data_size);
    close_fifo(toClose);
    send(talkBackFifo, &p1);
    close(talkBackFifo);
}

void closeAllFifos(struct fifo ** fs, int nextFifo){
    for (size_t i = 1; i < nextFifo ; i++)
    {
        if (fs[i])
        {
            close_fifo(fs[i]);
        }
    }
}

int router(int fd[]){
    int nextFifo = 1, maxFifos = 50;
    struct node * furos_memoria = NULL;
    struct fifo ** openFifos = malloc(maxFifos * sizeof *openFifos);
    char * fname = make_fifo_name(true);
    unlink(fname);
    mkfifo(fname, 0664);
    close(fd[0]);
    write(fd[1], "1", 1);
    close(fd[1]);
    openFifos[0] = create_n_open_fifo(fname);
    int brk = false;
    while (true)
    {
        struct package * p = malloc(sizeof(struct package));
        receive(openFifos[0]->fifoLeitura, p);
        switch (p->type_of_message)
        {
        case 0:
            if (p->source == -1) brk = true;
            break;
        case 1:
            nextFifo = handle_message_t1(*p,&openFifos, nextFifo, &maxFifos, &furos_memoria);   
            break;
        case 3:
            //printf("ROUTER: terminate connection with client %d\n", p->source);
            if(p->source != 0) handle_message_t3(*p, openFifos,&furos_memoria);
            break;
        default:
            if (p->destiny != 0) send(openFifos[p->destiny]->fifoEscrita,p);
            break;
        }
        free(p);
        if (brk) break;
    }
    closeAllFifos(openFifos, nextFifo);
    delete_n_close_fifo(openFifos[0]);
    return 1;
}