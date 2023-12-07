#include "specific.h"
#include <../bsa.h>
#include <stdio.h>
#include <string.h>
#define BSA_ROWS 30
#define LISTSTRLEN 1000

bsa* bsa_init(void) {
    bsa *b = (bsa*)malloc(sizeof(bsa));
    if (!b) {
        return NULL; 
    }
    for (int i = 0; i < BSA_ROWS; i++) {
        b->rows[i] = NULL; 
    }
    b->max_index = -1;
    return b;
}

bool bsa_set(bsa *b, int index, int value) {
    if (!b || index < 0 || index > (1 << BSA_ROWS) - 1) {
        return false; 
    }

    int row = 0;
    int temp = index + 1;
    while (temp >>= 1) {
        ++row; 
    }

    element **current = &(b->rows[row]);
    while (*current && (*current)->index != index) {
        current = &((*current)->next); 
    }

    if (*current && (*current)->index == index) {
        (*current)->val = value;
    } else {
        element *new_element = (element*)malloc(sizeof(element));
        if (!new_element) {
            return false; 
        }
        new_element->index = index;
        new_element->val = value;
        new_element->next = *current; 
        *current = new_element; 
    }

    if (index > b->max_index) {
        b->max_index = index;
    }

    return true;
}


int* bsa_get(bsa *b, int index) {
    if (!b || index < 0 || index > b->max_index) {
        return NULL; 
    }

    int row = 0;
    int temp = index + 1;
    while (temp >>= 1) {
        ++row;
    }

    element *current = b->rows[row];
    while (current) {
        if (current->index == index) {
            return &(current->val); 
        }
        current = current->next; 
    }
    return NULL; 
}


bool bsa_delete(bsa *b, int index) {
    if (!b || index < 0 || index > b->max_index) {
        return false; 
    }

    int row = 0;
    int temp = index + 1;
    while (temp >>= 1) {
        ++row; 
    }

    element **current = &(b->rows[row]);
    while (*current && (*current)->index != index) {
        current = &((*current)->next);
    }
    if (*current && (*current)->index == index) {
        element *to_delete = *current;
        *current = (*current)->next; 
        free(to_delete); 
        if (index == b->max_index) {
            b->max_index = -1;
            for (int i = 0; i < BSA_ROWS; ++i) {
                element *el = b->rows[i];
                while (el) {
                    if (el->index > b->max_index) {
                        b->max_index = el->index;
                    }
                    el = el->next;
                }
            }
        }

        return true;
    }

    return false; 
}

int bsa_maxindex(bsa *b){
    if (!b) {
        return -1; 
    }
    return b->max_index; 
}

bool bsa_free(bsa *b) {
    if (!b) {
        return false;
    }

    for (int i = 0; i < BSA_ROWS; ++i) {
        element *current = b->rows[i];
        while (current) {
            element *to_delete = current;
            current = current->next; 
            free(to_delete); 
        }
        b->rows[i] = NULL; 
    }
    free(b); 
    return true;
}



void bsa_foreach(void (*func)(int *value, int *acc), bsa *b, int *acc) {
    if (!b || !func) {
        return; 
    }

    for (int i = 0; i < BSA_ROWS; ++i) {
        element *current = b->rows[i];
        while (current) {
            func(&(current->val), acc); 
            current = current->next; 
        }
    }
}
void test(){
    
}