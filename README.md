# EK compiler

# Author: Jiawei Jiang && Jingchun Wang

# Dependencies
1. Bison(>=3.0)     
2. Flex     
3. Yaml-cpp     

# Compilation
Can automatically compiled by `cmake`       
```
    mkdir build && cd build
    cmake ../src
    make
    (optional:) make install
```



## Assignment 1 Parsing and building AST
Implemented "-emit-ast" option.

## Exercise 1 Fuzzing Test
1. Implemented using `libFuzzer`.       
2. Built corpus as slides indicated     
```
    $ cat test1.ek | sed 's/[ \n]\+/\n/g' | grep -v '^$' | sed 's/^/"/' |
    sed 's/$/"/' > dict2
```
3. Run: `./fuzzer -dict=dict2 -detect_leaks=0`
4. Crash found! Run `./fuzzer crash-da39a3ee5e6b4b0d3255bfef95601890afd80709` to see the outputs:       
5. It seems like memory leak happens in 
```
    #1 0x58e176 in __gnu_cxx::new_allocator<bison::Parser::stack_symbol_type>::allocate(unsigned long, void const*)
```

    It is really fair, because I do not set destructors for my AST tree node which will cause memory leak for pointer members.

## Assignment 2 Add Constraints and Type Inference
Refined "-emit-ast" option.
