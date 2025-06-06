#!/bin/sh
clang++ -o research_machine_code.o -c research_machine_code.cpp \
        -std=c++23 -arch arm64e -DNDEBUG -O3
        #-fno-rtti -fno-exceptions \
        #-fptrauth-calls \
        #-fptrauth-vtable-pointer-address-discrimination \
        #-fptrauth-vtable-pointer-type-discrimination \
        #-fptrauth-type-info-vtable-pointer-discrimination

objdump -d research_machine_code.o | tee output_research_machine_code.txt

clang++ -o prog research_machine_code.cpp \
        -std=c++23 -arch arm64e -DNDEBUG -O3
        #-fno-rtti -fno-exceptions \
        #-fptrauth-calls \
        #-fptrauth-vtable-pointer-address-discrimination \
        #-fptrauth-vtable-pointer-type-discrimination \
        #-fptrauth-type-info-vtable-pointer-discrimination

echo ===== build script about to run program ===========================
echo "uname -a"
uname -a
echo "sysctl machdep.cpu.features \| grep -i pac"
sysctl machdep.cpu.features | grep -i pac
echo "sysctl kern.bootargs \| grep -i pac"
sysctl kern.bootargs | grep -i pac
./prog
