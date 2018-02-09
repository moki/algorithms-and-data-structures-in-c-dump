#include <assert.h>
#include "queue.h"

int main(int argc, char **argv) {
  Queue q = QueueConstructor();
  assert(q);

  QueueEnqueue(q, 1);
  QueueEnqueue(q, 2);
  QueueEnqueue(q, 3);
  QueueEnqueue(q, 4);

  assert(QueueDequeue(q)->data == 1);
  assert(QueueDequeue(q)->data == 2);
  assert(QueueDequeue(q)->data == 3);
  assert(QueueDequeue(q)->data == 4);

  QueueDestructor(q);
}