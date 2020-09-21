#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "country.h"
#include "index_country.h"
#include "city.h"
#include "index_city.h"

typedef struct global_data {
    unsigned long long country_id;
    unsigned long long city_id;
    unsigned long long countries_count;
    unsigned long long cities_count;
    unsigned deleted_countries;
    unsigned deleted_cities;
} global_data;

int rand_int(int left, int right);
void rand_string(char* string, size_t length);
void rand_country(country_t* p);

void print_country(const country_t country);
void print_index_country(const index_country_t index_country);
void print_city(const city_t city);
void print_index_city(const index_city_t index_city);
void print_countries(void);
void print_cities(void);

long long get_eof(FILE* file);

country_t* get_master(unsigned long long id);
city_t* get_slave(unsigned long long id);

void delete_master(unsigned long long id);
void delete_slave(unsigned long long id);

void update_master(const country_t country);
void update_slave(void);

void insert_master(void);
void insert_slave(void);

#endif // FUNCTIONS_H