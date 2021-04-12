#include  <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
extern UART_HandleTypeDef huart2;
typedef void (*funptr)(void);

struct task{
	funptr function;
    uint16_t priority;
    uint16_t delay;    
};
static void dummy(){}

static const struct task DefaultTask = {&dummy, 9999, 9999};

struct TasksQueue {
	uint16_t ptr;
	uint16_t size;
	struct task* tasks;
};
void enqueueReady(struct TasksQueue* q, struct TasksQueue* mainQ);
void decrementDelay(struct TasksQueue* q);
struct task dequeue(struct TasksQueue* q);
void rearranging(struct TasksQueue* q);
void enqueue(struct TasksQueue* q, struct task newTask);
void enqueueFun(struct TasksQueue* q, funptr fun, uint16_t priority, uint16_t delay);
void initQueue(struct TasksQueue* q, uint16_t size);
void swap(struct task* one, struct task* two);
void swap(struct task* one, struct task* two);
uint8_t comp(struct task one, struct task two);


uint8_t comp(struct task one, struct task two)
{
	// min delay first
	if(one.delay < two.delay) 
        return 1;
	// min priority on tie
	return (one.delay == two.delay) && (one.priority < two.priority);
}

void swap(struct task *one, struct task *two)
{
	struct task temp = *one;
	*one = *two;
	*two = temp;
}

void initQueue(struct TasksQueue * q, uint16_t size)
{	
	q->ptr = 0;
	q->size = size;
	q->tasks = (struct task*)malloc(size * sizeof(struct task));
	for(uint16_t i = 0; i < q->size; i++) 
        q->tasks[i] = DefaultTask;
}

void enqueueFun(struct TasksQueue* q, funptr fun, uint16_t priority, uint16_t delay)
{
	// Create new task and call the private enqueue function
	volatile struct task Temp ={fun, priority, delay*50};
	enqueue(q, Temp);
}

// Enqueue function
void enqueue(struct TasksQueue* q, struct task newTask)
{
	// Check for queue limits
	if (q->ptr == q->size - 1)
		return;
	// put at end of the queue
	q->tasks[q->ptr++] = newTask;
	
	// Go up the tree till it can't overthrow its parent
	int i = q->ptr-1;
	while (i != 0 && comp(q->tasks[i], q->tasks[i-1]))
	{
		swap(&q->tasks[i-1], &q->tasks[i]);
		i = i-1;
	}
}
void rearranging(struct TasksQueue* q)
{
	for(int i=0; i<q->size-1; i++)
		q->tasks[i] = q->tasks[i+1];
}

struct task dequeue(struct TasksQueue* q)
{
	struct task ret = DefaultTask;
	// Check for queue limits
	uint8_t m;
	m = q->ptr + '0';
	//HAL_UART_Transmit(&huart2,(uint8_t *)&m, sizeof(m),200);
	if (q->ptr == 0)
		return ret;
	// Take task reference and replace its reference with the lasr task in the queue
	ret = q->tasks[0];
	// rearrange all the tasks
	q->ptr-=1;
	rearranging(q);
	// return the desired task
	return ret;
}

void decrementDelay(struct TasksQueue* q)
{
	uint8_t m;
	for(uint8_t i = 0; i < q->size; i++)
	{
		q->tasks[i].delay -= 1;
		m = q->tasks[i].delay + '0';
		//HAL_UART_Transmit(&huart2,(uint8_t *)&m, sizeof(m),200);
	}
}

void enqueueReady(struct TasksQueue* q, struct TasksQueue* mainQ)
{
	while(q->ptr >= 0 && q->tasks[0].delay == 0)
		enqueue(mainQ, dequeue(q));
}
