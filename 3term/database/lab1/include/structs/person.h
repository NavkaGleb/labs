#ifndef PERSON_H
#define PERSON_H

#include <string.h>

enum sex { unknown = -1, male = 0, female };

typedef struct _person {
    // variables
    long long passport;
    char name[5];
    short sex;

} person_t;

#endif // PERSON_H
