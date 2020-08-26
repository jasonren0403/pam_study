/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   apis.h
 * Author: jason
 *
 * Created on 2020年8月23日, 下午3:51
 */

#ifndef APIS_H
#define APIS_H

#define BUFF_LEN 256
#define MAX_CONFIG_LINES 50

struct ban_info {
    char username[50];
    char host[40];
    char unlock_code[40];
} infos[MAX_CONFIG_LINES];

int parse_ban_info_from_file(const char*);
void printAll(const struct ban_info*, int);
char* get_unlock_str(char*, char*, struct ban_info);
void write_ban(const char*, char* , char* );
void remove_ban(const char* , char* , char* );
int check_ban(char *,char *);
#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* APIS_H */

