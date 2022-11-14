#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

enum
{
    MAX_ARGS_SIZE = 64
};

typedef struct Args
{
    char stack[MAX_ARGS_SIZE];
} Args;

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        return 1;
    }
    void * handle = dlopen(argv[1], RTLD_LAZY);
    if (handle == NULL) {
        printf("%s\n", dlerror());
        return 2;
    }
    void *func = dlsym(handle, argv[2]);
    if (func == NULL) {
        return 3;
    }
    Args args;
    int args_len = 0;
    size_t sign_len = strlen(argv[3]);
    for (int i = 1; i < sign_len; i++) {
        if (argv[3][i] == 's') {
            *(char**)(args.stack + args_len) = argv[i + 3];
            args_len += sizeof(char*);
        } else if (argv[3][i] == 'd') {
            double value;
            sscanf(argv[i+3], "%lf", &value);
            *(double*)(args.stack + args_len) = value;
            args_len += sizeof(double);
        } else if (argv[3][i] == 'i') {
            int value;
            sscanf(argv[i+3], "%d", &value);
            *(int*)(args.stack + args_len) = value;
            args_len += sizeof(int);
        } else {
            return 4;
        }
    }
    if (argv[3][0] == 's') {
        char *result = ((char* (*)())func)(args);
        printf("%s\n", result);
    } else if (argv[3][0] == 'd') {
        double result = ((double (*)())func)(args);
        printf("%.10g\n", result);
    } else if (argv[3][0] == 'i') {
        int result = ((int (*)())func)(args);
        printf("%d\n", result);
    } else if (argv[3][0] == 'v'){
        ((void (*)())func)(args);
    } else {
        return 5;
    }
    if (dlclose(handle)) {
        return 6;
    }
    return 0;
}