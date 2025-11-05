Wow, the C++ compiler market has narrowed since this project began. We need a modernish C++ compiler and that means we'll be supporting Microsoft's cl, known as MSVC and g++ known as GNU C++. Clang from Apple & Google effectively emulates the front ends of both while using an LLVM backend so It'll be best efforts to support that as well.

msvc
gcc
clang

The qor_compiler library will eventually be a QOR module
At compile time it will represent the options and definitions specific to the compiler in use.
At runtime it will inform the rest of the QOR which compiler did the build and the properties of the code, like object layout in memory, that are determined by the compiler that was used.

