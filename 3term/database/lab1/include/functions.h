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
#include "global_data.h"

int rand_int(int left, int right);
void rand_string(char* string, size_t length);
void rand_country(country_t* p);

void print_country(const country_t country, const char* title);
void print_index_country(const index_country_t index_country, const char* title);
void print_city(const city_t city);
void print_index_city(const index_city_t index_city);
void print_countries(void);
void print_cities(void);
void print_global_data(const global_data_t global_data);

long long get_eof(FILE* file);
void init_global_data();
global_data_t get_global_data();
void put_global_data(global_data_t global_data);

void update_countries();
void update_cities();
void update_files(global_data_t* global_data);

country_t* get_master(unsigned long long id);
city_t* get_slave(unsigned long long id);

void delete_master(unsigned long long id);
void delete_slave(unsigned long long id);

void update_master(const country_t country);
void update_slave(const city_t city);
void update_slaves(const city_t* cities, size_t size);

bool insert_master(void);
bool insert_slave(void);

#endif // FUNCTIONS_H