#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 1000000000

#define size 5

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;

int buffer[size];
int count = 0, in = 0, out = 0;

const clock_t MAXDELAY = 2000000;

void delay(clock_t ticks) { /* a "busy" delay */
  clock_t start = clock();
  do
    ; while (clock() < start + ticks);
}

void *producer(void *ptr) {
    int i, item;

    /* usado para sinalizacao */
    /* buffer usado entre produtor e consumidor */
    /* dados do produtor */
    for (i = 1; i <= MAX; i++) {
        item = i;

        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (count == size)
            pthread_cond_wait(&condp, &the_mutex);

        buffer[in] = item; /* coloca item no buffer */
        printf("Colocando item %d no buffer[%d]\n", item, in);
        in = (in + 1) % size;
        count++;

        delay(MAXDELAY);

        pthread_cond_signal(&condc); /* acorda consumidor */
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
    }

    pthread_exit(0);
}

void *consumer(void *ptr) {
    int i, item;

    /* quantos numeros produzir */
    /* dados do consumidor */
    for (i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (count == 0)
            pthread_cond_wait(&condc, &the_mutex);

        item = buffer[out]; /* retira o item do buffer */
        printf("Retirando item %d do buffer[%d]\n", item, out);
        out = (out + 1) % size;
        count--;

        delay(MAXDELAY);

        /* acorda o produtor */
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
    }
    pthread_exit(0);
}

int main(int argc, char **argv) {

    pthread_t pro, con;
    pthread_mutex_init(&the_mutex, 0);
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);
    pthread_join(pro, 0);
    pthread_join(con, 0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}