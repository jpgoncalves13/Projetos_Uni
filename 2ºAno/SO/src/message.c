#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "message.h"
#include "fifologic.h"
#include "stdio.h"

#define packageSize sizeof(struct package) - sizeof(unsigned char *)

ssize_t sendPackage(struct fifo * f, int src, int dst, int type, void * data){
    size_t raw_data_size;
    unsigned char * raw_data;
    char * message;
    switch (type)
    {
    case 0:
        message = "kill";
        raw_data = (unsigned char *) strdup(message);
        raw_data_size = strlen(message) + 1;
        break;
    case 1:
        raw_data = serialize_m_t1((struct m_t1 *) data, &raw_data_size);
        break;
    case -1:
        message = "inited";
        raw_data = (unsigned char *) strdup(message);
        raw_data_size = strlen(message) + 1;
        break;
    case 2:
        raw_data = serialize_m_t2((struct m_t2 *) data, &raw_data_size);
        break;
    case -2:
        message = (char *) data;
        raw_data = (unsigned char *) strdup(message);
        raw_data_size = strlen(message) + 1;
        break;
    case 3:
        message = "destroy";
        raw_data = (unsigned char *) strdup(message);
        raw_data_size = strlen(message) + 1;
        break;
    case -3:
        message = "ended";
        raw_data = (unsigned char *) strdup(message);
        raw_data_size = strlen(message) + 1;
        break;
    case -4:
        raw_data = serialize_m_t4((struct m_t4 *) data, &raw_data_size);
        break;
    case 4:
        message = "status";
        raw_data = (unsigned char *) strdup(message);
        raw_data_size = strlen(message);
        break;
    case 5:
        // char buf[100] = {0};
        // sprintf(buf,"pid:%d task:%d\n",((struct m_t5 *) data)->pid,((struct m_t5 *) data)->taskId);
        // write(1,buf,100);
        raw_data = serialize_m_t5((struct m_t5 *) data, &raw_data_size);
        // struct m_t5 * m5 = deserialize_m_t5(raw_data);
        // char buf1[1000] = {0};
        // sprintf(buf1,"BANG       id:%d task:%d\n",m5->pid,m5->taskId);
        // write(1,buf1,1000);
        break;
    case 6:
        message = "refused";
        raw_data = (unsigned char *) strdup(message);
        raw_data_size = strlen(message);
        break;
    default:
        break;
    }
    struct package p = {
        .source = src,
        .destiny = dst,
        .type_of_message = type,
        .raw_data_size=raw_data_size,
        .raw_data=raw_data
    };
    
    ssize_t r = send(f->fifoEscrita,&p);
    free(raw_data);
    return r;
}



//size
//info
//rawdata
ssize_t send(int fd, struct package * p){
    size_t size = packageSize;
    ssize_t r;
    if(size + p->raw_data_size < 1024){
        unsigned char toSend[1024] = { 0 };
        memcpy(toSend, p, size);
        memcpy(toSend + size, p->raw_data, p->raw_data_size);
        // write(STDOUT_FILENO, toSend, nBytes);
        // write(STDOUT_FILENO, "\n", 1);
        r = write(fd, toSend, 1024);
        if(r<0) r = errno;
    } else {
        r = 0;
        write(1, "package too big to send\n", 25);
    }
    return r;
}


//Este recive parece estar mal mas devido a cada fifo so ter um processo 
//a ler dele .Nao existe o problema de outro processo ler daquele fd antes
//entre os nossos reads, uma vez que so existe um processo por fifo a ler(o processo criador do fifo)
// this mallocs the space of the struct because safety
ssize_t receive(int fd, struct package * m){
    size_t sizeToRead=packageSize;
    unsigned char buf[1024];
    ssize_t r = 0;
    if(read(fd, buf, 1024)<0) r = errno;
    else{
        memcpy(m, buf, sizeToRead);
        m->raw_data = calloc(m->raw_data_size, sizeof(unsigned char));
        memcpy(m->raw_data, buf + sizeToRead,m->raw_data_size);
    }
    return r;
}

struct package * makePackage(size_t dataSize, unsigned char * rawData, int source, int destiny, int typeOfMessage){
    struct package * p = malloc(sizeof(struct package));
    p->destiny=destiny;
    p->source=source;
    p->type_of_message=typeOfMessage;
    p->raw_data_size=dataSize;
    p->raw_data=rawData;
    return p;
}

unsigned char * serialize_m_t1(struct m_t1 * m, size_t * bytesUsed){
    int int_size = sizeof(int);
    int size_t_size = sizeof(size_t);
    *bytesUsed = int_size + size_t_size + m->sizeStr;
    unsigned char * serialized = malloc(*bytesUsed);
    memcpy(serialized, &(m->id), int_size);
    memcpy(serialized + int_size, &(m->sizeStr), size_t_size);
    memcpy(serialized + int_size + size_t_size, m->fifo, m->sizeStr);
    return serialized;
}

