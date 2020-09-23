#ifndef COUNTRY_H
#define COUNTRY_H

#define COUNTRY_NAME_LENGTH 10

typedef struct country {
    unsigned long long id;
    char name[COUNTRY_NAME_LENGTH];
    int area;
    int cities_count;
    long long city_pos;
    bool deleted;
} country_t;

#endif // COUNTRY_H
