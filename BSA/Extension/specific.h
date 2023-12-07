#include "../bsa.h"
#include <stdbool.h>
#define BSA_ROWS 30
#ifndef EXTENSION_SPECIFIC_H
#define EXTENSION_SPECIFIC_H
typedef struct element {
    int index;
    int val;
    struct element *next;
} element;

struct bsa{
    element *rows[BSA_ROWS];
    int max_index;
};
#endif 
