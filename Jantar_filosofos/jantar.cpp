#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <random>
#include <ctime>
#include <semaphore.h>

using namespace std;

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int phil[N];
int times = 200;

unsigned seed = time(0);

class monitor {
    int state[N];

    sem_t mutex;
    sem_t s[N];

public:
    monitor() {
        sem_init(&mutex, 0, 1);
        for (int j = 0; j < N; j++) {
            sem_init(&s[j], 0, 0);
            state[j] = THINKING;
        }
    }

    ~monitor() {
        sem_destroy(&mutex);
        for (int j = 0; j < N; j++) {
            sem_destroy(&s[j]);
        }
    }

    void take_fork(int phnum) {
        sem_wait(&mutex);

        state[phnum] = HUNGRY;
        cout << "Filosofo " << phnum+1 << " esta com fome" << endl;

        srand(seed);
        sleep((rand()%100/50.0));

        test(phnum);

        sem_post(&mutex);

        sem_wait(&s[phnum]);

        state[phnum] = EATING;
        cout << "Filosofo " << phnum+1 << " pegou os garfos e esta comendo" << endl;
    }

    void put_fork(int phnum) {
        sem_wait(&mutex);

        state[phnum] = THINKING;
        cout << "Filosofo " << phnum+1 << " soltou os garfos e esta pensando" << endl;

        test(RIGHT);
        test(LEFT);

        sem_post(&mutex);
    }

    void test(int phnum) {
        if(state[phnum] == HUNGRY and state[(phnum + 1) % 5] != EATING and state[(phnum + 4) % 5] != EATING) {
            state[phnum] = EATING;
            sem_post(&s[phnum]);
        }
    }
};

monitor phil_object;

void* philosopher(void* arg) {
    int c = 0;

    while(c < times) {
        int j = *(int*)arg;
        sleep(3.5);
        phil_object.take_fork(j);
        sleep(3.5);
        phil_object.put_fork(j);
        c++;
    }
}

int main() {
    pthread_t thread_id[N];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int j=0; j < N; j++) {
        phil[j] = j;
    }

    for (int j=0; j < N; j++) {
        pthread_create(&thread_id[j], &attr, philosopher, &phil[j]);
        cout << "Filosofo " << j+1 << " esta pensando..." << endl;
    }

    for (int j=0; j < N; j++) {
        pthread_join(thread_id[j], NULL);
    }

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    return 0;
}