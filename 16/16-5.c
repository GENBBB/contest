#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <sys/stat.h>

double
integral(int n, double left, double right, double (*func)(double x))
{
    double dx = (right - left) / (double) n;
    double integral = 0;
    for (int i = 0; i < n; i++) {
        integral += func(left + i * dx) * dx;
    }
    return integral;
}

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        return 1;
    }
    double left, right;
    int n;
    if (sscanf(argv[1], "%lf", &left) != 1)
        return 1;
    if (sscanf(argv[2], "%lf", &right) != 1)
        return 1;
    if (sscanf(argv[3], "%d", &n) != 1)
        return 1;
    char *tmp_path;
    if (!(tmp_path = getenv("XDG_RUNTIME_DIR"))) {
        if (!(tmp_path = getenv("TMPDIR"))) {
            tmp_path = "/tmp";
        }
    }
    char file_c[PATH_MAX];
    char file_o[PATH_MAX];
    char file_so[PATH_MAX];
    pid_t pid = getpid();
    sprintf(file_c, "%s/%d.c", tmp_path, pid);
    sprintf(file_o, "%s/%d.o",tmp_path, pid);
    sprintf(file_so, "%s/%d.so", tmp_path, pid);
    FILE *lib = fopen(file_c, "w");
    fclose(lib);
    chmod(file_c, 0777);
    lib = fopen(file_c, "w");
    if (fprintf(lib, "#include <math.h>\n"
    "double func(double x) {\n"
    "return (%s);\n"
    "}\n", argv[4]) < 0)
        return 1;
    fclose(lib);
    char *args_o[] = {
            "gcc", "-m32", "-std=gnu11",  "-lm",  "-fPIC", "-DPIC",
            "-c",      "-o", file_o, file_c, "-lm", NULL
    };
    if (!fork()) {
        execvp("gcc", args_o);
        _exit(1);
    }
    wait(NULL);
    char *args_so[] = {
            "gcc", "-m32", "-std=gnu11", "-fPIC", "-lm",
            "-shared", "-o", file_so, file_o, "-lm", NULL
    };
    if (!fork()) {
        execvp("gcc", args_so);
        _exit(1);
    }
    wait(NULL);
    void *handle = dlopen(file_so, RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        return 1;
    }
    double (*func)(double) = dlsym(handle, "func");
    if (func == NULL) {
        return 1;
    }
    printf("%.10g\n", integral(n, left, right, func));
    dlclose(handle);
    remove(file_c);
    remove(file_o);
    remove(file_so);
    return 0;
}
