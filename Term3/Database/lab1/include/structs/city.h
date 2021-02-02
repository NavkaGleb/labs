#ifndef CITY_H
#define CITY_H

#define CITY_NAME_LENGTH 10

typedef struct city {
    unsigned long long id;
    char name[CITY_NAME_LENGTH];
    int population;
    unsigned long long country_id;
    long long next;
    bool deleted;
} city_t;

#endif // CITY_H
