#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main ()
{
    char buff[] = "minh|tuesday|alo|thao|wednesday|wut";
    char sender[30];
    char sent_time[30];
    char content[250];
    char *rest = buff;
    while(strlen(rest) != 0) {
        strcpy(sender, strtok_r(rest, "|", &rest));
        strcpy(sent_time, strtok_r(rest, "|", &rest));
        strcpy(content, strtok_r(rest, "|", &rest));
        printf("%s %s %s\n", sender, sent_time, content);
    }
}


