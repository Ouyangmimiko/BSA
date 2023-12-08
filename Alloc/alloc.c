#include "specific.h"
#include <../bsa.h>
#include <stdio.h>
#include <string.h>
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

bool bsa_set(bsa *b, int indx, int value) {
    if (!b || indx < 0 || indx > (1 << BSA_ROWS)-1) {
        return false;
    }
    int row = 0;
    int temp = indx + 1;
    while (temp >>= 1) {
        ++row;
    }
    if (!b->rows[row]) {
        b->rows[row] = (struct element*)(calloc(1 << row, sizeof(struct element)));
        if (!b->rows[row]) {
            return false;
        }
    }
    b->rows[row][indx - (1 << row) + 1].val = value;
    b->rows[row][indx - (1 << row) + 1].flag = true;
    if(b->max_index < indx){
        b->max_index = indx;
    }
    return true;
}


int* bsa_get(bsa *b, int indx) {
    if (!b || indx < 0 || indx > b->max_index) { 
        return NULL;
    }
    int row = 0;
    int temp = indx + 1;
    while (temp >>= 1) {
        ++row;
    }
    if (!b->rows[row]) {
        return NULL;
    }
    if(!b->rows[row][indx - (1 << row) + 1].flag){
        return NULL;
    }
    return &(b->rows[row][indx - (1 << row) + 1].val);
}

bool bsa_delete(bsa *b, int indx) {
    int row = 0;
    int temp = indx + 1;
    while (temp >>= 1) {
        ++row;
    }
    if (!b || indx < 0 || indx > b->max_index || !b->rows[row] || !b->rows[row][indx - (1 << row) + 1].flag) {
        return false;
    }
    
    b->rows[row][indx - (1 << row) + 1].flag = false;
    bool empty = true;
    for (int i = 0; i < (1<<row); ++i) {
        if (b->rows[row][i].flag == true) {
            empty = false;
        }
    }
    if (empty) {
        free(b->rows[row]);
        b->rows[row] = NULL;
    }

    if (b->max_index == indx) {
        b->max_index = -1;
        for(int k = row; k >= 0; k--){
            if(b->rows[k]){
                for (int i = 0; i < (1 << k); ++i) {
                    int idx = (1 << k)- 1 + i;
                    if (b->rows[k][i].flag == true && idx > b->max_index) {
                        b->max_index = idx;
                    }
                }
            }
        }
        
    }

    return true;
}

int bsa_maxindex(bsa *b) {
    return b ? b->max_index : -1;
}

bool bsa_free(bsa *b) {
    if (!b) {
        return false;
    }

    for (int i = 0; i < BSA_ROWS; ++i) {
        if (b->rows[i]) {
            free(b->rows[i]);
        }
    }
    free(b);
    return true;
}


bool bsa_tostring(bsa *b,char *str) {
    if (!b || !str) {
        return false;
    }
    str[0] = '\0'; 
    if(b->max_index == -1) return true;
    for (int row = 0; row < BSA_ROWS; row++) {
        if (b->rows[row]) {
            char rowStr[LISTSTRLEN]; 
            strcpy(rowStr, "{");
            bool flag = false;
            for (int i = 0; i < (1 << row); i++) {
                char elementStr[50]; 
                if (b->rows[row][i].flag && !flag) {
                	flag = true;
                    sprintf(elementStr, "[%d]=%d", (1 << row) - 1 + i, b->rows[row][i].val); 
                    strcat(rowStr, elementStr);
                    if((1 << row) - 1 + i == b->max_index) {
			            if (rowStr[strlen(rowStr) - 2] == ' ') { 
			                rowStr[strlen(rowStr) - 2] = '\0'; 
			            }
			            strcat(rowStr, "}"); 
			            strcat(str, rowStr);
                    	return true;
					}
                }
                else if(b->rows[row][i].flag && flag){
                	 sprintf(elementStr, " [%d]=%d", (1 << row) - 1 + i, b->rows[row][i].val);
                    strcat(rowStr, elementStr); 
                    if((1 << row) - 1 + i == b->max_index) {
			            if (rowStr[strlen(rowStr) - 2] == ' ') { 
			                rowStr[strlen(rowStr) - 2] = '\0'; 
			            }
			            strcat(rowStr, "}"); 
			            strcat(str, rowStr); 
                    	return true;
					}
				}
            }
            if (rowStr[strlen(rowStr) - 2] == ' ') { 
                rowStr[strlen(rowStr) - 2] = '\0'; 
            }
            strcat(rowStr, "}"); 
            strcat(str, rowStr); 
        }
        else{
        	strcat(str,"{}");
		}
    }
    if (str[strlen(str) - 2] == ',') { 
        str[strlen(str) - 2] = '\0'; 
    }
    return str;
}






void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc) {
    if (!b || !func) {
        return;
    }
    int row = 0;
    int temp = b->max_index + 1;
    while (temp >>= 1) {
        ++row;
    }
    for (int i = 0; i <= row; ++i) {
        if (b->rows[i]) {
            int row_size = 1 << i; 
            for (int j = 0; j < row_size; ++j) { 
                if(b->rows[i][j].flag) func(&(b->rows[i][j].val), acc);
            }
        }
    }
}

void test(){
   char str[LISTSTRLEN] = "";
   bsa* tmp = bsa_init();
   assert(tmp);
   assert(bsa_maxindex(tmp)==-1);
   assert(bsa_set(tmp, 1, 7));
   assert(bsa_maxindex(tmp)==1);
   assert(bsa_set(tmp, 0, 0));
   assert(bsa_set(tmp, 1, 15));
   int* p = bsa_get(tmp, 0);
   assert(p);
   assert(*p == 0);
   p = bsa_get(tmp, 1);
   assert(p);
   assert(*p == 15);
   assert(bsa_tostring(tmp, str));
   assert(strcmp(str, "{[0]=0}{[1]=15}")==0);
   assert(bsa_free(tmp));
}