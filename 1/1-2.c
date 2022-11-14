#include <stdio.h>
#include <stdint.h>

enum 
{
    SIZE_ALPHABET = 64,
    UNREADLE = -1,
    SIZE_NUMBERS = '9' - '0' + 1,
    SIZE_EN_ALPHABET = 'z' - 'a' + 1,
    THIRD_BIT = 8,
    SECOND_BIT = 4,
    ALL_BIT = 63
};

void 
get_alphabet(char *alphabet)
{
    alphabet[0] = '@';
    alphabet[SIZE_ALPHABET - 1] = '#';
    for (size_t i = 1; i < 1 + SIZE_NUMBERS; i++) {
        alphabet[i] = i - 1 + '0';
    }
    for (size_t i = 1 + SIZE_NUMBERS; i < 1 + SIZE_NUMBERS + SIZE_EN_ALPHABET; i++) {
        alphabet[i] = 'a' + i - SIZE_NUMBERS - 1; 
    }
    for (size_t i = 1 + SIZE_NUMBERS + SIZE_EN_ALPHABET; i < 1 + SIZE_NUMBERS + (SIZE_EN_ALPHABET << 1); i++) {
        alphabet[i] = 'A' + i - SIZE_NUMBERS - 1 - SIZE_EN_ALPHABET;
    }
}

int32_t
get_code(int32_t letter)
{
    if (letter >= '0' && letter <= '9') {
        return letter - '0' + 1;
    } else if (letter >= 'a' && letter <= 'z') {
        return letter - 'a' + 1 + SIZE_NUMBERS;
    } else if (letter >= 'A' && letter <= 'Z') {
        return letter - 'A' + 1 + SIZE_NUMBERS + SIZE_EN_ALPHABET;
    }
    return UNREADLE;
}

int32_t
change_code(int32_t code)
{
    return (code ^ THIRD_BIT) & (ALL_BIT ^ SECOND_BIT);
}

int
main(void)
{
    char alphabet[SIZE_ALPHABET];
    get_alphabet(alphabet);
    int32_t letter;
    while ((letter = getchar()) != EOF) {
        int32_t code = get_code(letter);
        if (code != UNREADLE) {
            code = change_code(code);
            putchar(alphabet[code]);
        }
    }
    return 0;
}
