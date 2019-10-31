
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/queue.h>

#define DEBUG 0
#define MAX_LINE_LENGTH 100

#define FCFS 0
#define RR   1
#define PS   2
#define MLFQ 3
#define BUMP_UP 200

#define PRIORITY_LEVELS 4


/*
 * Stores raw event data from the input,
 * and has spots for per-task statistics.
 */

typedef struct Task_t {
    int   arrival_time;
    float length;
    int   priority;

    float finish_time;
    int   schedulings;
    float cpu_cycles;
} task_t;


/*
 * Some function prototypes.
 */

void read_task_data(void);
void init_simulation_data(int);
void first_come_first_serve(void);
void round_robin(int);
void priority_scheduling(void);
void mlfq_scheduling(void);
void run_simulation(int, int);
void compute_and_print_stats(void);

/*
 * Some global vars.
 */
int     num_tasks = 0;
int quantum;
task_t *tasks = NULL;

void read_task_data()
{
    int max_tasks = 2;
    int  in_task_num, in_task_arrival, in_task_priority;
    float in_task_length;


    assert( tasks == NULL );

    tasks = (task_t *)malloc(sizeof(task_t) * max_tasks);
    if (tasks == NULL) {
        fprintf(stderr, "error: malloc failure in read_task_data()\n");
        exit(1);
    }

    num_tasks = 0;

    /* Given the format of the input is strictly formatted,
     * we can used fscanf .
     */
    while (!feof(stdin)) {
        fscanf(stdin, "%d %d %f %d\n", &in_task_num,
            &in_task_arrival, &in_task_length, &in_task_priority);
        assert(num_tasks == in_task_num);
        tasks[num_tasks].arrival_time = in_task_arrival;
        tasks[num_tasks].length       = in_task_length;
        tasks[num_tasks].priority     = in_task_priority;

        num_tasks++;
        if (num_tasks >= max_tasks) {
            max_tasks *= 2;
            tasks = (task_t *)realloc(tasks, sizeof(task_t) * max_tasks);
            if (tasks == NULL) {
                fprintf(stderr, "error: malloc failure in read_task_data()\n");
                exit(1);
            }
        }
    }
}


void init_simulation_data(int algorithm)
{
    int i;

    for (i = 0; i < num_tasks; i++) {
        tasks[i].finish_time = 0.0;
        tasks[i].schedulings = 0;
        tasks[i].cpu_cycles = 0.0;
    }
}

void first_come_first_serve()
{
    int current_task = 0;
    int current_tick = 0;

    printf("\n\tFirst Come first serve\n");

    for (;;)
    {
        current_tick++;

        if (current_task >= num_tasks)
        {
            break;
        }

        /*
         * Is there even a job here???
         */
        if (tasks[current_task].arrival_time > current_tick-1) {
            continue;
        }

        tasks[current_task].cpu_cycles += 1.0;

        if (tasks[current_task].cpu_cycles >= tasks[current_task].length)
        {
            float quantum_fragment = tasks[current_task].cpu_cycles -
                tasks[current_task].length;
            tasks[current_task].cpu_cycles = tasks[current_task].length;
            tasks[current_task].finish_time = current_tick - quantum_fragment;
            tasks[current_task].schedulings = 1;
            current_task++;
            if (current_task > num_tasks)
            {
                break;
            }
            tasks[current_task].cpu_cycles += quantum_fragment;
        }
    }
}

/*
My code goes here
*/

//Checks if the queue is empty
int isEmpty(int front, int rear)
{
	if(front == rear)
	{
		return 1;
        }
	else
	{
		return 0;
	}
}

//Adds an element to the queue
void enqueue(int queue[], int front, int *rear, int value)
{
	*rear= (*rear +1) % (num_tasks + 1);
  	if(*rear == front && isEmpty(front,*rear))
  	{
		printf("The queue is full\n");
  	}
	queue[*rear] = value;
}

//Removes front from the queue and returns it
int dequeue(int queue[], int *front, int rear)
{
	if(*front == rear)
	{
		printf("The queue is empty\n");
        	exit(0);
        }
        *front = (*front + 1) % (num_tasks + 1);
        return queue[*front];
}