struct m_t1 * deserialize_m_t1(unsigned char * m){
    int int_size = sizeof(int);
    int size_t_size = sizeof(size_t);
    struct m_t1 * deserialized = malloc(sizeof(struct m_t1));
    memcpy(&(deserialized->id), m, int_size);
    memcpy(&(deserialized->sizeStr), m + int_size, size_t_size);
    deserialized->fifo = malloc(deserialized->sizeStr);
    memcpy(deserialized->fifo, m + int_size + size_t_size, deserialized->sizeStr);
    return deserialized;
}

unsigned char * serialize_m_t2(struct m_t2 * m, size_t * bytesUsed){
    int size_t_size = sizeof(size_t);
    int char_size = sizeof(char);
    *bytesUsed = char_size + 3*size_t_size + m->size1 + m->size2 + m->size3;
    unsigned char * serialized = malloc(*bytesUsed), *s1;
    
    memcpy(serialized, &(m->priority), size_t_size);
    memcpy(serialized + char_size, &(m->size1), size_t_size);
    memcpy(serialized + char_size + size_t_size, &(m->size2), size_t_size);
    memcpy(serialized + char_size + size_t_size * 2, &(m->size3), size_t_size);
    
    s1 = serialized  + char_size + size_t_size * 3;
    memcpy(s1, m->OriginPath, m->size1);
    memcpy(s1 + m->size1, m->DestinyPath, m->size2);
    memcpy(s1 + m->size1 + m->size2, m->Transformations, m->size3);
    return serialized;
}

struct m_t2 * deserialize_m_t2(unsigned char * m){
    int size_t_size = sizeof(size_t);
    int char_size = sizeof(char);
    struct m_t2 * deserialized = malloc(sizeof(struct m_t2));
    unsigned char * s1;
    
    deserialized->priority = *(char*) (m);
    deserialized->size1 = *(size_t*) (m + char_size);
    deserialized->size2 = *(size_t*) (m + char_size + size_t_size);
    deserialized->size3 = *(size_t*) (m + char_size + size_t_size * 2);
    
    s1 = m + char_size + size_t_size * 3;
    deserialized->OriginPath = malloc(deserialized->size1);
    memcpy(deserialized->OriginPath, s1, deserialized->size1);
    
    s1+=deserialized->size1;
    deserialized->DestinyPath = malloc(deserialized->size2);
    memcpy(deserialized->DestinyPath, s1, deserialized->size2);
    
    s1+=deserialized->size2;
    deserialized->Transformations = malloc(deserialized->size3);
    memcpy(deserialized->Transformations, s1, deserialized->size3);

    return deserialized;
}

unsigned char * serialize_m_t4(struct m_t4 * m, size_t * bytesUsed){
    int numTransfMax_size = sizeof(int) * 7;
    int numTransfNow_size = sizeof(int) * 7;
    *bytesUsed = numTransfMax_size + numTransfNow_size;
    unsigned char * serialized = malloc(sizeof(unsigned char) * (*bytesUsed));
    memcpy(serialized, m->numTransfMax, (size_t)numTransfMax_size);
    memcpy(serialized + numTransfMax_size, m->numTransfNow, (size_t)numTransfNow_size);
    return serialized;
}

struct m_t4 * deserialize_m_t4(unsigned char * m){
    int numTransfMax_size = sizeof(int) * 7;
    int numTransfNow_size = sizeof(int) * 7;
    struct m_t4 * deserialized = malloc(sizeof(struct m_t4));
    deserialized->numTransfMax = malloc(numTransfMax_size);
    deserialized->numTransfNow = malloc(numTransfNow_size);
    memcpy(deserialized->numTransfMax, m, (size_t) numTransfMax_size);
    memcpy(deserialized->numTransfNow, m + numTransfMax_size, (size_t) numTransfNow_size);
    return deserialized;
}


unsigned char * serialize_m_t5(struct m_t5 * m, size_t * bytesUsed){
    int int_size = sizeof(int);
    *bytesUsed = int_size * 2;
    unsigned char * serialized = malloc(*bytesUsed);
    memcpy(serialized, m, *bytesUsed);
    return serialized;
}

struct m_t5 * deserialize_m_t5(unsigned char * m){
    int int_size = sizeof(int);
    struct m_t5 * deserialized = malloc(sizeof(struct m_t5));
    memcpy(deserialized, m, int_size * 2);
    return deserialized;
}

// void main(){
//     struct m_t5 m5 = { .pid=1, .taskId=3 },* nm;
//     printf("first:%d second:%d\n", m5.pid, m5.taskId);
//     size_t bu;
//     unsigned char * ser = serialize_m_t5(&m5, &bu);
//     nm = deserialize_m_t5(ser);
//     printf("first:%d second:%d bytes:%zu\n", nm->pid, nm->taskId, bu);
// }