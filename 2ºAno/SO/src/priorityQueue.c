/**
 * @file priorityQueue.c
 * @brief Modulo que implementa uma Priority Queue com base numa max heap.
 */
#include <stdlib.h>
#include "priorityQueue.h"
#include <unistd.h>


// Function to insert a new Node
void push(struct no** fr, struct no** rr, struct task *n, int p) {
	struct no* news = malloc(sizeof(struct no));
	news->info = n;
	news->priority = p;

	// If linked list is empty
	if (*fr == NULL) {
		*fr = news;
		*rr = news;
		news->next = NULL;
		news->prev = NULL;
	}
	else {
		// If p is greater than front node's priority, then insert at the front.
		if (p > (*fr)->priority) {
			news->next = *fr;
			(*fr)->prev = news;
			*fr = news;
		}

		// If p is less or equal rear node's priority, then insert after the rear.
		else if (p <= (*rr)->priority) {
			news->next = NULL;
			(*rr)->next = news;
			news->prev = (*rr);
			*rr = news;
		}

		// Handle other cases
		else {

			// Find position where we need to insert.
			struct no* start = (*fr)->next;
			while (start->priority >= p)
				start = start->next;
			(start->prev)->next = news;
			news->next = start;
			news->prev = (start->prev);
			start->prev = news;
		}
	}
}

// Return the value at front
struct task *peek(struct no* fr) { return fr->info; }

int isEmpty(struct no* fr) { return (fr == NULL); }

// Removes the element with the least priority value form the list
struct task *pop(struct no** fr, struct no** rr) {
	struct no* temp = *fr;
	struct task *res = temp->info;
	(*fr) = (*fr)->next;
    (*fr)->prev = NULL;
	free(temp);
	if (*fr == NULL)
		*rr = NULL;
	return res;
}

void freeQueue (struct no **fr) {
	struct no *start = (*fr);
	struct no *aux;

	while (start!=NULL) {
		aux = start;
		start = start->next;
		free(aux);
	}
}

void printQueue(struct no **fr) {
	struct no *start = (*fr);
	
	char buffer1[4096];
    int b1 = snprintf(buffer1,4096,"\n QUEUE \n");
    write(1,buffer1,b1);

	while (start!=NULL) {
		struct task *t = start->info;

		char buffer[4096];
    	int b = snprintf(buffer,4096,"\n prio:%d id:%d, rid:%d, %s %s\n",start->priority,t->id,t->routerid, t->originPath, t->destinyPath);
    	write(1,buffer,b);

		start=start->next;
	}
	

}
