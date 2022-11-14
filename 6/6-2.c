enum
{
    CURR_DIR = 2,
    PREV_DIR = 3
};

unsigned long long
len_str(char *buf) {
    unsigned long long result = 0, i = 0;
    for (; buf[i] != 0; i++) {
        result++;
    }
    return result;
}

void
normalize_path(char * buf) {
    unsigned long long len = len_str(buf);
    unsigned long long left = len, right = len, n_del = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (buf[i] == '/') {
            right = left;
            left = i;
            if (right - left == CURR_DIR && buf[right-1] == '.') {
                buf[right-1] = '/';
            } else if (right - left == PREV_DIR && buf[right-1] == '.' && buf[right-2] =='.') {
                buf[right-1] = '/';
                buf[right-2] = '/';
                n_del++;
            } else if (n_del) {
                for (int j = left + 1; j < right; j++) {
                    buf[j] = '/';
                }
                n_del--;
            }
        }
    }
    int token_flag = 1;
    int shift = 0;
    for (int i = 1; i < len; i++) {
        if (buf[i] == '/' && token_flag) {
            shift++;
        } else {
            if (buf[i] != '/') {
                token_flag = 0;
            } else {
                token_flag = 1;
            }
            buf[i-shift] = buf[i];
        }
    }
    if (len - shift - 1 == 0) {
        buf[1] = 0;
    } else if (buf[len - shift - 1] != '/') {
        buf[len - shift] = 0;
    } else {
        buf[len - shift - 1] = 0;
    }
    return;
}