void round_robin(int quantum)
{
	printf("ROUND ROBIN\n");

	int RR_queue[num_tasks + 1];
   	int RR_front,RR_rear;
   	RR_front=0;
   	RR_rear=0;

	int current_task = 0;
	int working_task = 0;
	float current_tick = 0;

	for(;;)
	{

		if (current_task >= num_tasks  && isEmpty(RR_front, RR_rear) == 1)
		{
            		break;
        	}

        	while(tasks[current_task].arrival_time <= current_tick && current_task < num_tasks)
        	{
        		if(DEBUG == 1)
        		{
				printf("\nTask %d arrival time: %d.\n", current_task , tasks[current_task].arrival_time);
				printf("\tCurrent tick: %f\n", current_tick);
				printf("\tAdded Task %d to the Queue.\n", current_task);
			}
        		enqueue(RR_queue, RR_front, &RR_rear, current_task);
        		current_task++;
        	}

        	if(isEmpty(RR_front, RR_rear) == 1)
        	{
        		current_tick++;
        		continue;
        	}
        	else
        	{
        		working_task = dequeue(RR_queue, &RR_front, RR_rear);

        		tasks[working_task].cpu_cycles += quantum;
        		tasks[working_task].schedulings++;
        		current_tick += quantum;

			if(DEBUG == 1)
        		{
        			printf("\nWorking on task: %d\n", working_task);
        			printf("\tCurrent tick: %f\n", current_tick);
				printf("\tTask %d cpu cycles: %f\n", working_task, tasks[working_task].cpu_cycles);
        		}


        		if(tasks[working_task].cpu_cycles >= tasks[working_task].length)
        		{
        			float quantum_fragment = tasks[working_task].cpu_cycles - tasks[working_task].length;
                		tasks[working_task].finish_time = current_tick - quantum_fragment;
                		tasks[working_task].cpu_cycles -= quantum_fragment;
        			current_tick -= quantum_fragment;
        			if(DEBUG == 1)
        			{
        				printf("Completing task: %d***********\n", working_task);
        				printf("\tQuantum Fragment: %f\n", quantum_fragment);
        				printf("\tTask %d quantams to finish: %f\n", working_task, tasks[working_task].length);
        				printf("\tTask %d new cpu cycles: %f\n", working_task, tasks[working_task].cpu_cycles);
        				printf("\tTask %d Finish time: %f\n", working_task, tasks[working_task].finish_time);
        				printf("\tCurrent tick: %f\n", current_tick);
        			}
        		}
        		else
        		{
        			while(tasks[current_task].arrival_time <= current_tick && current_task < num_tasks)
				{
					if(DEBUG == 1)
					{
						printf("\nTask %d arrival time: %d.\n", current_task , tasks[current_task].arrival_time);
						printf("\tCurrent tick: %f\n", current_tick);
						printf("\tAdded Task %d to the Queue.\n", current_task);
					}
					enqueue(RR_queue, RR_front, &RR_rear, current_task);
					current_task++;
				}
        			enqueue(RR_queue, RR_front, &RR_rear, working_task);
        		}
        	}

	}
}



