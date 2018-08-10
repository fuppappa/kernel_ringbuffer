
/*
* kernel_version
* this program is ringbuffer module
* part of the program used for research
* developer yfujieda
* target machine is hammerhead (Nexus5)
*/


#include <linux/module.h>	/* Needed for all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/slab.h>   /* Needed for kmalloc kfree */
#include <linux/string.h> /* Needed for strcat, memcpy */
#include <linux/types.h>  /* Needed for any type(size_t ...etc) */

/*
* if buffer is full return 1
* empty return 2
*/

#define Failure -1
#define full 1
#define emp 2
#define True 0

typedef struct ring_buf{
  char *buf;
  char *head;
  char *tail;
  int   size;
} ring_t;


int buf_init(ring_t *q , int b_size)
{
  q->size = b_size;
  q->buf= (char *)kmalloc(sizeof(char *) * b_size, GFP_KERNEL);
  q->head=q->buf;
  q->tail=q->buf;

  return 0;

}

int buf_state(ring_t *q)
{
  if(q->head == q->tail){
    return emp;
  }else if(q->head+1 == q->tail){
    return full;
  }
  return 0;
}


int buf_put(ring_t *q, void *in_data, int d_size)
{
  int i = 0;
  if(!q){
    return Failure;
  }

  if((int)((q->head - q->buf)+d_size) > q->size){
    for(i=0; i<d_size; i++){
      *(q->head) = *((char *)in_data+i);
      q->head = q->buf + (int)((q->head - q->buf)+sizeof(char)) % q->size;
    }
  }else{
    memcpy(q->head, in_data, d_size);
  }
  //printf("%dbyte\n", size);
  //*(q->head) = in_data;
  /*head refresh
  */
  q->head = q->buf + (int)((q->head - q->buf)+d_size) % q->size;

  return 0;
}


int buf_get(ring_t *q, char *out_data)
{
  if(!q){
    return Failure;
  }

  memcpy(out_data, q->tail, sizeof(*out_data));
  //  *out_data = *(q->tail);
  q->tail = q->buf + ((q->tail - q->buf) + sizeof(*out_data)) % q->size;

  return 0;
}
