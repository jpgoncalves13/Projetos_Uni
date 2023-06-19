#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "routerConnections.h"
#include "message.h"
#define MAXBUF 2048

char parseTransf(char * transf){
    char code;
    switch (transf[0]) {
        case 'n':
            code = 1;
            break;
        case 'e':
            code = 6;
            break;
        case 'd':
            code = 7;
            break;
        case 'b':
            code = (transf[1] == 'c') ? 2 : 3 ;
            break;
        case 'g':
            code = (transf[1] == 'c') ? 4 : 5 ;
            break;
        default:
            break;
    }
    return code;
}

int main(int argc, char *argv[]) {
    int ok = (argc>1) ? (!strcmp(argv[1],"proc-file")) * 1 + (!strcmp(argv[1],"status")) * 2 : 0;
    
    if (argc>1 && ok) {
        int i, j, priority = 0, baseIndex = 2, ntransf;
        char *transf;
        struct connection * c = connect();
        switch (ok) {
            case 1: /*PROC-FILE*/
                if (strcmp(argv[2],"-p") == 0){
                    priority = atoi(argv[3]);
                    baseIndex += 2;
                }
                ntransf = argc - (baseIndex + 2);
                transf = malloc(sizeof(char) * ntransf);

                for(i=baseIndex + 2, j=0; i<argc; i++)
                    transf[j++] = parseTransf(argv[i]);
             
                struct m_t2 m = {
                    .priority=priority,
                    .size1=strlen(argv[baseIndex]),
                    .size2=strlen(argv[baseIndex+1]),
                    .size3=ntransf,
                    .OriginPath=argv[baseIndex],
                    .DestinyPath=argv[baseIndex+1],
                    .Transformations=transf
                };
                sendPackage(c->routerFifo,c->id,1,2,&m);
                
                bool notConcluded = true;
                int bytesoutput,fdout, bytes;
                int fdin = open(argv[baseIndex],O_RDONLY);
                int bytesinput = lseek(fdin,0,SEEK_END);
                close(fdin); 

                while(notConcluded){
                    struct package * p = malloc(sizeof(struct package));
                    receive(c->clientFifo->fifoLeitura, p);
                    
                    notConcluded = !(strcmp((char *) p->raw_data, "Concluded") == 0);
                    if (!notConcluded) {
                        fdout = open(argv[baseIndex+1],O_RDONLY);
                        bytesoutput = lseek(fdout,0,SEEK_END);
                        close(fdout);                  
    
                        char status[MAXBUF];
                        bytes = snprintf(status,MAXBUF, "%s (bytes-input: %d, bytes-output: %d)\n", (char *) p->raw_data,bytesinput,bytesoutput);
                        write(STDOUT_FILENO, status, bytes);
                    }
                    else {
                        if (p->type_of_message == 6) notConcluded = false;
                        char status[p->raw_data_size+1];
                        bytes = sprintf(status, "%s\n", (char *) p->raw_data);
                        write(STDOUT_FILENO, status, bytes);
                    }
                    free(p);
                }
                free(transf);
                break;

            case 2: /*STATUS*/
                sendPackage(c->routerFifo,c->id,1,4,NULL);
                struct package * p = malloc(sizeof(struct package));
                receive(c->clientFifo->fds.dir.leitura,p);
                struct m_t4 * m4 = deserialize_m_t4(p->raw_data);
                
                char buf[MAXBUF];
                for (size_t i = 0; i < 7; i++) m4->numTransfNow[i] = m4->numTransfMax[i] - m4->numTransfNow[i]; 
                int size = snprintf(buf, MAXBUF, "transf nop: %d/%d (running/max)\ntransf bcompress: %d/%d (running/max)\ntransf bdecompress: %d/%d (running/max)\ntransf gcompress: %d/%d (running/max)\ntransf gdecompress: %d/%d (running/max)\ntransf encrypt: %d/%d (running/max)\ntransf decrypt: %d/%d (running/max)\n", m4->numTransfNow[0], m4->numTransfMax[0], m4->numTransfNow[1], m4->numTransfMax[1], m4->numTransfNow[2], m4->numTransfMax[2], m4->numTransfNow[3], m4->numTransfMax[3], m4->numTransfNow[4], m4->numTransfMax[4], m4->numTransfNow[5], m4->numTransfMax[5], m4->numTransfNow[6], m4->numTransfMax[6]); 
                write(1, buf, size);
                break;

            default:
                break;
        }
        disconnect(c);
        free(c);
    }
    else if (argc==1 || !ok) {

        char buffer[MAXBUF];
        int n_bytes = snprintf(buffer,MAXBUF,"./sdstore status\n");
        write(1,buffer,n_bytes);

        n_bytes = snprintf(buffer,MAXBUF,"./sdstore proc-file [-p priority] input-filename output-filename transformation-id-1 transformation-id-2 ...\n");
        write(1,buffer,n_bytes);
        
    }

    return 0;
}