void priority_scheduling()
{
	printf("PRIORITY SCHEDULING");

	int P0_queue[num_tasks + 1];
	int P0_front,P0_rear;
	P0_front = 0; P0_rear = 0;

	int P1_queue[num_tasks + 1];
	int P1_front,P1_rear;
	P1_front = 0; P1_rear = 0;

	int P2_queue[num_tasks + 1];
	int P2_front,P2_rear;
	P2_front = 0; P2_rear = 0;

	int P3_queue[num_tasks + 1];
	int P3_front,P3_rear;
	P3_front= 0; P3_rear = 0;

	int current_task = 0;
	int working_task = 0;
	float current_tick = 0;

	for(;;)
	{
		current_tick++;
		if (current_task >= num_tasks  && isEmpty(P0_front, P0_rear) == 1 && isEmpty(P1_front, P1_rear) == 1 && isEmpty(P2_front, P2_rear) == 1 && isEmpty(P3_front, P3_rear) == 1)
		{
            		break;
        }

		while(tasks[current_task].arrival_time <= current_tick && current_task < num_tasks)
        {
			if(tasks[current_task].priority == 0)
			{
				enqueue(P0_queue, P0_front, &P0_rear, current_task);
			}
			else if(tasks[current_task].priority == 1)
			{
				enqueue(P1_queue, P1_front, &P1_rear, current_task);
			}
			else if(tasks[current_task].priority == 2)
			{
				enqueue(P2_queue, P2_front, &P2_rear, current_task);
			}
			else if(tasks[current_task].priority == 3)
			{
				enqueue(P3_queue, P3_front, &P3_rear, current_task);
			}
			current_task++;
		}

		if(isEmpty(P0_front, P0_rear) == 1 && isEmpty(P1_front, P1_rear) == 1 && isEmpty(P2_front, P2_rear) == 1 && isEmpty(P3_front, P3_rear) == 1)
		{
			continue;
		}

		if(isEmpty(P0_front, P0_rear) == 0)
		{
			working_task = dequeue(P0_queue, &P0_front, P0_rear);
		}
		else if(isEmpty(P1_front, P1_rear) == 0)
		{
			working_task = dequeue(P1_queue, &P1_front, P1_rear);
		}
		else if(isEmpty(P2_front, P2_rear) == 0)
		{
			working_task = dequeue(P2_queue, &P2_front, P2_rear);
		}
		else if(isEmpty(P3_front, P3_rear) == 0)
		{
			working_task = dequeue(P3_queue, &P3_front, P3_rear);
		}

		tasks[working_task].cpu_cycles += 1.0;
		tasks[working_task].schedulings++;
		//current_tick += quantum;

		if(DEBUG == 1)
		{
			printf("\nWorking on task: %d\n", working_task);
			printf("\tCurrent tick: %f\n", current_tick);
			printf("\tTask %d cpu cycles: %f\n", working_task, tasks[working_task].cpu_cycles);
			printf("\tTask %d's Priority: %d\n",working_task ,tasks[working_task].priority);
		}


		if(tasks[working_task].cpu_cycles >= tasks[working_task].length)
		{
			float quantum_fragment = tasks[working_task].cpu_cycles - tasks[working_task].length;
        		tasks[working_task].finish_time = current_tick - quantum_fragment;
        		tasks[working_task].cpu_cycles -= quantum_fragment;
			//current_tick -= quantum_fragment;
			if(DEBUG == 1)
			{
				printf("Completing task: %d***********\n", working_task);
				printf("\tQuantum Fragment: %f\n", quantum_fragment);
				printf("\tTask %d quantams to finish: %f\n", working_task, tasks[working_task].length);
				printf("\tTask %d new cpu cycles: %f\n", working_task, tasks[working_task].cpu_cycles);
				printf("\tTask %d Finish time: %f\n", working_task, tasks[working_task].finish_time);
				printf("\tCurrent tick: %f\n", current_tick);
			}
		}
		else
		{
			while(tasks[current_task].arrival_time <= current_tick && current_task < num_tasks)
			{
				if(DEBUG == 1)
				{
					printf("\nTask %d arrival time: %d.\n", current_task , tasks[current_task].arrival_time);
					printf("\tCurrent tick: %f\n", current_tick);
					printf("\tAdded Task %d to the %d Priority Queue.\n",tasks[current_task].priority, current_task);
					printf("\tTask %d's Priority: %d\n",current_task ,tasks[current_task].priority);
				}
				if(tasks[current_task].priority == 0)
				{
					enqueue(P0_queue, P0_front, &P0_rear, current_task);
				}
				else if(tasks[current_task].priority == 1)
				{
					enqueue(P1_queue, P1_front, &P1_rear, current_task);
				}
				else if(tasks[current_task].priority == 2)
				{
					enqueue(P2_queue, P2_front, &P2_rear, current_task);
				}
				else if(tasks[current_task].priority == 3)
				{
					enqueue(P3_queue, P3_front, &P3_rear, current_task);
				}
				current_task++;
			}



			if(tasks[working_task].priority == 0)
			{
				enqueue(P0_queue, P0_front, &P0_rear, working_task);
			}
			else if(tasks[working_task].priority == 1)
			{
				enqueue(P1_queue, P1_front, &P1_rear, working_task);
			}
			else if(tasks[working_task].priority == 2)
			{
				enqueue(P2_queue, P2_front, &P2_rear, working_task);
			}
			else if(tasks[working_task].priority == 3)
			{
				enqueue(P3_queue, P3_front, &P3_rear, working_task);
			}
		}

	}
}

