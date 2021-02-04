# Perfect Hashing

For hashing used Universal Hashing: h(x) = ((a * x + b) % p) % n
* a, b - random params
* p - big prime humber
* n - number of elements

For hash table used Perfect Hashing:
* Ο(1) memory accesses in the worst case
* The set of keys is static - does not change after save to table
* First hashing level: n keys are hashed in m cells using the universal hash function
* Second hashing level: each cell has its own secondary hash table 
  with its universal hash a function selected to avoid collisions; 
  cell size - the square number of keys hashed in cell
* The expected total memory for such a structure is O(n)