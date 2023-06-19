#include "fifologic.h"


struct connection{
    int id;
    struct fifo * routerFifo;
    struct fifo * clientFifo;
};

void disconnect(struct connection *c);
struct connection * connect();
void killRouter(struct connection *c);