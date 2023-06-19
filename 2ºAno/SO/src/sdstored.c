#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "priorityQueue.h"
#include "execTransf.h"
#include "task.h"
#include "router.h"
#include "message.h"
#include "fifologic.h"
#include "routerConnections.h"
#include <errno.h>
#include <signal.h>

#define MAX_BUF 1024
#define MAX_AUX 10
char aux[MAX_AUX];
int pos=0,end=0;
bool dieGracefully = false;

int readc (int fd, char *c) {                      
    if (pos == end) {
        end = read(fd,aux,MAX_AUX);
        switch (end) {
            case -1: return -1;
                break;
            case 0: return 0;
                break;
            default: pos=0; 
        }
    }

    *c = aux[pos++];

    return 1;
}


ssize_t readln (int fd, char *line, size_t size) { 
    int i = 0;
    int bytes_read = 0; 

    while (bytes_read < size && (bytes_read += readc(fd,&line[i])) > 0) {
        if (line[i]=='\n') break;
        i++;
    }   

    return bytes_read; 
}

int tasksToExecute (int *nTasksRunnig,struct connection *c,Node **fr, int *numTransfNow, char *binpath) {

    int j, ready = 1;
    pid_t pid;

    struct no *start = *fr;

    while (start!=NULL) {

        struct task *task = start->info;
        struct no *n;

        for(j=0, ready=1;j<7 && ready;j++) 
            if (numTransfNow[j] < task->usedTransf[j]) ready=0;

        if (ready) {  
            task->inExecution = true;
            (*nTasksRunnig)++; 
            sendPackage(c->routerFifo,c->id,task->routerid,-2,(void *) "Processing");

            n = start;
            if (start->prev == NULL && start->next == NULL) {
                start=NULL;
                (*fr) = NULL;
            }
            else if (start->prev == NULL) {
                start->next->prev = NULL;
                (*fr) = start->next;
            }
            else if (start->next == NULL) {
                start->prev->next = NULL;
            }
            else {
                start->prev->next = start->next; 
                start->next->prev = start->prev;
            }

            for(j=0;j<7;j++) numTransfNow[j]-=task->usedTransf[j];
            if ((pid=fork())==0) {
                runTransf((unsigned char *) task->operations,task->originPath,task->destinyPath,binpath);
                struct connection * c = connect();
                struct m_t5 m5 = { .pid=getpid(), .taskId=task->id };
                sendPackage(c->routerFifo,c->id, 1, 5,(void *) &m5);
                disconnect(c);
                _exit(0);
            }
        }

        if (start!=NULL) start=start->next;
        //if (ready) free(n);
    }

    return pid;
}

int startDetachedRouter(int fd[]){
    int r = fork();
    if(r == 0){
        router(fd);
        _exit(0);
    }
    char buf[1];
    close(fd[1]);
    read(fd[0], buf, 1);
    close(fd[0]);
    return r;
}

void manage_SIGTERM(int signum){
    dieGracefully = true;
}

