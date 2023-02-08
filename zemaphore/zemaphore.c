#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
    s->value = value;
    pthread_mutex_init(&s->mtx, NULL);
    pthread_cond_init(&s->cnd, NULL);
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&s->mtx);
    while(s->value <= 0){
        pthread_cond_wait(&s->cnd,&s->mtx);
    };
    s->value --;
    pthread_mutex_unlock(&s->mtx);

}

void zem_up(zem_t *s) {
    pthread_mutex_lock(&s->mtx);
    s->value++;
    pthread_cond_signal(&s->cnd);
    pthread_mutex_unlock(&s->mtx);

}
