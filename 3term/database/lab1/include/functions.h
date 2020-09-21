#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "country.h"
#include "index_country.h"
#include "city.h"
#include "index_city.h"

enum bool { false = 0, true };

int rand_int(int left, int right);
void rand_string(char* string, size_t length);
void rand_country(country_t* p);

void print_country(const country_t country);
void print_index_country(const index_country_t index_country);
void print_city(const city_t city);
void print_index_city(const index_city_t index_city);

long long get_eof(FILE* file);

country_t get_master(long long id);
void get_slave(void);

void delete_master(void);
void delete_slave(void);

void update_master(const country_t country);
void update_slave(void);

void insert_master(void);
void insert_slave(void);

#endif // FUNCTIONS_H