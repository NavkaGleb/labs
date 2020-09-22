#ifndef CITY_H
#define CITY_H

typedef struct city {
    unsigned long long id;
    char name[10];
    int population;
    unsigned long long country_id;
    long long next;
    bool deleted;
} city_t;

#endif // CITY_H
