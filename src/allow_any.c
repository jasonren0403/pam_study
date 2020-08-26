/*
 * 2020-07-31
 */
#include<stdio.h>
#include<sys/param.h>
#include<pwd.h>

#include<security/pam_modules.h>
#include<security/pam_appl.h>
#include<security/pam_ext.h>

#ifndef PAM_EXTERN
#define PAM_EXTERN
#endif

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh,int flags,
        int argc,const char *argv[]){
    printf("\nThis is User-defined PAM module !\n");
    const char* username;
    char* password;
   
    int retval;
    retval = pam_get_user(pamh,&username,NULL);
    if(retval==PAM_SUCCESS){
        printf("Using local username %s\n",username);
        printf("Welcome in\n");
    }else{
        retval = pam_prompt(pamh,
                PAM_PROMPT_ECHO_OFF,
                &password,
                "%s",
                "Not a local user, please enter your credential>>>>");
        if(retval==PAM_SUCCESS){
            printf("Your credntial is %s\n.",password);
        }else{
            printf("pam_prompt failed!\n");
        }
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