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
    rw->waiting_reader=0
  pthread_mutex_unlock(&rw->rw_lock);

}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  // sem_wait(&rw->reader_semaphore);

  pthread_mutex_lock(&rw->rw_lock);
    rw->waiting_reader++;
    
    // if(rw->reader_count==1)
    //   sem_wait(&rw->writer_semaphore);
    while(rw->writer_count>0)
        pthread_cond_wait(&rw->can_read,&rw->rw_lock);
    rw->reader_count++;
    rw->waiting_reader--;  
  pthread_mutex_unlock(&rw->rw_lock);
  // sem_post(&rw->reader_semaphore);
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->rw_lock);
    rw->reader_count--;
    if(rw->reader_count==0&&rw->waiting_reader==0)
      {
        // sem_post(&rw->writer_semaphore);
        pthread_cond_signal(&rw->can_write);
      }
    pthread_cond_broadcast(&rw->can_read);
  pthread_mutex_unlock(&rw->rw_lock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  // sem_wait(&rw->writer_semaphore);
  pthread_mutex_lock(&rw->rw_lock);
  while(rw->reader_count>0||rw->writer_count>0||rw->waiting_reader>0){
    pthread_cond_wait(&rw->can_write,&rw->rw_lock);
  }
  rw->writer_count++;
  pthread_mutex_unlock(&rw->rw_lock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.

  // sem_post(&rw->writer_semaphore);
  pthread_mutex_lock(&rw->rw_lock);
  rw->writer_count--;
  pthread_cond_broadcast(&rw->can_read);
  pthread_cond_signal(&rw->can_write);
  pthread_mutex_unlock(&rw->rw_lock);

}




// //VERSION 2 SEMAPHORE:
// #include "rwlock.h"


// void InitalizeReadWriteLock(struct read_write_lock * rw)
// {
//   //	Write the code for initializing your read-write lock.
//   pthread_mutex_init(&rw->rw_lock,NULL);
//   sem_init(&rw->reader_semaphore,0,1);
//   sem_init(&rw->writer_semaphore,0,1);
//   rw->can_read=PTHREAD_COND_INITIALIZER;
//   rw->can_write=PTHREAD_COND_INITIALIZER;
//   pthread_mutex_lock(&rw->rw_lock);
//     rw->reader_count=0;
//     rw->writer_count=0;
//     rw->waiting_reader=0
//   pthread_mutex_unlock(&rw->rw_lock);

// }

// void ReaderLock(struct read_write_lock * rw)
// {
//   //	Write the code for aquiring read-write lock by the reader.
//   sem_wait(&rw->reader_semaphore);
//   pthread_mutex_lock(&rw->rw_lock);
//     rw->reader_count++;
//     if(rw->reader_count==1)
//       sem_wait(&rw->writer_semaphore);
    
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
//   sem_wait(&rw->writer_semaphore);
// }

// void WriterUnlock(struct read_write_lock * rw)
// {
//   //	Write the code for releasing read-write lock by the writer.

//   sem_post(&rw->writer_semaphore);

// }
