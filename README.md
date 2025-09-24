# Custom Clang Tidy plugin

## Viewing Clang AST

To view the AST for the constructors of the class `ExampleBad` run

`clang-check --ast-dump --ast-dump-filter="ExampleBad::ExampleBad" example/example.cpp`


## Compiling

`cmake -B build -G Ninja; cmake --build build`

## Linking

To link our shared library with clang-tidy we use the `--load` flag.

For example:

`clang-tidy -checks='*' --list-checks --load=./build/lib/libMoveConstructorNoexcept.dylib | grep -i asdf`

## Random

```
clang-tidy -checks='-*,*asdf' --list-checks --load=./build/lib/libMoveConstructorNoexcept.dylib ./example/example.cpp
```

To run
```
clang-tidy -checks='-*,*asdf' --load=./build/lib/libMoveConstructorNoexcept.dylib ./example/example.cpp
```


