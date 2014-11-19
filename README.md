lang
====

This is an implementation for Lang, a toy C-like programming language. The compiler is heavily based on examples from the [Dragon Book](http://dragonbook.stanford.edu/). Lang programs run in a simple virtual stack machine. For a formal definition of the language, see comments in [Parser.cpp](src/compiler/Parser.cpp).

example.src contains a sample program

```bash
$ make
$ ./bin/compiler example.src
$ ./bin/run example.lang
```
