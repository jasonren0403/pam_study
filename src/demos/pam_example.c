/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <security/pam_modules.h>
#include <security/pam_appl.h>
#include <security/pam_ext.h>
#include <security/pam_misc.h>
/**
 * 认证相关的函数
 */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	const char *ip = NULL;
	int ret = pam_get_item(pamh, PAM_RHOST, (const void **)&ip);
	printf("ret = %d, ip = %s\n", ret, ip?:"nil");
	pam_putenv(pamh, "test=testenv");
	return PAM_SUCCESS;
}

int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	printf("%s-%d\n", __func__, __LINE__);
	return PAM_CRED_ERR;
}

int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	printf("%s-%d\n", __func__, __LINE__);
	return PAM_AUTH_ERR;
}

int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	printf("%s-%d\n", __func__, __LINE__);
	return PAM_SUCCESS;
}

int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	printf("%s-%d\n", __func__, __LINE__);
	return PAM_SUCCESS;
}

int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	printf("%s-%d\n", __func__, __LINE__);
	return PAM_AUTHTOK_ERR;
}

#ifdef PAM_STATIC
struct pam_module _pam_radius_modstruct = {
  "pam_sfos",
  pam_sm_authenticate,
  pam_sm_setcred,
  pam_sm_acct_mgmt,
  pam_sm_open_session,
  pam_sm_close_session,
  pam_sm_chauthtok,
};
#endif
