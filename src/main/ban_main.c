/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <unistd.h>  // for int getopt(int argc, char * const argv[], const char *optstring)
#include <getopt.h>  // for int getopt_long
#include <stdlib.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/apis.h"

static const struct option long_options[] = {
    {"user", required_argument, NULL, 'u'},
    {"host", required_argument, NULL, 'h'},
    {"help", no_argument, NULL, '0'},
    {"check", no_argument, NULL, 'c'},
    {"new", no_argument, NULL, 'n'},
    {"remove", no_argument, NULL, 'r'},
    {0, 0, 0, 0},
};

int main(int argc, char *argv[]) {
    char user[100]={0};
    char host[100]={0};
    if (argc == 1) {
        printf("Usage:./ban -u[user] -h[host], --help for this help\n");
        exit(1);
    }
    int opt,ret;
    opterr = 0;
    while ((opt = getopt_long(argc, argv, "u:h:crn", long_options, NULL)) != -1) {
        //optarg is global
        switch (opt) {
            case 'u':
                snprintf(user, sizeof (user), "%s", optarg);
//                                printf("User:%s\n", user);
                break;
            case 'h':
                snprintf(host, sizeof (host), "%s", optarg);
                //                printf("Host:%s\n", host);
                break;
            case 'c': //check
                if(strlen(user)==0||strlen(host)==0) {
                    fprintf(stderr,"Error:option -c[heck] must be put after -u[ser] and -h[ost]\n");
                    exit(-1);
                }
                ret = check_ban(host, user);
                if (ret) {
                    printf("You need to unlock!\n");
                } else {
                    printf("You don't need to unlock!\n");
                }
                break;
            case 'r': //remove
                remove_ban(BANLIST, host, user);
                break;
            case 'n': //new
                write_ban(BANLIST, host, user);
                break;
            default:
                //非法参数处理，也可以使用case来处理，？表示无效的选项，：表示选项缺少参数
                printf("Usage:./ban -u[user] -h[host], --help for this help\n");
                printf("-c[check] -r[remove] -n[new] operation for specified user@host\n");
                exit(1);
                break;

        }
    }
    return 0;
}