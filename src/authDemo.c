/*
 * 2020-08-05
 */
#define _XOPEN_SOURCE   /* to use crypt() */

#include<stdio.h>
#include<sys/param.h>
#include<pwd.h>
#include<shadow.h>
#include <crypt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include<security/pam_modules.h>
#include<security/pam_appl.h>
#include<security/pam_ext.h>

#ifndef PAM_EXTERN
#define PAM_EXTERN
#endif


#define MAX_TRY_PER_SESSION 3

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh,int flags,
        int argc,const char *argv[]){
    //对输入的用户名判断是否在本机linux用户中(也就是/etc/passwd)存在

    //通过用户名查询/etc/shadow中加密后的密码与输入的密码加密后进行比较，判断是否相同。
    //linux密码采用SHA-512哈希加密算法。
    //(source:https://blog.csdn.net/qq_42960111/article/details/106388702)

    // -lcrypt -lpam

    const void *user;
    char *authtok;
    struct spwd *my_spwd;
    struct passwd *my_passwd;
    char *encry_passwd; 
    char salt[12];
    //get user name with pam_get_item function
    if(PAM_SUCCESS != pam_get_item(pamh, PAM_USER, &user)) {
	fprintf(stderr, "pam_get_item error!\n");
	return -1;
    }	
    printf("User: %s\n",(char *)user);
    
    int tries = 0;
    int ret;
    while(tries<MAX_TRY_PER_SESSION){
        ret = pam_prompt(pamh,PAM_PROMPT_ECHO_OFF,&authtok,"%s","Input password>> ");
        my_passwd = getpwnam((char*)user);
        if(my_passwd == NULL) {
            printf("User not exist!\n");
            return PAM_USER_UNKNOWN;
        }
        if(strlen(authtok)==0){
            fprintf(stderr,"Password cannot be empty!\n");
            tries++;
        }
        else if(strlen(authtok)<8){
            fprintf(stderr,"Password too short!\n");
            tries++;
        }
        else break;
    }
     
    if(tries == MAX_TRY_PER_SESSION){
        printf("Sorry, you have reached max try.\n");
        return PAM_MAXTRIES;
    }
    printf("Your username is %s and your password is %s\n",(char*) user,authtok);
    
    my_spwd = getspnam((char*) user);
    if(my_spwd == NULL) {
	printf("get shadow password error!\n");
	return PAM_CRED_ERR;
    }
    printf("shadow password is %s\n", my_spwd->sp_pwdp);
//calculate encrypted passwd with crypt function
    strncpy(salt, my_spwd->sp_pwdp, 12);
    encry_passwd = crypt(authtok, salt);	
//compare these two encrypted passwd; if wrong, return -1;
    
    if(strcmp(encry_passwd, my_spwd->sp_pwdp) == 0) {
	printf("My input encrypted passwd is %s\n", encry_passwd);
	printf("Authenticate success!\n");
    } else {
	printf("Wrong password!\n");
	return PAM_AUTH_ERR;
    }
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh,int flags,
        int argc, const char** argv){
        printf("<<<<pam_sm_setcred called\n");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags,
				int argc, const char **argv){
    printf("<<<<pam_sm_chauthtok called\n");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{
printf("<<<<pam_sm_acct_mgmt called\n");
    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{
printf("<<<<pam_sm_open_session called\n");
    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{
printf("<<<<pam_sm_close_session called\n");
    return (PAM_SUCCESS);
}
