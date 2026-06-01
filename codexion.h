#ifndef CODEXION_H
# define CODEXION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_waiter
{
    int coder_id;
    long priority;
} t_waiter;

typedef struct s_pqueue
{
    t_waiter *heap;
    int size;
    int capacity;
} t_pqueue;

typedef struct s_data{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
    long start_time;
    int is_finish;
    pthread_mutex_t log_mutex;
    pthread_mutex_t stop_mutex;
    struct s_dongle *dongles; 
    struct s_coder *coders;
} t_data;

typedef struct s_dongle
{
    int id;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    long last_released;
    t_pqueue queue;
} t_dongle;

typedef struct s_coder
{
    pthread_t thread;
    int id;
    int compiles_done;
    long last_compile;
    struct s_dongle *left_dongle;
    struct s_dongle *right_dongle;
    struct s_data *data;
} t_coder;


/* utils */
long    get_time_in_ms(void);

/* pqueue */
int     pqueue_init(t_pqueue *pq, int capacity);
void    pqueue_push(t_pqueue *pq, int coder_id, long priority);
int     pqueue_peek_id(t_pqueue *pq);
void    pqueue_free(t_pqueue *pq);

/* init */
int     init_data(t_data *data, char **argv);
int     init_dongles(t_data *data);
int     init_coders(t_data *data);
void    cleanup(t_data *data);

#endif