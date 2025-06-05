#!/bin/sh
clang++ -o research_machine_code.o -c research_machine_code.cpp -std=c++20 -DNDEBUG -O3 -frtti \
        -fptrauth-vtable-pointer-address-discrimination \
        -fptrauth-vtable-pointer-type-discrimination \
        -fptrauth-type-info-vtable-pointer-discrimination
        #-fptrauth-calls
        #-arch arm64 -target arm64-apple-macos -mcpu=apple-m1 -fPIC
        #-fno-exceptions

objdump -d research_machine_code.o | tee output_research_machine_code.txt

clang++ -o prog research_machine_code.cpp -std=c++20 -DNDEBUG -O3 -frtti \
        -fptrauth-vtable-pointer-address-discrimination \
        -fptrauth-vtable-pointer-type-discrimination \
        -fptrauth-type-info-vtable-pointer-discrimination
        #-fptrauth-calls
        #-arch arm64 -target arm64-apple-macos -mcpu=apple-m1 -fPIC
        #-fno-exceptions

echo ====================== start of program ===========================
./prog
