#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include<semaphore.h>

using namespace std;

struct read_write_lock
{
    int reader_count;
    int writer_count;
    sem_t reader_semaphore;
    sem_t writer_semaphore;
    pthread_mutex_t rw_lock;
    pthread_cond_t can_write;
    pthread_cond_t can_read;

    int waiting_writer;
    int waiting_reader;
  

};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);