int main(int argc, char *argv[]) {
    struct sigaction act;
    act.sa_handler = &manage_SIGTERM;
    if (sigaction(SIGTERM, &act,NULL)==-1){
        char *error = strerror(errno);
        write(STDOUT_FILENO,error,strlen(error));
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd<0) return 1;

    int numTransfMax[7], numTransfNow[7], nTasksRunning = 0;
    char buffer[MAX_BUF], *aux, *p, *p1;
    int n_bytes;

    while ((n_bytes = readln(fd,buffer,MAX_BUF))>0) {
        
        aux = buffer;
        p = strsep(&aux," \n");
        p1 = strsep(&aux," \n");

        numTransfMax[TransfN(p)] = atoi(p1);
        numTransfNow[TransfN(p)] = atoi(p1);
    }

    int pfd[2];
    pipe(pfd);
    int routerPid = startDetachedRouter(pfd);
    struct connection * c = connect();

    struct no *front = NULL, *rear = NULL;
    int status;

    int iTasks=0;
    int arrayTSize = 128;
    struct task **arrayT = malloc ((sizeof(struct task *))*arrayTSize);
 
    ssize_t r;
    do {
        struct package * p = malloc(sizeof(struct package));

        r = receive(c->clientFifo->fds.dir.leitura, p);
        if (r==0){
            int type = p->type_of_message;
            struct m_t2 * m2;
            struct task * t;
            struct m_t4 * m4;
            struct m_t5 * m5;
            int waitForThisPid;
            switch (type) {
                case 2:
                    m2 = deserialize_m_t2(p->raw_data);
                    t = make_task(iTasks,p->source,m2->Transformations,m2->OriginPath,m2->DestinyPath); 
                    
                    int j, ready;                   
                    for(j=0, ready=1;j<7 && ready;j++) {
                        if (numTransfMax[j] < t->usedTransf[j]) ready=0;  
                    }
                    
                    if (ready) {
                        if (arrayTSize == iTasks) {
                            arrayTSize*=2;
                            arrayT = realloc(arrayT,arrayTSize*(sizeof(struct task *)));
                        }   
                        arrayT[iTasks++] = t;
                        push(&front,&rear,t,m2->priority);
                        sendPackage(c->routerFifo,c->id,t->routerid,-2,(void *) "Pending");

                        tasksToExecute(&nTasksRunning,c,&front,numTransfNow,argv[2]);
                    }
                    else {
                        sendPackage(c->routerFifo, c->id, p->source, 6, NULL);
                    }
                    free(m2);
                    break;
                case 4:
                    m4 = malloc(sizeof(struct m_t4)); 
                    m4->numTransfNow = numTransfNow;
                    m4->numTransfMax = numTransfMax;
                    sendPackage(c->routerFifo,c->id,p->source,-4,m4);
                    free(m4);
                    break;
                case 5:
                    m5 = deserialize_m_t5(p->raw_data);
                    waitForThisPid = m5->pid;
                    waitpid(waitForThisPid ,&status ,0);
                    nTasksRunning--;

                    int taskIndex = m5->taskId;
                    struct task *toRemove = arrayT[taskIndex]; 
                    arrayT[taskIndex] = NULL;

                    sendPackage(c->routerFifo,c->id,toRemove->routerid,-2,(void *) "Concluded");
                    int i;
                    for(i=0;i<7;i++) numTransfNow[i]+= toRemove->usedTransf[i];
                    //freeTask(toRemove);

                    tasksToExecute(&nTasksRunning,c,&front,numTransfNow,argv[2]);
                    break;
                default:
                    break;
                }
        } else if (r!=EINTR) { 
            write(1, "Server received error message\n", 31);
        }
        free(p);
    } while (!dieGracefully);
    
    bool notFinished = (front == NULL && nTasksRunning==0) ? false : true;

    while (notFinished) {
        struct package * p = malloc(sizeof(struct package));
        r = receive(c->clientFifo->fds.dir.leitura, p);
        if (r==0){
            int type = p->type_of_message;
            struct m_t4 * m4;
            struct m_t5 * m5;
            int waitForThisPid;
            switch (type) {
            case 2:
                sendPackage(c->routerFifo, c->id, p->source, 6, NULL);
                break;      
            case 4:
                m4 = malloc(sizeof(struct m_t4)); 
                m4->numTransfNow = numTransfNow;
                m4->numTransfMax = numTransfMax;
                sendPackage(c->routerFifo,c->id,p->source,-4,m4);
                free(m4);
                break;
            case 5:
                m5 = deserialize_m_t5(p->raw_data);
                waitForThisPid = m5->pid;
                waitpid(waitForThisPid ,&status ,0);
                nTasksRunning--;
                if (!nTasksRunning) notFinished=false;
                
                int taskIndex = m5->taskId;
                struct task *toRemove = arrayT[taskIndex]; 
                arrayT[taskIndex] = NULL;
                
                sendPackage(c->routerFifo,c->id,toRemove->routerid,-2,(void *) "Concluded");

                int i;
                for(i=0;i<7;i++) numTransfNow[i]+= toRemove->usedTransf[i];
                freeTask(toRemove);

                tasksToExecute(&nTasksRunning,c,&front,numTransfNow,argv[2]);
                break;
            default:
                break;
            }
        } else {
            write(1, "Server received error message while terminating\n", 49);
        }
        free(p);
    }

    free(arrayT);
    killRouter(c);
    free(c);
    close(fd);
    waitpid(routerPid, NULL, 0);
    return 0;
}