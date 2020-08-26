/*
 * 2020-08-12
 */
#define _XOPEN_SOURCE   /* to use crypt() */

#include<stdio.h>
#include<sys/param.h>
#include<pwd.h>
#include<shadow.h>
#include <crypt.h> // -lcrypt
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "apis.h"
#include "utils.h"
#include "config.h" //-lccl

// -lpam
#include<security/pam_modules.h>
#include<security/pam_appl.h>
#include<security/pam_ext.h>

// -lbsd
#include <bsd/string.h>  // for size_t strlcat(char *dst, const char *src, size_t size) 
// and strlcpy()

#ifndef PAM_EXTERN
#define PAM_EXTERN
#endif

#define PAM_SM_AUTH

int account_locked(const void*, const void*);
int unlock_account(pam_handle_t*, const void*, const void*);
int trusted_host(const void*, const void*);
int login(pam_handle_t*, const void*, const void*, int);

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags,
        int argc, const char *argv[]) {
    const void* user = NULL;
    const void* rhost = NULL;
    
    int ret;
    ret = pam_prompt(pamh,PAM_TEXT_INFO,NULL,
            "This is User-defined PAM module!\n"
            "This module will control logins to this host.\n");
    //get user name with pam_get_item function
    if (PAM_SUCCESS != pam_get_item(pamh, PAM_USER, &user) ||
            PAM_SUCCESS != pam_get_item(pamh, PAM_RHOST, &rhost)) {
        fprintf(stderr, "pam_get_item error!\n");
        return -1;
    }
    printf("User:%s remote_host:%s\n", (char *) user,(char *)rhost);
    struct passwd *my_pwd;
    my_pwd = getpwnam((char*) user);
    if(my_pwd == NULL) {
         pam_prompt(pamh,PAM_ERROR_MSG,NULL,"User not exist!\n");
         return PAM_USER_UNKNOWN;
    }
    int uid = my_pwd->pw_uid;
    if (uid == 0) {
        printf("login as root\n");
//         current user is root
        if (account_locked(user, rhost) != 0) {
            unlock_account(pamh, user, rhost);
            ret = login(pamh, user, rhost, get_trust_retries_root());
        } else {
            int is_trusted = trusted_host(user, rhost);
            ret = login(pamh, user, rhost, is_trusted ? get_trust_retries_root() : get_untrust_retries_root());
        }
    } else {
        printf("login as normal user\n");
//         normal user
        if (account_locked(user, rhost) != 0) {
            unlock_account(pamh, user, rhost);
            ret = login(pamh, user, rhost, get_trust_retries());
        } else {
            int is_trusted = trusted_host(user, rhost);
            ret = login(pamh, user, rhost, is_trusted ? get_trust_retries() : get_untrust_retries());
        }
    }
    return PAM_SUCCESS;
}

// if 0, the account is ok,otherwise it's locked
int account_locked(const void* user, const void* host) {
    if (strlen((char*) host) == 0) host = "localhost";
    printf("checking if account[%s@%s] is locked...\n", (char *) user, (char *) host);
    return check_ban((char *)host, (char *)user);
}

int unlock_account(pam_handle_t *pamh, const void* user, const void* host) {
    printf("Unlocking account\n");
    char *authtok,*unc_str = NULL;
    char tmp[50] = {0};
    int ct = parse_ban_info_from_file(BANLIST);

    for (int i = 0; i < ct; i++) {
        unc_str = get_unlock_str((char*)user, (char*)host, infos[i]);
//        printf("%s\n",get_unlock_str((char*)user, (char*)host, infos[i]));
        if (unc_str!=NULL){
            strlcpy(tmp,unc_str,sizeof(tmp));
            printf("[%s]\n",tmp);
            free(unc_str);
            break;
        }   
    }   
    
    if(strlen(tmp)==0)
    {
//        printf("%s\n",tmp);
        printf("No unlock code,please contact administrator\n");
        return -1;
    }
    
    while(1){
//        memset(authtok,0,256);
        int ret = pam_prompt(pamh, PAM_PROMPT_ECHO_ON, &authtok, "%s", "Input unlock code>> ");
        printf("%s,%s\n",tmp,authtok);
        if(strcmp(tmp,authtok)==0){
            remove_ban(BANLIST,(char*)host,(char*)user);
            printf("Unlock success!\n");
            return 0;
        }
        pam_prompt(pamh,PAM_TEXT_INFO,&authtok,"Incorrect code,please try again!");
    }
    return 0;
}

int login(pam_handle_t *pamh, const void* user, const void* host, int retries) {
    printf("checking password\n");
    int tries = 0, ret;
    char salt[12];
    char *authtok, *encrypt_passwd;
    struct spwd *my_spwd;
    struct passwd *my_passwd;
    if (strlen((char*) host) == 0) host = "localhost";
    while (tries < retries) {
        printf("%d trial(s) left\n", (retries - tries));
        ret = pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &authtok, "%s", "Input password>> ");
        my_passwd = getpwnam((char*) user);
        if (my_passwd == NULL) {
            printf("User not exist!\n");
            return -1;
        }
        my_spwd = getspnam((char*) user);
        if (my_spwd == NULL) {
            printf("get shadow password error!\n");
            return -1;
        }
        strncpy(salt, my_spwd->sp_pwdp, 12);
        encrypt_passwd = crypt(authtok, salt);
        if (strcmp(encrypt_passwd, my_spwd->sp_pwdp) == 0) {
            printf("Authenticate success!\n");
            return 0;
        } else {
            tries++;
            if (strcmp("root", user) == 0 && strcmp("localhost", host) == 0) {
                tries--;
                //prevent root@localhost from being locked
                //todo:This should be changed 
            }
            if (tries != retries) {
                printf("Wrong username or password,try again.\n");
            }
        }
    }
    printf("You hava reached max try. Your account %s@%s will be blocked.\n", (char *) user, (char *) host);
    // block the account
    write_ban(CONF,(char*)host,(char*)user);
    return -1;
}

// 1---trusted 0----untrusted -1----remain default

int trusted_host(const void* user, const void* host) {
    char tmp[256];
    parse_config();
    for (int i = 0; i < conf._trusted_ct; i++) {
        snprintf(tmp,sizeof(tmp),"%s@%s",(char*)user,(char*)host);
        if(strcmp(conf.trusted[i],tmp)==0) return 1;
    }
    for (int i = 0; i < conf._blocked_ct; i++) {
        snprintf(tmp,sizeof(tmp),"%s@%s",(char*)user,(char*)host);
        if(strcmp(conf.blocked[i],tmp)==0) return 0;
    }
    clearup();
    return -1;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags,
        int argc, const char** argv) {
    printf("<<<<pam_sm_setcred called\n");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags,
        int argc, const char **argv) {
    printf("<<<<pam_sm_chauthtok called\n");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
        int argc, const char *argv[]) {
    printf("<<<<pam_sm_acct_mgmt called\n");
    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags,
        int argc, const char *argv[]) {
    printf("<<<<pam_sm_open_session called\n");
    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags,
        int argc, const char *argv[]) {
    printf("<<<<pam_sm_close_session called\n");
    return (PAM_SUCCESS);
}