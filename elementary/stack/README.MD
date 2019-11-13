# 1.1 Stack

## Description:
Stack is the simplest datastructe with obvious uses: putting objects on the stack and taking them off again, with access only to the top item. Also called LIFO for Last In First Out storage.

Applications include evaluation of arithmetic expressions with parens and operators priorities, search with backtracking, central structure in deapth-first search in tree data structures.

## Operations
   | Time Complexity 	   | Return Type      | Operation name         | Arguments 		       	|
   |:----------------------|:-----------------|:-----------------------|:-------------------------------|
   | O(1)	     	   | stack_t *        | stack\_constructor     | (size\_t entry\_size)		|
   | fake: O(1), real: O(n)| void             | stack\_destructor      | (stack\_t *stack)		|
   | O(1)	     	   | void	      | stack\_push            | (stack\_t *stack, void *entry) |
   | O(1)                  | void * 	      | stack\_pop             | (stack\_t *stack)		|
   | O(1)	           | unsinged char    | stack\_is\_empty       | (stack\_t *stack)		|

## Implementations
1. [Array Based Stack](./array)
2. [List](./list)
3. TODO: [Block List Based Stack](./block_list)