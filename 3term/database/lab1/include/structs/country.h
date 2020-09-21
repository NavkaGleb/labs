#ifndef COUNTRY_H
#define COUNTRY_H

typedef struct country {
    unsigned long long id;
    char name[10];
    int area;
    int cities_count;
    long long city_pos;
    bool deleted;
} country_t;

#endif // COUNTRY_H
