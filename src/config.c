/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccl/ccl.h"
#include "headers/config.h"
#include "headers/utils.h"
struct ccl_t config;

void parse_config()
{
    /* Set configuration file details */
    config.comment_char = '#';
    config.sep_char = '=';
    config.str_char = '"';
    int tmp;
    if(ccl_parse(&config, CONF)){
        printf("parse error!\n");
        return;
    }
//    conf.blocked = NULL;
//    conf.trusted = NULL;
    memset(conf.blocked,0,sizeof(conf.blocked));
    memset(conf.trusted,0,sizeof(conf.trusted));
    
    const char* blocked = ccl_get(&config,"blocked");
    
    const char* trusted = ccl_get(&config,"trusted");

    const char* temp = ccl_get(&config,"trust_retries");
    if(temp) conf.trust_retries = atoi(temp);

    const char* temp2 = ccl_get(&config,"trust_retries_root");
    if(temp2) conf.trust_retries_root = atoi(temp2);
    
    const char* temp3 = ccl_get(&config,"untrust_retries");
    if(temp3) conf.untrust_retries = atoi(temp3);
    
    const char* temp4 = ccl_get(&config,"untrust_retries_root");
    if(temp4) conf.untrust_retries_root = atoi(temp4);
    
    const char* temp5 = ccl_get(&config,"retries_default");
    if(temp5) conf.normal_retries = atoi(temp5);
    
    const char* temp6 = ccl_get(&config,"normal_retries_root");
    if(temp6) conf.normal_retries_root = atoi(temp6);
    if(blocked!=NULL) {      
        split((char*)blocked,",",conf.blocked,&tmp);
        conf._blocked_ct = tmp;
    }
    if(trusted!=NULL) {
        split((char*)trusted,",",conf.trusted,&tmp);
        conf._trusted_ct = tmp;
    }
    conf_ptr = &conf;
}

void PrintConf(struct ccl_t* config)
{
    if(!config||!conf_ptr)
    {
        parse_config();
    }
    printf("Trust retries:%d\n",get_trust_retries());
    printf("Blocked retries:%d\n",get_untrust_retries());
    printf("Trusted hosts:\n");
    present_str_array(conf_ptr->trusted,conf_ptr->_trusted_ct);
    printf("Blocked hosts:\n");
    present_str_array(conf_ptr->blocked,conf_ptr->_blocked_ct);
}

const char* get_trusted_host(struct ccl_t* config)
{
    if(!config||!conf_ptr) {
        parse_config();
    }
    
    const char *tmpstr = ccl_get(config,"trusted");
    if(!tmpstr) return NULL;
    
    return tmpstr;
}

const char* get_untrusted_host(struct ccl_t* config)
{
    if(!config||!conf_ptr) {
        parse_config();
    }
    const char *tmpstr = ccl_get(config,"blocked");
    if(!tmpstr) return NULL;
    
    return tmpstr;
}

int get_trust_retries(){
    if(!conf_ptr) {
        parse_config();
    }
    if(conf.trust_retries==0) return DEFAULT_TRUST_RETRIES;
    else return conf.trust_retries;
}
int get_untrust_retries(){
    if(!conf_ptr) {
        parse_config();
    }
    if(conf.untrust_retries==0) return DEFAULT_UNTRUST_RETRIES;
    else return conf.untrust_retries;
}
int get_trust_retries_root(){
    if(!conf_ptr) {
        parse_config();
    }
    if(conf.trust_retries_root==0) return DEFAULT_TRUST_RETRIES_ROOT;
    else return conf.trust_retries_root;
}

int get_normal_retries(){
    if(!conf_ptr){
        parse_config();
    }
    if(conf.normal_retries==0) return DEFAULT_RETRIES;
    else return conf.normal_retries;
}

int get_normal_retries_root(){
    if(!conf_ptr){
        parse_config();
    }
    if(conf.normal_retries_root==0) return DEFAULT_RETRIES_ROOT;
    else return conf.normal_retries_root;
}

int get_untrust_retries_root(){
    if(!conf_ptr) {
        parse_config();
    }
    if(conf.trust_retries==0) return DEFAULT_UNTRUST_RETRIES_ROOT;
    else return conf.untrust_retries_root;
}

void clearup()
{
    if(conf_ptr) {
        ccl_release(&config);
        conf_ptr = NULL;
    }
}


