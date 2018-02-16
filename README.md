# The programming language - ckx

[![CircleCI](https://circleci.com/gh/ILoveChenKX/compilers-ckx.svg?style=svg)](https://circleci.com/gh/ILoveChenKX/compilers-ckx)

A general purposed programming language -- for my honey -- chenkx

<br/>
<hr/>

## Original idea

Originally I decided to write a C compiler as a practice in my university. And it may also become a good compiler for freshmens to learn how to build compilers.

But unfortunately, I don't like the `array` in C

```c
int array[10]; // Ok, that's an array.
// ...	

int max(int array[10]); // But this function takes a pointer instead of an array
int m = max(array); // Well, array decays to pointer int* now.
```

So I decided to create another language that would merely improve C...

And finally I designed ckx which is absolutely not C.

## Basic syntax

> Note : the grammers may change in the further future. Before the version 1.0, everything is uncertain and newer versions may be no compatibility with the former versions.

### Basic structure of a ckx program


	1)  import std.io;
	2)  
	3)  fn main() : i32
	4)  {
	5)  	vi16 a, b;
	6)  	input(a);
	7)  	input(b);
	8)  	vi16 c = a + b;
	9)  	print(c);
	A)  	return 0;
	B)  }
	C)

The first line `import std.io` uses ckx keyword `import` to import `io` utility module from package `std`. As you can see, ckx will have a very powerful module manage system, and we will use this package manage system in the future. But currently I ~~the programmer~~ does not know how to implement this module system. So in the early versions, ckx will include a preprocessor system, importing functions still uses `#include`.

the `function main` is equivalent to the `main` function in C or C++. it shall returns an integer..

### Basic data types

Instead of using type names like `int` and `long`, ckx explicitly label the size/capacity in type names.

* `vi8` `vi16` `vi32` and `vi64` represents signed integers of size 8/16/32/64 bits. 
* `vu8` `vu16` `vu32` and `vu64` represents unsigned integers of their corresponding size.
* `vr32` is mostly equivalent to `float` and `vr64` is mosylt equivalent to `double`
* `vch` is the char type for ckx. however, still I don't know what to use for this type. the default C/C++ `char` of size 8 may be a temporary solution.

### Functions

You can `declare` a function like this:

	1) fn add (vi16 a, vi16 b) : vi16

And you can define a function like this:

    1)  fn add (vi16 a, vi16 b) : vi16
    2)  {
    3)  	return a + b;
    4)  }

And invoking a function will be:

	1) vi16 c = add(a, b);
	2) vi16 d;
	3) d = add(add(a, b), c);

Nothing different from other languages.

### User-defined types

User-defined types grammer are also similar to C. You can use `struct` to define a record type, use `variant` to define a variant/union type, and use `enum` to define an enum type and enumerators.

> example : Defining a struct student. Note no comma after the closing brace. Once defined, student will become a typename.

    1)  struct student
    2)  {
    3)  	vu64 id;
    4)  	vr32 score;
    5)  }

### Pointers

Pointers in ckx is the same as pointers in C. The powerful low-level operator.

> I'm not about to include Garbage collection in this programming language.

    1)  vi8 i;
    2)  vi8 *p = &i;            // Taking address
    3)  vi8 *q = heap_alloc();  // Using heap allocation
    4)  heap_free(q);           // Remember to free!
    5)
    6)  vi8 const* = &i;

> Note: ckx supports `T const*` but does not support `const T*`.

### Arrays

> P0001R0 "Array TS"

Unlike in C, an `array` in ckx means a `range`. That is to say, for any array, it is possible to calculate its size according to its start point and finish point.

There are four "create array" syntax:
    
    type[] array = type[]()(start-point, finish-point);           (1)
    type[] array = type[](array-size)(start-point);               (2)
    type[] array = type[](array-size);                            (3)
    type[] array = type[](array-size){initializer-list};          (4)
    
(1) Create an array according to the given storage, specified by start-point and finish-point.

(2) Create an array according to the given storage, unlike (1), the finishpoint is calculated through n automatically.

(3) Create an array on stack, containing n default-initialized elements.

(4) Create an array on stack, containing n elements, with the first several elements initialized with initializer-list.
 

### Other special features

##### Type casts

* Only integers could be implicitly promoted. for example, promoting from `vi8` to `vi16`, or from `vu8` to `vu64`.
* Signed integers will not be implicitly converted to unsigned ints. Vice versa.
* Integers will not be implicitly converted to floats. And floats will also not be implicitly converted to ints.
* Enumerators are actually integer constants. However, they have their individual type so will not be implicitly converted to integers. Integers also could not be implicitly converted to enums.

##### Function overloading

* Function overloading is one targeted feature of ckx.
* Currently I ~~The programmer~~ am not familiar with function overloading, name mangling... So I need some help.

##### Closures

> Still in progress. If you have a good idea about it, contact me and I'll be appreciated.
