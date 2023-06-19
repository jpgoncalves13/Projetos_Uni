#include <stdbool.h>

#ifndef FIFOLOGIC
#define FIFOLOGIC

#define fifoLeitura fds.dir.leitura
#define fifoEscrita fds.dir.escrita
#define fifoSize sizeof(struct fifo)

typedef union
{
    struct {
        int leitura;
        int escrita;
    } dir;
    int arr[2];
} FF;

typedef struct fifo{
    FF fds;
    char * path;
} Fifo;

char * make_fifo_name(bool isRouter);

struct fifo * open_fifo_wr(char * path);
void close_fifo(struct fifo * f);

struct fifo * create_n_open_fifo(char * path);
void delete_n_close_fifo(struct fifo * f);

#endif