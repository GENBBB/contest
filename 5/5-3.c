#include <stdio.h>
#include <string.h>
int
parse_rwx_permissions(const char *str) 
{
    if (str == NULL) {
        return -1;
    }
    const char *format = "rwxrwxrwx";
    int mask = 1 ;
    int result = 0;
    int len = strlen(format);
    for (int i = 0; i < len; i++) {
        if (str[i] == '\0') {
            return -1;
        }
    }
    if (str[len] != '\0') {
        return -1;
    }
    for (int i = len-1; i >= 0; i--){
        if (str[i] == format[i]) {
            result += mask;
        } else if (str[i] != '-') {
            return -1;
        }
        mask = mask << 1;
    }
    return result;
}
