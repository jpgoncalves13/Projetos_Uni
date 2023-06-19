#include <stdlib.h>
#include "fifologic.h"

struct package{
    size_t raw_data_size;
    int source;
    int destiny;
    int type_of_message;
    unsigned char * raw_data;
};

struct m_t1{
    int id;
    size_t sizeStr;
    char * fifo;
};


struct m_t2{
    char priority;
    size_t size1;
    size_t size2;
    size_t size3;
    char * OriginPath;
    char * DestinyPath;
    char * Transformations;
};

// struct m_t2_r{
//     int status;
// };

struct m_t4{
    int * numTransfMax;
    int * numTransfNow;
};

struct m_t5{
    int pid;
    int taskId;
};

unsigned char * serialize_m_t4(struct m_t4 * m, size_t * bytesUsed);

struct m_t4 * deserialize_m_t4(unsigned char * m);

//tipo 1_r, 3 e 3_r nao precisam de struct pois a informacao do package chega

ssize_t sendPackage(struct fifo * f, int src, int dst, int type, void * data);

ssize_t send(int fd, struct package * m);

// this mallocs the space of the struct because safety
ssize_t receive(int fd, struct package * m);

struct package * makePackage(size_t dataSize, unsigned char * rawData, int source, int destiny, int typeOfMessage);

unsigned char * serialize_m_t1(struct m_t1 * m, size_t * bytesUsed);

struct m_t1 * deserialize_m_t1(unsigned char * m);

unsigned char * serialize_m_t2(struct m_t2 * m, size_t * bytesUsed);

struct m_t2 * deserialize_m_t2(unsigned char * m);

unsigned char * serialize_m_t5(struct m_t5 * m, size_t * bytesUsed);

struct m_t5 * deserialize_m_t5(unsigned char * m);