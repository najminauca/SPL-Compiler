/*
 * identifier.c -- symbol management
 */

#include <string.h>
#include <stdbool.h>
#include <util/memory.h>
#include <util/string_ops.h>
#include "identifier.h"

static int hashSize = 0;
static Identifier **buckets;
static int numEntries;

static unsigned stamp = 314159265;

static unsigned hash(char *s) {
    unsigned h, g;

    h = 0;
    while (*s != '\0') {
        h = (h << 4) + *s++;
        g = h & 0xF0000000;
        if (g != 0) {
            h ^= g >> 24;
            h ^= g;
        }
    }
    return h;
}

static bool isPrime(int i) {
    int t;

    if (i < 2) {
        return false;
    }
    if (i == 2) {
        return true;
    }
    if (i % 2 == 0) {
        return false;
    }
    t = 3;
    while (t * t <= i) {
        if (i % t == 0) {
            return false;
        }
        t += 2;
    }
    return true;
}

static void initTable(void) {
    int i;

    hashSize = INITIAL_HASH_SIZE;
    while (!isPrime(hashSize)) {
        hashSize++;
    }
    buckets = (Identifier **) allocate(hashSize * sizeof(Identifier *));
    for (i = 0; i < hashSize; i++) {
        buckets[i] = NULL;
    }
    numEntries = 0;
}

static void growTable(void) {
    int newHashSize;
    Identifier **newBuckets;
    int i, n;
    Identifier *p, *q;

    /* compute new hash size */
    newHashSize = 2 * hashSize + 1;
    while (!isPrime(newHashSize)) {
        newHashSize += 2;
    }
    /* init new hash table */
    newBuckets = (Identifier **) allocate(newHashSize * sizeof(Identifier *));
    for (i = 0; i < newHashSize; i++) {
        newBuckets[i] = NULL;
    }
    /* rehash old entries */
    for (i = 0; i < hashSize; i++) {
        p = buckets[i];
        while (p != NULL) {
            q = p;
            p = p->next;
            n = q->hashValue % newHashSize;
            q->next = newBuckets[n];
            newBuckets[n] = q;
        }
    }
    /* swap tables */
    release(buckets);
    buckets = newBuckets;
    hashSize = newHashSize;
}

Identifier *newIdentifier(char *string) {
    unsigned hashValue;
    int n;
    Identifier *p;

    /* initialize hash table if necessary */
    if (hashSize == 0) {
        initTable();
    }
    /* grow hash table if necessary */
    if (numEntries == hashSize) {
        growTable();
    }
    /* compute hash value and bucket number */
    hashValue = hash(string);
    n = hashValue % hashSize;
    /* search in bucket list */
    p = buckets[n];
    while (p != NULL) {
        if (p->hashValue == hashValue) {
            if (strcmp(p->string, string) == 0) {
                /* found: return symbol */
                return p;
            }
        }
        p = p->next;
    }
    /* not found: add new symbol to bucket list */
    p = new(Identifier);
    p->string = copy(string);
    p->stamp = stamp;
    stamp += 0x9E3779B9;  /* Fibonacci hashing, see Knuth Vol. 3 */
    p->hashValue = hashValue;
    p->next = buckets[n];
    buckets[n] = p;
    numEntries++;
    return p;
}
