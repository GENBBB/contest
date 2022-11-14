#include <stdio.h>
int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++){
        int value;
        sscanf(argv[i], "%o", &value);
        char buf[10];
        snprintf(buf, 10, "rwxrwxrwx");
        int mask = 1;
        for (int i = 8; i >= 0; i--){
            if ((mask & value) == 0) {
                buf[i] = '-';
            }
            mask = mask << 1;
        }
        printf("%s\n", buf);
    }  
    return 0;
}
