# 1.2 Queue

## Description:
Queue is the datastructe with obvious uses: putting objects in the queue and taking them off again, with access to the inserted first items. Also called FIFO for First In First Out storage.

Applications include task that have to be processed cyclically, also central structure in breadth-first search, in tree data structures.

The difference from stack is that changes occur at both ends: at one end, there are inserts; at the other deletes.

## Operations
   | Time Complexity | Return Type    | Operation name         | Arguments 		       	|
   |:----------------|:---------------|:-----------------------|:-------------------------------|
   | O(1)	     | queue_t *      | queue\_constructor     | (size\_t entry\_size)		|
   | O(n)  	     | void           | queue\_destructor      | (queue\_t *queue)		|
   | O(1)	     | void	      | queue\_enqueue         | (queue\_t *queue, void *entry) |
   | O(1)            | void * 	      | queue\_dequeue         | (queue\_t *queue)		|
   | O(1)	     | unsinged char  | queue\_is\_empty       | (queue\_t *queue)		|

## Implementations
[List Based Queue](./list)