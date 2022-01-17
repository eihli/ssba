#include <stdio.h>
#include <string.h>

/*
**
*/
int split_on(char delim, char *string)
{
    int i = 0, count = 0;
    while (string[i] != '\0') {
        if (string[i] == delim) {
            string[i] = '\0';
            count++;
        }
        i++;
    }
    return count;
}

