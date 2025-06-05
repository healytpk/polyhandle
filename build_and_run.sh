#!/bin/sh
clang++ -o research_machine_code.o -c research_machine_code.cpp -std=c++20 -O3 -DNDEBUG
objdump -d research_machine_code.o | tee output_research_machine_code.txt
