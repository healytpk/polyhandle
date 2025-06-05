#!/bin/sh
clang++ -o research_machine_code.o -c research_machine_code.cpp -std=c++20 -O3 -DNDEBUG \
        -fptrauth-calls \
        -fptrauth-vtable-pointer-address-discrimination \
        -fptrauth-vtable-pointer-address-discrimination -fptrauth-vtable-pointer-type-discrimination \
        #-fno-exceptions

objdump -d research_machine_code.o | tee output_research_machine_code.txt

clang++ -o prog *.o -std=c++20 -O3 -DNDEBUG \
        -fptrauth-calls \
        -fptrauth-vtable-pointer-address-discrimination \
        -fptrauth-vtable-pointer-address-discrimination -fptrauth-vtable-pointer-type-discrimination \
        #-fno-exceptions

echo ====================== start of program ===========================
./prog
