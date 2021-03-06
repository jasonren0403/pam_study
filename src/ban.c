#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <bsd/string.h>

#include "headers/apis.h"
#include "headers/utils.h"
#include "headers/config.h"

void write_ban(const char* filepath, char *host, char *username) {
    if (strlen(host) == 0) host = "localhost";
    FILE* p = fopen(filepath, "r");
    char path_tmp[256] = {0};
    snprintf(path_tmp, sizeof (path_tmp), "%s.tmp", filepath);
    FILE* tmp = fopen(path_tmp, "a+");
    setbuf(tmp,NULL);
    if (p == NULL) {
        fprintf(stderr, "open file %s failed!\n", filepath);
        return;
    }
    char *line = (char *) malloc(BUFF_LEN * sizeof (char));
    while (fgets(line, BUFF_LEN, p) != NULL) {
        printf("%s\n",line);
        fputs(line,tmp);
    }
    
    char *buffer = (char *) malloc(40 * sizeof (char));
    random_string(32, buffer);
    fseek(tmp,0,SEEK_END);
    fprintf(tmp, "%s@%s %s\n", username, host, buffer); //not ok
    fflush(tmp);
    fclose(tmp);
    printf("%s:%s->ban list\n", username, host); //ok
    
    free(buffer);
    free(line);

    fclose(p);
    remove(filepath);
    rename(path_tmp, filepath);
}

void remove_ban(const char* filepath, char *host, char *username) {
    if (strlen(host) == 0) host = "localhost";
    FILE* p = fopen(filepath, "r");
    char path_tmp[256] = {0};
    snprintf(path_tmp, sizeof (path_tmp), "%s.tmp", filepath);
    FILE* tmp = fopen(path_tmp, "a+");
    if (p == NULL) {
        fprintf(stderr, "open file %s failed!\n", filepath);
        return;
    }
    int num;
    char *line = (char *) malloc(BUFF_LEN * sizeof (char));
    while (fgets(line, BUFF_LEN, p) != NULL) {
        char *revbuf[8] = {0};
        char _tmp[256] = {0};
        strncpy(_tmp, line, strlen(line));
        //        printf("%s",tmp);
        trim(line, '\n');
        split(line, " ", revbuf, &num);

        split(revbuf[0], "@", revbuf, &num);
        if (num != 2) {
            printf("%d\n", num);
            fprintf(tmp, "%s", _tmp);
            continue;
        }
        //        printf("%s,%s %s,%s\n",revbuf[0],username,revbuf[1],host);
        if (strcmp(revbuf[0], username) == 0 && strcmp(revbuf[1], host) == 0) {
            printf("%s:%s removed from ban list\n", username, host);
            continue; //delete this
        } else {
            fprintf(tmp, "%s", _tmp);
        }
    }
    fclose(tmp);
    fclose(p);
    remove(filepath);
    rename(path_tmp, filepath);
}

int check_ban(char *host, char *user) {
    if (strlen(host) == 0) host = "localhost";
    printf("checking if account[%s@%s] is locked...\n", user, host);
    int ct = parse_ban_info_from_file(BANLIST);
    for (int i = 0; i < ct; i++) {
        char* unc_str = get_unlock_str(user, host, infos[i]);
        if (unc_str) {
            return 1;
        }
    }
    return 0;
}

void printAll(const struct ban_info* _infos, int count) {
    printf("PrintAll count:%d\n", count);
    for (int i = 0; i < count; i++) {
        printf("Line %d:%s %s %s\n", i + 1, _infos[i].host, _infos[i].username, _infos[i].unlock_code);
    }
}

int parse_ban_info_from_file(const char *filepath) {
    FILE *fp;
    if (NULL == (fp = fopen(filepath, "rw"))) {
        printf("Open file %s failed\n", filepath);
        exit(0);
    }
    char *line = (char *) malloc(BUFF_LEN * sizeof (char));

    int current = 0;
    int num = 0;
    while (fgets(line, BUFF_LEN, fp) != NULL && current <= MAX_CONFIG_LINES)//逐行读取数据
    {
        char *revbuf[8] = {0};
        split(line, " ", revbuf, &num);
        if (num != 2) {
            printf("Will ignore this bad config: %s\n", line); //user@host unlock_code
            continue;
        }
        trim(revbuf[1], '\n');
        snprintf(infos[current].unlock_code, sizeof (infos[current].unlock_code), "%s", revbuf[1]);
        //        infos[current].unlock_code = revbuf[1];
        //        printf("unlock code:%s\n",infos[current].unlock_code);
        //        printf("%s,%s\n", revbuf[0], revbuf[1]);
        //        memset(revbuf,0,sizeof(revbuf));
        split(revbuf[0], "@", revbuf, &num);
        if (num != 2) continue;
        //        infos[current].username = revbuf[0];
        snprintf(infos[current].username, sizeof (infos[current].username), "%s", revbuf[0]);
        snprintf(infos[current].host, sizeof (infos[current].host), "%s", revbuf[1]);
        //        infos[current].host = revbuf[1];
        //        printf("uname:%s,host:%s\n", infos[current].username, infos[current].host);
        current++;
    }
    //    printAll(infos,current);
    free(line);
    fclose(fp);
    return current;
}

char* get_unlock_str(char* user, char* host, struct ban_info _info) {
    //    printf("[get_unlock_str()] User:%s,host:%s\n",user,host);
    char *tmp = {0};
    if (strlen(host) == 0) host = "localhost";
    //    printf("[Debug]Comparing %s with %s\n",user,_info.username);
    if (strcmp(_info.host, "%") == 0) {
        // user@% at any host
        if (strcmp(user, _info.username) == 0) {
            tmp = (char*) malloc(strlen(_info.unlock_code + 1));
            strlcpy(tmp, _info.unlock_code, strlen(_info.unlock_code + 2));
        }
    } else {
        //        printf("[Debug]Comparing %s with %s\n",host,_info.host);
        if (strcmp(user, _info.username) == 0 && strcmp(host, _info.host) == 0) {
            tmp = (char*) malloc(strlen(_info.unlock_code + 1));
            strlcpy(tmp, _info.unlock_code, strlen(_info.unlock_code) + 2);
            //            printf("You are banned!You need %s to unlock. %ld\n", tmp,strlen(tmp));       
        }
    }
    //    printf("[Debug]Returning %s\n",tmp);
    return tmp;
}

