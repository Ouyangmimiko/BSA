#include "../bsa.h"
#include <stdbool.h>
#define BSA_ROWS 30
#ifndef ALLOC_SPECIFIC_H
#define ALLOC_SPECIFIC_H
struct element{
   int val;
   bool flag;
} ;
struct bsa{
    struct element *rows[BSA_ROWS];      
    int max_index;  
};
#endif 

