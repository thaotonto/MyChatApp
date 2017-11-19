#include <stdio.h>
int main(int argc, char const *argv[])
{
    char *abc = "abc";
    char buffer[20];
    strcpy(buffer, abc);
    printf("%s\n", buffer);
    return 0;
}
