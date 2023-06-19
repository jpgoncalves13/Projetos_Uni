/**
 * @file execTransf.h
 * @brief API do modulo de execução de tranformações sobre ficheiros.
 */
#include <stdio.h>
#ifndef EXECTRANSF_H
#define EXECTRANSF_H

/**
 * Função que dado o identificador de cada transformação 
 * retorna uma string com o caminho do binário correspondente.
 */
char *whichTransf (char c);

/**
 * Função que executa pelo menos duas tranformações sobre um ficheiro.
 */ 
int runTransf (unsigned char *transfs,char *originPath, char *destinyPath, char*binPath);


#endif