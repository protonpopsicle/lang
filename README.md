lang
====

This is a language implementation for a toy C-like language. The compiler is heavily based on examples from the [Dragon Book](http://dragonbook.stanford.edu/). Lang programs run in a simple virtual stack machine. For a formal definition of the language, see comments in [Parser.cpp](src/compiler/Parser.cpp)

example.src contains a sample program

```bash
$ make
$ ./bin/compile example.src
$ ./bin/run example.lang
```
