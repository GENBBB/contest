STYPE
bit_reverse(STYPE value) {
    int size_type = 1;
    UTYPE UTYPE_MAX = 1;
    while (!__builtin_mul_overflow(UTYPE_MAX, 2, &UTYPE_MAX)) {
        size_type++;
    }
    UTYPE result = 0;
    UTYPE u_value = (UTYPE) value;
    for (int i = 0; i < size_type; i++) {
        result = result << 1;
        result += u_value & 1;
        u_value = u_value >> 1;
    }
    return result;
}
