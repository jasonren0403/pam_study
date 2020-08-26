/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#define _XOPEN_SOURCE   /* to use crypt() */
#include<pwd.h>
#include<shadow.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <stdlib.h>
#include <bsd/string.h>  // for size_t strlcat(char *dst, const char *src, size_t size) 
// and strlcpy()
/*

int main(int argc, const char **argv) {
    // -lbsd -lcrypt should use sudo to run this
    char user[100];
    char newpw[400];
    if (argc != 3) {
        printf("Usage: sudo ./changepw <user> <password>\n");
        exit(-1);
    } else if (sizeof (argv[1]) >= 90) {
        printf("parameter too long!\n");
        exit(-1);
    } else {
        strlcpy(user, argv[1], sizeof (user));
        strlcpy(newpw, argv[2], sizeof (newpw));
    }
    char salt[12];
    char *encrypt_passwd;
    struct spwd *my_spwd;
    struct passwd *my_passwd;
    
    my_passwd = getpwnam(user);
    if (my_passwd == NULL) {
        printf("Error:User not exist!\n");
        return -1;
    }
//    printf("%s\n",my_passwd->);
    setspent();
    while((my_spwd=getspent())!=NULL){
        if(strcmp(user,my_spwd->sp_namp)==0) break;
    }
    if(!my_spwd) return -1;
    strncpy(salt, my_spwd->sp_pwdp, 12);
    encrypt_passwd = crypt(newpw, salt);
    endspent();
    printf("Encrypted password: %s\n",encrypt_passwd);
    return 0;
} 
 */