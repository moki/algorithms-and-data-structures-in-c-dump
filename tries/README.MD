# 8 Tries

## Description:
In real applications, text processing is more important than the processing of
numbers, and text fragments have a length. So we need different structures
for strings than for numeric keys.

The basic tool for string data structures, similar in role to the balanced bi-
nary search tree, is called “trie,” which is said to derive from “retrieval.”

Trie is a kind of search tree—an ordered tree data structure used to store a dynamic set or associative array, where the keys are usually strings.
In a trie nodes are not binary,they contain potentially one outgoing edge for each possible character, so the degree is at most the alphabet size |A|.
All the descendants of a node have a common prefix of the string associated with that node, and the root is associated with the empty string.
Keys tend to be associated with leaves, though some inner nodes may correspond to keys of interest. Hence, keys are not necessarily associated with every node.
The node corresponding to the prefix σ1 contains for each character a ∈ A a pointer to the node corresponding to the prefix *σ1***a** if such a node exists, that is if there is a string *σ1* **a** *σ2* in the set.

## Implementations
1. [Array Trie](./array/README.MD)
2. [List Trie](./list/README.MD)
2. [Alphabet Reduction](./alphabet_reduction/README.MD)