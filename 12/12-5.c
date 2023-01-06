#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr,"No expression\n");
        return 1;
    }
    char *tmp_path;
    if (!(tmp_path = getenv("XDG_RUNTIME_DIR"))) {
        if (!(tmp_path = getenv("TMPDIR"))) {
            tmp_path = "/tmp";
        }
    }
    char file_path_c[PATH_MAX];
    char file_path_scr[PATH_MAX];
    pid_t pid = getpid();
    if (sprintf(file_path_c, "%s/%d.c", tmp_path, pid) == -1) {
        fprintf(stderr,"Error calculating temporary directory\n");
        return 1;
    }
    if (sprintf(file_path_scr, "%s/%d.scr", tmp_path, pid) == -1) {
        fprintf(stderr,"Error calculating temporary directory\n");
        return 1;
    }
    errno = 0;
    if (creat(file_path_scr, 0700) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    errno = 0;
    FILE *fd_c = fopen(file_path_c, "w+");
    if (!fd_c) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    errno = 0;
    FILE *fd_scr = fopen(file_path_scr, "w");
    if (!fd_scr) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    errno = 0;
    if (fprintf(fd_c,
                "#include <stdio.h>\n"
                "\n"
                "int\n"
                "main(void)\n"
                "{\n"
                "    char *summon = \"summon\";\n"
                "    char *disqualify = \"disqualify\";\n"
                "    char *reject = \"reject\";\n"
                "    int x;\n"
                "    while(scanf(\"%%d\", &x) == 1) {\n"
                "        printf(\"%%s\\n\", %s);\n"
                "    }\n"
                "    return 0;\n"
                "}", argv[1]) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    errno = 0;
    if (fprintf(fd_scr,
                "#!/bin/bash\n"
                "gcc %s -o %s.exe\n"
                "%s.exe\n"
                "rm %s\n"
                "rm %s.exe\n"
                "rm %s", file_path_c, file_path_c, file_path_c, file_path_c, file_path_c, file_path_scr) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    fclose(fd_scr);
    fclose(fd_c);
    execl("/bin/bash", "bash", file_path_scr, NULL);
    fprintf(stderr, "Error run script\n");
    return 1;
}