void mlfq_scheduling()
{
	printf("MLFQ SCHEDULING\n");

	int P0_queue[num_tasks + 1];
	int P0_front,P0_rear;
	P0_front = 0; P0_rear = 0;

	int P1_queue[num_tasks + 1];
	int P1_front,P1_rear;
	P1_front = 0; P1_rear = 0;

	int P2_queue[num_tasks + 1];
	int P2_front,P2_rear;
	P2_front = 0; P2_rear = 0;

	int P3_queue[num_tasks + 1];
	int P3_front,P3_rear;
	P3_front= 0; P3_rear = 0;

	int current_task = 0;
	int working_task = 0;
	float current_tick = 0;
	float last_bump = 0;
	int temp_task;

	int priority_levels_queue[num_tasks];
	int i;
	for(i = 0; i < num_tasks; i++)
	{
		priority_levels_queue[i] = tasks[i].priority;
	}

	for(;;)
	{
		//Check if we are done
		if (current_task >= num_tasks  && isEmpty(P0_front, P0_rear) == 1 && isEmpty(P1_front, P1_rear) == 1 && isEmpty(P2_front, P2_rear) == 1 && isEmpty(P3_front, P3_rear) == 1)
		{
			for(i = 0; i < num_tasks; i++)
			{
				 tasks[i].priority = priority_levels_queue[i];
			}
            	break;
        	}

        	//Do a bump up every BUMP_UP cycles
		if(last_bump <= (current_tick - BUMP_UP))
		{
			if(DEBUG == 1)
			{
				printf("\n\tBUMPING!\n");
				printf("\tLast Bump: %f\n",last_bump);
				printf("\tCurrent tick - Bumpup: %f\n",(current_tick - BUMP_UP));
			}
			last_bump = current_tick;
			while(isEmpty(P1_front, P1_rear) == 0)
			{
				temp_task = dequeue(P1_queue, &P1_front, P1_rear);
				enqueue(P0_queue, P0_front, &P0_rear, temp_task);
				if(DEBUG == 1)
				{
					printf("\n\tBumping Task %d from priority %d -> 0",temp_task,tasks[temp_task].priority);
				}
				tasks[temp_task].priority = 0;
			}
			while(isEmpty(P2_front, P2_rear) == 0)
			{
				temp_task = dequeue(P2_queue, &P2_front, P2_rear);
				enqueue(P1_queue, P1_front, &P1_rear, temp_task);
				if(DEBUG == 1)
				{
					printf("\n\tBumping Task %d from priority %d -> 1",temp_task,tasks[temp_task].priority);
				}
				tasks[temp_task].priority = 1;
			}
			while(isEmpty(P3_front, P3_rear) == 0)
			{
				temp_task = dequeue(P3_queue, &P3_front, P3_rear);
				enqueue(P2_queue, P2_front, &P2_rear, temp_task);
				if(DEBUG == 1)
				{
					printf("\n\tBumping Task %d from priority %d -> 2",temp_task,tasks[temp_task].priority);
				}
				tasks[temp_task].priority = 2;
			}
		}

		//Add a task to the queue if it showed up
		while(tasks[current_task].arrival_time <= current_tick && current_task < num_tasks)
        	{
			enqueue(P0_queue, P0_front, &P0_rear, current_task);
			tasks[current_task].priority = 0;
			if(DEBUG == 1)
        		{
				printf("\nTask %d arrival time: %d.\n", current_task , tasks[current_task].arrival_time);
				printf("\tCurrent tick: %f\n", current_tick);
				printf("\tAdded Task %d to the %d Priority Queue.\n",current_task, tasks[current_task].priority);
				printf("\tTask %d's Priority: %d\n",current_task ,tasks[current_task].priority);
			}
			current_task++;
		}

		//If every queue is empty
		if(isEmpty(P0_front, P0_rear) == 1 && isEmpty(P1_front, P1_rear) == 1 && isEmpty(P2_front, P2_rear) == 1 && isEmpty(P3_front, P3_rear) == 1)
		{
			current_tick++;
			continue;
		}

		//Otherwise work with the top task
		//Dequeue The Working Task
		int temp_quantum = quantum;
		if(isEmpty(P0_front, P0_rear) == 0)
		{
			working_task = dequeue(P0_queue, &P0_front, P0_rear);
		}
		else if(isEmpty(P1_front, P1_rear) == 0)
		{
			working_task = dequeue(P1_queue, &P1_front, P1_rear);
			temp_quantum *= 2;
		}
		else if(isEmpty(P2_front, P2_rear) == 0)
		{
			working_task = dequeue(P2_queue, &P2_front, P2_rear);
			temp_quantum *= 4;
		}
		else if(isEmpty(P3_front, P3_rear) == 0)
		{
			working_task = dequeue(P3_queue, &P3_front, P3_rear);
			temp_quantum *= 8;
		}

		tasks[working_task].cpu_cycles += temp_quantum;
		tasks[working_task].schedulings++;
		current_tick += temp_quantum;

		if(DEBUG == 1)
		{
			printf("\nWorking on task: %d\n", working_task);
			printf("\tCurrent tick: %f\n", current_tick);
			printf("\tTask %d cpu cycles: %f\n", working_task, tasks[working_task].cpu_cycles);
			printf("\tTask %d's Priority: %d\n",working_task ,tasks[working_task].priority);
		}

		//Check if the task is completed
		if(tasks[working_task].cpu_cycles >= tasks[working_task].length)
		{
			float quantum_fragment = tasks[working_task].cpu_cycles - tasks[working_task].length;
        		tasks[working_task].finish_time = current_tick - quantum_fragment;
        		tasks[working_task].cpu_cycles -= quantum_fragment;
			current_tick -= quantum_fragment;
			if(DEBUG == 1)
			{
				printf("Completing task: %d***********\n", working_task);
				printf("\tQuantum Fragment: %f\n", quantum_fragment);
				printf("\tTask %d quantams to finish: %f\n", working_task, tasks[working_task].length);
				printf("\tTask %d new cpu cycles: %f\n", working_task, tasks[working_task].cpu_cycles);
				printf("\tTask %d Finish time: %f\n", working_task, tasks[working_task].finish_time);
				printf("\tCurrent tick: %f\n", current_tick);
			}
		}
		//If it hasn't completed
		else
		{
			//Move the working task down a priority unless its already at the bottom
			if(tasks[working_task].priority != 3)
			{
				tasks[working_task].priority++;
				if(DEBUG == 1)
				{
					printf("\tChanged Task %d's priority to: %d\n", working_task,tasks[working_task].priority);
				}
			}
			//Then check if there are any new tasks
			while(tasks[current_task].arrival_time <= current_tick && current_task < num_tasks)
			{
				enqueue(P0_queue, P0_front, &P0_rear, current_task);
				tasks[current_task].priority = 0;
				if(DEBUG == 1)
				{
					printf("\nTask %d arrival time: %d.\n", current_task , tasks[current_task].arrival_time);
					printf("\tCurrent tick: %f\n", current_tick);
					printf("\tAdded Task %d to the %d Priority Queue.\n",current_task, tasks[current_task].priority);
					printf("\tTask %d's Priority: %d\n",current_task ,tasks[current_task].priority);
				}
				current_task++;
			}
			//Then re-enqueue the working task
			if(tasks[working_task].priority == 0)
			{
				enqueue(P0_queue, P0_front, &P0_rear, working_task);
			}
			else if(tasks[working_task].priority == 1)
			{
				enqueue(P1_queue, P1_front, &P1_rear, working_task);
			}
			else if(tasks[working_task].priority == 2)
			{
				enqueue(P2_queue, P2_front, &P2_rear, working_task);
			}
			else if(tasks[working_task].priority == 3)
			{
				enqueue(P3_queue, P3_front, &P3_rear, working_task);
			}
		}

	}
}

