#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include "fifologic.h"
#include <errno.h>

char * make_fifo_name(bool isRouter){
    char * str;
    if(!isRouter){
        int pid = getpid();
        size_t strSize = (size_t) (( ceil(log10(pid)) + 9 )*sizeof(char));//magia negra de conversao de ints em strings
        str = malloc(strSize);
        sprintf(str, "SO_%i", pid);
    } else str = strdup("SO_Limianos_router");
    return str;
}

struct fifo * open_fifo_wr(char * path){
    struct fifo * r = malloc(sizeof(struct fifo));
    r->path=strdup(path);
    r->fds.dir.leitura = -1;
    r->fds.dir.escrita = open(r->path, O_WRONLY);
    if(r->fds.dir.escrita==-1){
        char * bang = strerror(errno);
        write(STDOUT_FILENO, bang, strlen(bang));
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, r->path, strlen(r->path));
    }
    return r;
}

void close_fifo(struct fifo * f){
    if (f->fds.dir.escrita>1) close(f->fds.dir.escrita);
    if (f->fds.dir.leitura>1) close(f->fds.dir.leitura);
    free(f->path);
    free(f);
}

struct fifo * create_n_open_fifo(char * path){
    struct fifo * r = malloc(sizeof(struct fifo));
    r->path = strdup(path);
    mkfifo(r->path, 0664);
    r->fds.dir.leitura = open(r->path, O_RDONLY);
    if(r->fds.dir.leitura==-1){
        char * bang = strerror(errno);
        write(STDOUT_FILENO, bang, strlen(bang));
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, r->path, strlen(r->path));
    }
    r->fds.dir.escrita = open(r->path, O_WRONLY);
    if(r->fds.dir.escrita==-1){
        char * bang = strerror(errno);
        write(STDOUT_FILENO, bang, strlen(bang));
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, r->path, strlen(r->path));
    }
    //write(r->fds.dir.escrita, buf, 1);
    return r;
}

// struct fifo * create_n_open_fifo(bool isRouter){
//     struct fifo * r = malloc(sizeof(struct fifo));
//     r->path = (isRouter) ? strdup("SO_Limianos_router") : make_fifo_name();
//     int a;
//     while(mkfifo(r->path, 0664)!=0) unlink(r->path);
//     //char buf[1] = {0};
//     //if(fork()==0){
//     //    int f = open(r->path, O_RDONLY);
//     //    read(f, buf, 1);
//     //    close(f);
//     //    _exit(0);
//     //}
//     r->fds.dir.leitura = open(r->path, O_RDONLY);
//     r->fds.dir.escrita = open(r->path, O_WRONLY);   
//     //write(r->fds.dir.escrita, buf, 1);
//     return r;
// }

void delete_n_close_fifo(struct fifo * f){
    close(f->fds.dir.escrita);
    close(f->fds.dir.leitura);
    unlink(f->path);
    free(f->path);
    free(f);
}