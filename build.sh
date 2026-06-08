#!/bin/bash

#gcc tests.c Gen_Table/gen_table.c Groups/group.c Algorithms/snf.c Algorithms/algs.c Algorithms/jules.c Garbage_Collector/garbage_collector.c Linked_List/linked_list.c -o TESTS
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./TESTS

gcc simple_tests.c Gen_Table/gen_table.c Groups/group.c Algorithms/snf.c Algorithms/algs.c Algorithms/jules.c Garbage_Collector/garbage_collector.c Linked_List/linked_list.c -o TESTS


# gcc -g main.c Gen_Table/gen_table.c Linked_List/linked_list.c Groups/group.c Algorithms/algs.c Algorithms/snf.c
# echo "[+] main.c build finished."
