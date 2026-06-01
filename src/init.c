#include "../codexion.h"

int     init_data(t_data *data, char **argv)
{
    data->number_of_coders = atoi(argv[1]);
    data->time_to_burnout = atoi(argv[2]);
    data->time_to_compile = atoi(argv[3]);
    data->time_to_debug = atoi(argv[4]);
    data->time_to_refactor = atoi(argv[5]);
    data->number_of_compiles_required = atoi(argv[6]);
    data->dongle_cooldown = atoi(argv[7]);
    data->scheduler = argv[8];
    data->is_finish = 0;
    data->start_time = 0;
    data->dongles = NULL;
    data->coders = NULL;

    //tenta inicializar o log_mutex
    if (pthread_mutex_init(&data->log_mutex, NULL) !=  0)
        return (1);
    //tenta inicializar o stop_mutex
    if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&data->log_mutex);
        return (1);
    }
    return (0);
}

static void    destroy_dongles(t_data *data, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&data->dongles[i].mutex);
        pthread_cond_destroy(&data->dongles[i].cond);
        i++;
    }
    free(data->dongles);
    data->dongles = NULL;
}

int     init_dongles(t_data *data)
{
    int i;
    int n;

    n = data->number_of_coders;
    data->dongles = malloc(sizeof(t_dongle) * n);
    if (!data->dongles)
        return (1);
    i = 0;
    while (i < n)
    {
        data->dongles[i].id = i;
        data->dongles[i].last_released = 0;
        if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
        {
            destroy_dongles(data, i);
            return (1);
        }
        if (pthread_cond_init(&data->dongles[i].cond, NULL) != 0)
        {
            pthread_mutex_destroy(&data->dongles[i].mutex);
            destroy_dongles(data, i);
            return (1);
        }
        i++;
    }
    return (0);
}

int     init_coders(t_data *data)
{
    int i;
    int n;

    n = data->number_of_coders;
    data->coders = malloc(sizeof(t_coder) * n);
    if (!data->coders)
        return (1);
    i = 0;
    while (i < n)
    {
        data->coders[i].id = i + 1;
        data->coders[i].compiles_done = 0;
        data->coders[i].last_compile = 0;
        data->coders[i].data = data;
        data->coders[i].left_dongle = &data->dongles[i];
        data->coders[i].right_dongle = &data->dongles[(i + 1) % n];
        i++;
    }
    return (0);
}

void    cleanup(t_data *data)
{
    if (data->dongles)
        destroy_dongles(data, data->number_of_coders);
    if (data->coders)
        free(data->coders);
    pthread_mutex_destroy(&data->log_mutex);
    pthread_mutex_destroy(&data->stop_mutex);
}
