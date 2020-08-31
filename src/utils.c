/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "headers/utils.h"

void trim(char * s, char c) {
    char *t = s;
    while (*s == c)s++;
    if (*s) {
        char* t1 = s;
        while (*s) s++;
        s--;
        while (*s == c) s--;
        while (t1 <= s) {
            *(t++) = *(t1++);
        }
    }
    *t = 0;
}

void split(char *src, const char *separator, char **dest, int *num) {
    /*
            src 源字符串的首地址(buf的地址) 
            separator 指定的分割字符
            dest 接收子字符串的数组
            num 分割后子字符串的个数
     */
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
        return;
    if (separator == NULL || strlen(separator) == 0)
        return;
    pNext = (char *) strtok(src, separator);
    while (pNext != NULL) {
        *dest++ = pNext;
        ++count;
        pNext = (char *) strtok(NULL, separator);
    }
    *num = count;
}

void random_string(int length, char *output) {
    int flag, i;
    srand((unsigned) time(NULL));
    for (i = 0; i < length - 1; i++) {
        flag = rand() % 3;
        switch (flag) {
            case 0:
                output[i] = 'A' + rand() % 26;
                break;
            case 1:
                output[i] = 'a' + rand() % 26;
                break;
            case 2:
                output[i] = '0' + rand() % 10;
                break;
            default:
                output[i] = 'x';
                break;
        }
    }
}

void present_str_array(char** arr, int num) {
    for (int i = 0; i < num; i++) {
        printf("%s\n", arr[i]);
    }
}