/**
 * @file priorityQueue.h
 * @brief API do modulo de uma Priority Queue.
 */
#include <stdio.h>
#include "task.h"
#ifndef PRIORITYQEUEU_H
#define PRIORITYQEUEU_H

/**
 * Estrutura de dados que define a Priority Queue.
 */
typedef struct no {
    struct task *info;
    int priority;
    struct no *prev, *next;
} Node;

void push(struct no** fr, struct no** rr, struct task *n, int p);

struct task *pop(struct no** fr, struct no** rr);

int isEmpty(struct no* fr);

struct task *peek(struct no* fr);

void printQueue(struct no **fr);

#endif