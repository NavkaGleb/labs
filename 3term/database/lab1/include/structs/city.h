#ifndef CITY_H
#define CITY_H

typedef struct city {
    unsigned long long id;
    char name[10];
    int population;
    unsigned long long next;
} city_t;

#endif // CITY_H