#define amountOne 5
#define amountTwo 10
#define amountThree 25
#define amountFour 50
#define amountFive 100
#define amountSix 150
#define amountSeven 200
#define amountEight 250

void print_graph()
{
	int level1 = 0;
	int l1 = 0;
	int level2 = 0;
	int l2 = 0;
	int level3 = 0;
	int l3 = 0;
	int level4 = 0;
	int l4 = 0;
	int level5 = 0;
	int l5 = 0;
	int level6 = 0;
	int l6 = 0;
	int level7 = 0;
	int l7 = 0;
	int level8 = 0;
	int l8 = 0;
	int i;

	for(i = 0; i < num_tasks; i++)
	{
		 if(tasks[i].length <= amountOne)
		 {
		 	level1 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l1++;
		 }
		 else if(amountOne < tasks[i].length && tasks[i].length <= amountTwo)
		 {
		 	level2 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l2++;
		 }
		 else if(amountTwo < tasks[i].length && tasks[i].length < amountThree)
		 {
		 	level3 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l3++;
		 }
		 else if(amountThree < tasks[i].length && tasks[i].length < amountFour)
		 {
		 	level4 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l4++;
		 }
		 else if(amountFour < tasks[i].length && tasks[i].length < amountFive)
		 {
		 	level5 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l5++;
		 }
		  else if(amountFive < tasks[i].length && tasks[i].length < amountSix)
		 {
		 	level6 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l6++;
		 }
		 else if(amountSix < tasks[i].length && tasks[i].length < amountSeven)
		 {
		 	level7 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l7++;
		 }
		 else if(amountSeven < tasks[i].length)
		 {
		 	level8 += tasks[i].finish_time - tasks[i].arrival_time;
		 	l8++;
		 }
	}
	if(l1 != 0) {
		printf("\nCPU Lengths (0 < X <= %d)\t: Avg Response Time (%d)", amountOne, level1/l1);
	}
	if(l2 != 0) {
		printf("\nCPU Lengths (%d < X <= %d)\t: Avg Response Time (%d)", amountOne, amountTwo, level2/l2);
	}
	if(l3 != 0) {
		printf("\nCPU Lengths (%d < X <= %d)\t: Avg Response Time (%d)", amountTwo, amountThree, level3/l3);
	}
	if(l4 != 0) {
		printf("\nCPU Lengths (%d < X <= %d)\t: Avg Response Time (%d)", amountThree, amountFour, level4/l4);
	}
	if(l5 != 0) {
		printf("\nCPU Lengths (%d < X <= %d)\t: Avg Response Time (%d)", amountFour, amountFive, level5/l5);
	}
	if(l6 != 0) {
		printf("\nCPU Lengths (%d < X <= %d)\t: Avg Response Time (%d)",amountFive, amountSix, level6/l6);
	}
	if(l7 != 0) {
		printf("\nCPU Lengths (%d < X <= %d)\t: Avg Response Time (%d)", amountSix, amountSeven, level7/l7);
	}
	if(l8 != 0) {
		printf("\nCPU Lengths (%d < X)\t\t: Avg Response Time (%d)", amountSeven, level8/l8);
	}
	printf("\n\n");
}

