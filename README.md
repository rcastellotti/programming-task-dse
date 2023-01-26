# programming-task-dse

This repository contains a programming task the [Distributed Systems & Operating Systems](https://dse.in.tum.de) gave me to test my skills before discussing a guided research project
## Instructions

-  build with `make`
- run `./server <size>`
- run `./client`

Implement two programs in C/C++/Rust:

## Server program:
 - Initialize a hash table of given size (command line)
 - Support insertion of items in the hash table
 - Hash table collisions are resolved by maintaining a linked list for each bucket/entry in the hash table
 - Supports concurrent operations (multithreading) to perform (insert, read, delete operations on the hash table)
 - Use readers-writer lock to ensure safety of concurrent operations, try to optimize the granularity   
 - Communicates with the client program using shared memory buffer (POSIX shm)

## Client program:
 - Enqueue requests/operations (insert, read a bucket, delete) to the server (that will operate on the the hash table) via shared memory buffer (POSIX shm)

Please only use standard libraries. For those not specified, implement as appropriate. Please send me the code (or link to a repository) with a Makefile to compile and run it. There is no specific deadline, though if you decide not to solve the task, just let me know. Also, as I told you in the slack, solving the task does not confirm a topic. We will discuss and make a final decision depending on a situation at that time after you solve the task.

## Notes

+ I decided to use "dead" as a null value for the hashmap
+ Collisions are handled using a linked list, if the key is already present we update the value
+ I used [rxi/log.c](https://github.com/rxi.log.c) to have a prettier logging, I don't think this is a violation of "only use standard libraries"
+ I am acquiring locks only on buckets in order to not block the entire hashmap
+ I asked chatGPT to generate a simple hashing function, it's far from being a good hash function, but this is intended, as we need to test for collisions
### Test the hashmap implementation

```c
#include "log.h"
#include "map.h"

int main()
{
    hashmap *m = hashmap_create(10);
    hashmap_put(m, "vt", "8");
    hashmap_put(m, "distsys", "9");
    hashmap_put(m, "acn", "10");
    hashmap_put(m, "aca", "5");
    hashmap_put(m, "netsec", "10");
    hashmap_put(m, "idp", "85");
    log_debug("got acn: %s", hashmap_get(m, "acn"));
    hashmap_print(m);
    hashmap_remove(m, "acn");
    hashmap_print(m);
}
```
