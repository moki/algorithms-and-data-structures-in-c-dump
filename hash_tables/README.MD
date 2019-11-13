# 8 Hash Tables

## Description:
Hash tables are a dictionary of greap practical importance and can be very efficient.
Idea is that we have universe U and want to store a set of objects with keys from U.
We also have s buckets and a function h from U to S = {0, ..., s - 1}. Then we store the object with key u in the h(u)th bucket. If several objects map to the same bucket we have collision, then we can realize bucket as an array, each array space having space for one object. The theory of the hash tables mainly deals with the collision and choosing the function h(to achieve smaller collision rate).

Mapping big universe U to a small set S = {0, ..., s - 1} leads to many universe elements mapping to the same element of S. In dic. we dont have to store whole universe U but only some set X ⊂ U of n keys. Set is unavoidably dynamic, unknown to us when we choose hash function h: U->S, known that we can chooose a set X whose elements are mapped to the same s ∈ S, so the collision needs to be dealt with.

Solutions:

1. For each s ∈ S, a secondary structure that stores all elements x ∈ X, with h(x) = s.

	So Each of the s buckets contains another dic, since buckets contain only few elements it might be very simple i.e linked list.

	This approach called **Chaining**

2. Having for each u ∈ U a sequence of alternative addresses in S.

	if h(u) = h_1(u) is already used by a colliding element, we try h_2(u), h_3(u), ... until we find and empty bucket.

	This approach called **Open addressing**


## Implementations
1. [Chaining Hash Table](./chaining/README.MD)