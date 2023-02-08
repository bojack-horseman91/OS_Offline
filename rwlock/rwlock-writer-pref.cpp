#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  pthread_mutex_init(&rw->rw_lock,NULL);
  // sem_init(&rw->reader_semaphore,0,1);
  // sem_init(&rw->writer_semaphore,0,1);
  rw->can_read=PTHREAD_COND_INITIALIZER;
  rw->can_write=PTHREAD_COND_INITIALIZER;
  pthread_mutex_lock(&rw->rw_lock);
    rw->reader_count=0;
    rw->writer_count=0;
    rw->waiting_writer=0;
  pthread_mutex_unlock(&rw->rw_lock);
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->rw_lock);
  // printf("reader waiting\n");
    while(rw->writer_count==1||rw->waiting_writer>0)
      pthread_cond_wait(&rw->can_read,&rw->rw_lock);
    rw->reader_count++;
  pthread_mutex_unlock(&rw->rw_lock);
  
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->rw_lock);
    rw->reader_count--;
    if(rw->reader_count==0)
      pthread_cond_signal(&rw->can_write);
    if(rw->writer_count==1||rw->waiting_writer>0)
      pthread_cond_broadcast(&rw->can_read);
  pthread_mutex_unlock(&rw->rw_lock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->rw_lock);
    rw->waiting_writer++; 
  // printf("writer waiting\n");
  while (rw->writer_count==1||rw->reader_count>0)
  { 

    pthread_cond_wait(&rw->can_write,&rw->rw_lock);
  }
    rw->writer_count=1;
    rw->waiting_writer--;
  pthread_mutex_unlock(&rw->rw_lock);

}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  
  pthread_mutex_lock(&rw->rw_lock);
    rw->writer_count--;
    if(rw->waiting_writer>0)
      pthread_cond_signal(&rw->can_write);
    else
      pthread_cond_broadcast(&rw->can_read);
  pthread_mutex_unlock(&rw->rw_lock);
  
  
}











// //VERSION 2 WITH SEMAPHORES
// #include "rwlock.h"

// void InitalizeReadWriteLock(struct read_write_lock * rw)
// {
//   //	Write the code for initializing your read-write lock.
//   pthread_mutex_init(&rw->rw_lock,NULL);
//   sem_init(&rw->reader_semaphore,0,1);
//   sem_init(&rw->writer_semaphore,0,1);
//   // rw->can_read=PTHREAD_COND_INITIALIZER;
//   // rw->can_write=PTHREAD_COND_INITIALIZER;
//   pthread_mutex_lock(&rw->rw_lock);
//     rw->reader_count=0;
//     rw->writer_count=0;
//     rw->waiting_writer=0;
//   pthread_mutex_unlock(&rw->rw_lock);
// }

// void ReaderLock(struct read_write_lock * rw)
// {
//   //	Write the code for aquiring read-write lock by the reader.
//   sem_wait(&rw->reader_semaphore);
//   pthread_mutex_lock(&rw->rw_lock);
//   // printf("reader waiting\n");
//   //   while(rw->writer_count==1||rw->waiting_writer>0)
//   //     pthread_cond_wait(&rw->can_read,&rw->rw_lock);
//     if(rw->reader_count==1)
//       sem_wait(&rw->writer_semaphore);
//     rw->reader_count++;
//   pthread_mutex_unlock(&rw->rw_lock);
//   sem_post(&rw->reader_semaphore);
  
// }

// void ReaderUnlock(struct read_write_lock * rw)
// {
//   //	Write the code for releasing read-write lock by the reader.
//   pthread_mutex_lock(&rw->rw_lock);
//     rw->reader_count--;
//     if(rw->reader_count==0)
//       sem_post(&rw->writer_semaphore);
//   pthread_mutex_unlock(&rw->rw_lock);
// }

// void WriterLock(struct read_write_lock * rw)
// {
//   //	Write the code for aquiring read-write lock by the writer.
//   pthread_mutex_lock(&rw->rw_lock);
//     rw->waiting_writer++; 
//     if(rw->waiting_writer==1){
//       sem_wait(&rw->reader_semaphore);
//     }
//     pthread_mutex_unlock(&rw->rw_lock);
//   sem_wait(&rw->writer_semaphore);
//   pthread_mutex_lock(&rw->rw_lock);
//     rw->writer_count=1;
//     rw->waiting_writer--;
//   pthread_mutex_unlock(&rw->rw_lock);

// }

// void WriterUnlock(struct read_write_lock * rw)
// {
//   //	Write the code for releasing read-write lock by the writer.
  
//   pthread_mutex_lock(&rw->rw_lock);
//     rw->writer_count--;
//     if(rw->waiting_writer==0)
//       sem_post(&rw->reader_semaphore);
//     sem_post(&rw->writer_semaphore);
//   pthread_mutex_unlock(&rw->rw_lock);
  
  
  
// }

