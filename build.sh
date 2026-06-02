#!/bin/bash

# gcc -g tests.c Algorithms/snf.c -o TESTS
# valgrind --leak-check=full --show-leak-kinds=all ./TESTS

gcc -g main.c Gen_Table/gen_table.c Linked_List/linked_list.c Groups/group.c Algorithms/algs.c Algorithms/snf.c
echo "[+] main.c build finished."

valgrind --leak-check=full --show-leak-kinds=all ./a.out