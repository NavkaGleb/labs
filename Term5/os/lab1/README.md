# Operating Systems Lab1

Variant 22 (using C++)

```text
Use C++, processes (fork) and unnamed pipes. Implement the whole 
system in one program. Use polling.
```

### Class UnnamedPipe

Basic abstraction to syscall `pipe`

### Class NonblockingReadPoll

Basic abstraction to syscall `poll` with `POLLIN` mode and `0 timeout`

### Class Keyboard

Allows switch between blocking and non-blocking input

### Class Manager

Implements whole program logic