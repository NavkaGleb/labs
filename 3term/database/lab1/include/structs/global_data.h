#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <stdlib.h>

typedef struct global_data {
    unsigned long long country_id;
    unsigned long long city_id;
    unsigned long long countries_count;
    unsigned long long cities_count;
    long long deleted_countries;
    long long deleted_cities;
} global_data_t;

#endif // GLOBAL_DATA_H
