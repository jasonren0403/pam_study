/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <string.h>
#include "ccl/ccl.h"
extern void parse_config(void);
extern void PrintConf(ccl_t*);
extern void clearup(void);
extern struct ccl_t config;
int main(int argc,char** argv){
    // -lccl
    //    const struct ccl_pair_t *iter;
        /* Iterate through all key/value pairs */
    //    while((iter = ccl_iterate(&config)) != 0) {
    //      printf("(%s,%s)\n", iter->key, iter->value);
    //    }
    char tmp[256]={0};

    parse_config();
    PrintConf(&config);
    const char* trusted = ccl_get(&config,"trusted");

    printf("trusted:%s\n",trusted);

    const char* blocked = ccl_get(&config,"blocked");
    snprintf(tmp,sizeof(tmp),"%s",blocked);
    printf("blocked:%s\n",tmp);
    clearup();
    return 0;
}
