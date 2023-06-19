#include "message.h"
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fifologic.h"
#include <string.h>
#include <stdlib.h>
#include "routerConnections.h"

//to change
// fix the fifo creation process... nao da erro mas esta messy
// make more error detection


void disconnect(struct connection *c){
    sendPackage(c->routerFifo, c->id, 0, 3, NULL);
    struct package * p1 = malloc(sizeof(struct package));
    int oldfl = fcntl(c->clientFifo->fds.dir.leitura, F_GETFL);
    fcntl(c->clientFifo->fds.dir.leitura, F_SETFL, oldfl & ~O_NONBLOCK);
    receive(c->clientFifo->fifoLeitura,p1);
    // printf("\npackage:\n    -src:%d\n    -dst:%d\n    -type:%d\n    -sizeRaw:%zu\n", p1->source, p1->destiny, p1->type_of_message, p1->raw_data_size);
    if (p1->type_of_message == -3)
    {
        //delete
        close_fifo(c->routerFifo);
        delete_n_close_fifo(c->clientFifo);
    } else {
        //bang
    }
    free(p1->raw_data);
    free(p1);
}

struct connection * connect(){
    struct connection * c = malloc(sizeof(struct connection));
    char * fifo_name_client = make_fifo_name(false);
    char * fifo_name_router = make_fifo_name(true);
    c->routerFifo = open_fifo_wr(fifo_name_router);
    // size_t bu;
    struct m_t1 m = { .id=-1, .sizeStr=strlen(fifo_name_client) , fifo_name_client};
    // printf("enm_t1:\n    -id:%d\n    -string size:%zu\n    -string:%s\n", m.id, m.sizeStr, m.fifo);
    mkfifo(fifo_name_client, 0664);//fix this
    sendPackage(c->routerFifo, -1, 0, 1, &m);
    c->clientFifo = create_n_open_fifo(fifo_name_client);
    struct package * p1 = malloc(sizeof(struct package));
    // puts("\npipe opened?");
    receive(c->clientFifo->fds.dir.leitura,p1);
    // printf("\npackage:\n    -src:%d\n    -dst:%d\n    -type:%d\n    -sizeRaw:%zu\n", p1->source, p1->destiny, p1->type_of_message, p1->raw_data_size);
    c->id=p1->destiny;
    // free(data);
    free(p1->raw_data);
    free(p1);
    free(fifo_name_client);
    free(fifo_name_router);
    return c;
}


// int main(){
//     // puts("client");
//     struct connection * c = initClient();

    
//     destroyClientFifos(c);

//     // printf("client id:%i", c->id);
//     free(c);
//     return 0;
// }0

void killRouter(struct connection *c){
    sendPackage(c->routerFifo, c->id, 0, 3, NULL);
    struct package * p1 = malloc(sizeof(struct package));
    int oldfl = fcntl(c->clientFifo->fds.dir.leitura, F_GETFL);
    fcntl(c->clientFifo->fds.dir.leitura, F_SETFL, oldfl & ~O_NONBLOCK);
    receive(c->clientFifo->fifoLeitura,p1);
    // printf("\npackage:\n    -src:%d\n    -dst:%d\n    -type:%d\n    -sizeRaw:%zu\n", p1->source, p1->destiny, p1->type_of_message, p1->raw_data_size);
    delete_n_close_fifo(c->clientFifo);
    sendPackage(c->routerFifo, -1, 0, 0, NULL);
    close_fifo(c->routerFifo);
    free(p1->raw_data);
    free(p1);
}