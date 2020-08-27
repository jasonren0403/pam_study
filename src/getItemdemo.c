/*
 * 2020-08-04
 */
#include<stdio.h>
#include<sys/param.h>
#include<pwd.h>
#include <stdlib.h>

#include<security/pam_modules.h>
#include<security/pam_appl.h>
#include<security/pam_ext.h>

#ifndef PAM_EXTERN
#define PAM_EXTERN
#endif

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags,
        int argc, const char *argv[]) {
    printf("pam_sm_authenticate>>>\n");
    const char *user;
    const char *prompt = "[User-defined PAM Module]Input username>> ";
    if (PAM_SUCCESS != pam_get_user(pamh, &user, prompt)) {
        fprintf(stderr, "pam_get_user error!\n");
        return -1;
    }
    printf("Username from pam_get_user: %s\n", user);
    int ret;
    char* pw;
    ret = pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &pw, "%s", "[User-defined PAM Module]Input password>> ");
    printf("Your username is %s and your password is %s\n", user, pw);

    printf("\nTest for pam_get_item !\n");
    const void* service;
    if (PAM_SUCCESS != pam_get_item(pamh, PAM_SERVICE, &service)) {
        fprintf(stderr, "pam_get_item error!\n");
        return -1;
    } else {
        printf("Service: %s\n", (char*) service);
    }

    const void* username;
    if (PAM_SUCCESS != pam_get_item(pamh, PAM_USER, &username)) {
        fprintf(stderr, "pam_get_item error!\n");
        return -1;
    } else {
        printf("Username: %s\n", (char*) username);
    }


    const void* rhost;
    if (PAM_SUCCESS != pam_get_item(pamh, PAM_RHOST, &rhost)) {
        fprintf(stderr, "pam_get_item error!\n");
        return -1;
    } else {
        printf("Rhost: %s\n", (char*) rhost);
    }
    return PAM_SUCCESS;
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
