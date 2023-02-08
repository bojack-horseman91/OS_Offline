#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>
// Constants for the maximum number of seats and customers
#define MAX_SEAT 5
#define MAX_CUSTOMER 10

// Global variables to keep track of the number of served and waiting customers
int number_of_served = 0;
int number_of_waiting_customer = 0;

// Mutex and condition variables for synchronizing the threads
pthread_mutex_t shop_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_cut = PTHREAD_COND_INITIALIZER;
pthread_cond_t customer_present = PTHREAD_COND_INITIALIZER;
pthread_cond_t hair_cut_done = PTHREAD_COND_INITIALIZER;
pthread_cond_t customer_leaves = PTHREAD_COND_INITIALIZER;
// Function that implements the behavior of the barber thread
void *barbarHaircut(void *arg) {
    printf("Barber is starting work\n");

    while (1) {
        // Lock the shop and wait for a customer to arrive
        pthread_mutex_lock(&shop_lock);


        while(number_of_waiting_customer==0){
            printf("Barber is waiting for a customer and has served %d\n",number_of_served);
            pthread_cond_wait(&customer_present, &shop_lock);
        }
        pthread_cond_signal(&can_cut);
        number_of_waiting_customer--;
        pthread_cond_wait(&customer_leaves,&shop_lock);
        printf("barber has served %d \n",number_of_served);
        if(number_of_served==MAX_CUSTOMER){
            printf("Barber is done for the day ....BYEEEEEE and remeber sakib is awesome!\n");
            pthread_mutex_unlock(&shop_lock);
            break;
        }
        pthread_mutex_unlock(&shop_lock);
    }
}

// Function that implements the behavior of a customer thread
void *customer_get_haircut(void *arg) {
    int customer_id = *(int *)arg;
    printf("Customer %d has entered the shop\n", customer_id);

    // Lock the shop and update the number of served customers
    pthread_mutex_lock(&shop_lock);
    

    // Check if there are any available seats
    if (number_of_waiting_customer == MAX_SEAT) {
        printf("Customer %d is leaving the shop\n", customer_id);
        number_of_served++;
        pthread_mutex_unlock(&shop_lock);
        return NULL;
    }

    // Update the number of waiting customers and signal the barber
    number_of_waiting_customer++;
    pthread_cond_signal(&customer_present);
    printf("Customer %d is waiting for a haircut\n", customer_id);

    // Wait for the barber to signal that it's the customer's turn
    pthread_cond_wait(&can_cut, &shop_lock);
    printf("Customer %d is getting a haircut\n", customer_id);
    sleep(1);
    printf("customer %d gets a haircut\n",customer_id);
    number_of_served++;
    pthread_cond_signal(&customer_leaves);   
    pthread_mutex_unlock(&shop_lock);
    free(arg);
}

int main(int argc, char const *argv[])
{

    // MAX_CUSTOMER=atoi(argv[1]);
    // MAX_SEAT=atoi(argv[2]);
    number_of_waiting_customer=0;
    number_of_served=0;
    printf("number_of customer %d and seat %d",MAX_CUSTOMER,MAX_SEAT);

    pthread_t barbarThread;
    pthread_t customer_thread[MAX_CUSTOMER];
    
    if(pthread_create(&barbarThread,NULL,&barbarHaircut,NULL)!=0){
        perror("ERROR WHILE CREATING THREAD\n");
    }
    else{
        printf("success\n");
    }
    for (int i = 0; i < MAX_CUSTOMER; i++)
    {
        int *a=(int *)malloc(sizeof(int));
        *a=i;
        if(pthread_create(&customer_thread[i],NULL,&customer_get_haircut,(void*)a)!=0){
            perror("ERROR WHILE CREATING THREAD\n");
        }
        else{
            printf("success2\n");
        }
        sleep(rand()%10*0.005+.01);

    }

    for (int i = 0; i < MAX_CUSTOMER; i++)
    {
        
        pthread_join(customer_thread[i],NULL);
        
    }
    pthread_join(barbarThread,NULL);
    return 0;
}

