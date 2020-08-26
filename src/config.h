/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   config.h
 * Author: jason
 *
 * Created on 2020年8月23日, 下午5:36
 */

#ifndef CONFIG_H
#define CONFIG_H
#include "ccl/ccl.h"
#define BANLIST "/etc/banned_list"
#define CONF "/etc/mylogin.conf"

#define DEFAULT_TRUST_RETRIES 5
#define DEFAULT_UNTRUST_RETRIES 3
#define DEFAULT_TRUST_RETRIES_ROOT 3
#define DEFAULT_UNTRUST_RETRIES_ROOT 1

const char *get_trusted_host(struct ccl_t*);
int get_trust_retries();
int get_untrust_retries();
int get_trust_retries_root();
int get_untrust_retries_root();
void parse_config(void);
void clearup(void);
void PrintConf(struct ccl_t*);
const char* get_untrusted_host(struct ccl_t*);
struct conf{
    char* blocked[8];
    int _blocked_ct;
    char* trusted[8];
    int _trusted_ct;
    int trust_retries;
    int untrust_retries;
    int trust_retries_root;
    int untrust_retries_root;
}conf,*conf_ptr;
#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H */

