#include <stdlib.h>
#include <stdio.h>

enum
{
    MODULUS = 0x80000000,
    MULTIPLIER = 1103515245,
    INCREMENT = 12345
};

struct RandomGenerator;
void destroy(struct RandomGenerator *);
int next(struct RandomGenerator *);

struct RandomOperations
{
    void (*destroy)(struct RandomGenerator *);
    int (*next)(struct RandomGenerator *);
};

struct RandomOperations operations = {.next = next, .destroy = destroy};

typedef struct RandomGenerator
{
    struct RandomOperations *ops;
    int value;
} RandomGenerator;

int
next(struct RandomGenerator *generator)
{
    generator->value = (int)((MULTIPLIER * (long long) generator->value + INCREMENT) % MODULUS);
    return generator->value;
}

void
destroy(struct RandomGenerator *generator)
{
    free(generator);
    return;
}

RandomGenerator *
random_create(int seed)
{
    RandomGenerator *generator = calloc(1, sizeof(*generator));
    generator->ops = &operations;
    generator->value = seed;
    return generator;
}