void run_simulation(int algorithm, int quantum)
{
    switch(algorithm) {
        case RR:
            round_robin(quantum);
            break;
        case PS:
            priority_scheduling();
            break;
        case MLFQ:
            mlfq_scheduling();
            break;
        case FCFS:
        default:
            first_come_first_serve();
            break;
    }
}


void compute_and_print_stats()
{
    int tasks_at_level[PRIORITY_LEVELS] = {0,};
    float response_at_level[PRIORITY_LEVELS] = {0.0, };
    int scheduling_events = 0;
    int i;

    for (i = 0; i < num_tasks; i++) {
        tasks_at_level[tasks[i].priority]++;
        response_at_level[tasks[i].priority] +=
            tasks[i].finish_time - (tasks[i].arrival_time * 1.0);
        scheduling_events += tasks[i].schedulings;

        printf("Task %3d: cpu time (%4.1f), response time (%4.1f), waiting (%4.1f), schedulings (%5d)\n",
            i, tasks[i].length,
            tasks[i].finish_time - tasks[i].arrival_time,
            tasks[i].finish_time - tasks[i].arrival_time - tasks[i].cpu_cycles,
            tasks[i].schedulings);

    }

    printf("\n");

    if (num_tasks > 0) {
        for (i = 0; i < PRIORITY_LEVELS; i++) {
            if (tasks_at_level[i] == 0) {
                response_at_level[i] = 0.0;
            } else {
                response_at_level[i] /= tasks_at_level[i];
            }
            printf("Priority level %d: average response time (%4.1f)\n",
                i, response_at_level[i]);
        }
    }
    printf ("Total number of scheduling events: %d\n", scheduling_events);
    print_graph();
}


int main(int argc, char *argv[])
{
    int i = 0;
    int algorithm = FCFS;
    quantum = 1;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0) {
            i++;
            quantum = atoi(argv[i]);
        } else if (strcmp(argv[i], "-a") == 0) {
            i++;
            if (strcmp(argv[i], "FCFS") == 0) {
                algorithm = FCFS;
            } else if (strcmp(argv[i], "RR") == 0) {
                algorithm = RR;
            } else if (strcmp(argv[i], "PS") == 0) {
                algorithm = PS;
            } else if (strcmp(argv[i], "MLFQ") == 0) {
                algorithm = MLFQ;
            }
        }
    }

    read_task_data();

    if (num_tasks == 0) {
        fprintf(stderr,"%s: no tasks for the simulation\n", argv[0]);
        exit(1);
    }

    init_simulation_data(algorithm);
    run_simulation(algorithm, quantum);
    compute_and_print_stats();

    exit(0);
}
