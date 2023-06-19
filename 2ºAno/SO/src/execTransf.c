/**
 * @file execTransf.c
 * @brief Modulo que implementa funções que executam tranformações sobre ficheiros.
 * 
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Função que dado o identificador de cada transformação 
 * retorna uma string com o caminho do binário correspondente.
 * 
 * @param c 
 * @return char* 
 */
char *whichTransf (char c) {
    char *r = NULL;
    switch (c){
        case 1: // nop
            r = "/nop";
            break;
        case 2: // bcompress
            r = "/bcompress";
            break;
        case 3: // bdecompress
            r = "/bdecompress";
            break;
         case 4: // gcompress
            r = "/gcompress";
            break;
         case 5: // gdecompress
            r = "/gdecompress";
            break;
         case 6: // encrypt
            r = "/encrypt";
            break;
         case 7: // decrypt
            r = "/decrypt";
            break;
        default:
            break;
    }
    return r;
}

/**
 * Função que executa uma transformação.
 */
int execTransf (unsigned char *transfs, char *binPath, int i) {
        int res;
        char *trans = whichTransf(transfs[i]);
        char *path = malloc(sizeof(char)*(strlen(trans)+strlen(binPath)+1));
        strcat(path,binPath);
        strcat(path,trans);
        res=execlp(path,path,NULL);
        free(path);
        return res;
}



/**
 * @brief Função que executa pelo menos duas tranformações sobre um ficheiro.
 * 
 * @return Número de bytes escritos no ficheiro resultado.
 * @param transfs 
 * @param originPath 
 * @param destinyPath 
 */
int runTransf (unsigned char *transfs,char *originPath, char *destinyPath, char *binPath) {
    
    int ret = 0, fdin, fdout;
    fdin = open(originPath,O_RDONLY);
    fdout = open(destinyPath,O_WRONLY | O_TRUNC | O_CREAT, 0640);
    if (fdin<0 || fdout<0) ret = -2;
    
    if (ret==0) {
        int i, status, res = 0;
        int n = strlen((char *)transfs);

        if (n==1) {
            pid_t pid;

            if ((pid=fork())==0) {

                dup2(fdin,0);
                dup2(fdout,1);
                close(fdin);
                close(fdout);

                int res = execTransf(transfs,binPath,0);

                _exit(res);
            }
            close(fdin);
            waitpid(pid,&status,0);
            if (WIFEXITED(status)) ret = WEXITSTATUS(status);
            else ret = -3;
        }
        else {

            int pps[n-1][2], pids[n], iPids=0;

            pipe(pps[0]);

            if ((pids[iPids++]=fork())==0) {

                close(pps[0][0]);

                dup2(pps[0][1],1);
                close(pps[0][1]);

                dup2(fdin,0);
                close(fdin);

                res = execTransf(transfs,binPath,0);

                _exit(res);

            }

            close(pps[0][1]);
            close(fdin);

            for(i=1;i<n-1;i++) {

                pipe(pps[i]);

                if ((pids[iPids++]=fork())==0) {

                    close(pps[i][0]);

                    dup2(pps[i][1],1);
                    close(pps[i][1]);
                    dup2(pps[i-1][0],0);
                    close(pps[i-1][0]);

                    res = execTransf(transfs,binPath,i);

                    _exit(res);
                }

                close(pps[i-1][0]);
                close(pps[i][1]);

            }

            if ((pids[iPids++]=fork())==0) {

                dup2(pps[n-2][0],0);
                close(pps[n-2][0]);

                dup2(fdout,1);
                close(fdout);

                res = execTransf(transfs,binPath,n-1);

                _exit(res);

            }

            int guarda = 0;
            for (i=0;i<n;i++) {
                waitpid(pids[i],&status,0);
                if (WIFEXITED(status)) ret = WEXITSTATUS(status);
                else guarda = 1;
            }
            ret = (guarda==1) ? -3 : 0;

        }
    }

    close(fdout);
    return ret;
}
