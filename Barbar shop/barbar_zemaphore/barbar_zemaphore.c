#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "zemaphore.h"

#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 10

zem_t* barber;
zem_t* customer;
zem_t* mutex;
int waiting = 0;

void *barberThread(void *arg) {
  int BARBAR_PROCESSING_TIME = *(int*)arg;
  printf("BARBAR PROCESSING TIME: %d\n", BARBAR_PROCESSING_TIME); 
  while (1) {
    zem_down(customer);
    zem_down(mutex);
    waiting--;
    printf("Waiting = %d\n", waiting);
    zem_up(barber);
    zem_up(mutex);
    printf("Barber is cutting hair.\n");
    sleep(BARBAR_PROCESSING_TIME);
  }
  return NULL;
}

void *customerThread(void *arg) {
  int customer_id = *(int*)arg;
  zem_down(mutex);
  if (waiting < NUM_CHAIRS) {
    waiting++;
    zem_up(customer);
    zem_up(mutex);
    zem_down(barber);
    printf("Customer %d is getting a haircut.\n", customer_id);
  } else {
    zem_up(mutex);
    printf("Customer %d leaves the barber shop.\n", customer_id);
  }
  return NULL; 
}

int main(void) {
  barber = (zem_t*) malloc(sizeof(zem_t));
  customer = (zem_t*) malloc(sizeof(zem_t));
  mutex = (zem_t*) malloc(sizeof(zem_t));

  pthread_t barber_thread;
  pthread_t customer_thread[NUM_CUSTOMERS];
  zem_init(barber, 1);
  zem_init(customer, 1);
  zem_init(mutex, 1);
  int *BARBAR_PROCESSING_TIME = (int*)malloc(sizeof(int));
  *BARBAR_PROCESSING_TIME = 2;
  pthread_create(&barber_thread, NULL, barberThread, BARBAR_PROCESSING_TIME);
  for (int i = 0; i < NUM_CUSTOMERS; i++) {
    int *customer_id = (int*)malloc(sizeof(int));
    *customer_id = i;
    pthread_create(&customer_thread[i], NULL, customerThread, customer_id);
    int sleeptime = rand()%1 + 0;
    sleep(sleeptime);
  }

  
  sleep(45);
  printf("A customer came after a log time.\n");
  pthread_t late_customer_thread; 
  int *customer_id = (int*)malloc(sizeof(int));
  *customer_id = NUM_CUSTOMERS;
  pthread_create(&late_customer_thread, NULL, customerThread, customer_id);

  pthread_join(barber_thread, NULL);
  for (int i = 0; i < NUM_CUSTOMERS; i++) {
    pthread_join(customer_thread[i], NULL);
  }
  pthread_join(late_customer_thread, NULL);

  free(barber);
  free(customer);
  free(mutex);
  return 0;
}
