/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "ccl/ccl.h"
int main(int argc,char** argv){
    // -lccl
    struct ccl_t config;

    /* Set configuration file details */
    config.comment_char = '#';
    config.sep_char = '=';
    config.str_char = '"';
    
    parse_config();
    PrintConf(&config);
    clearup();
    return 0;
}
