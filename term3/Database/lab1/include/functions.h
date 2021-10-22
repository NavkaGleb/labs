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

void scan_country(country_t* country);
void scan_city(city_t* city);

void print_country(const country_t country, const char* title);
void print_index_country(const index_country_t index_country, const char* title);
void print_city(const city_t city, const char* title);
void print_index_city(const index_city_t index_city, const char* title);
void print_countries(void);
bool print_existing_countries(void);
void print_cities(void);
bool print_existing_cities(void);
void print_global_data(const global_data_t global_data);

long long get_eof(FILE* file);
void init_global_data();
global_data_t get_global_data();
void put_global_data(global_data_t global_data);

void insert_master(bool input, char name[COUNTRY_NAME_LENGTH], int area);
bool insert_slave(bool input, char name[CITY_NAME_LENGTH], int population);

country_t* get_master(unsigned long long id);
void get_masters(country_t* countries, size_t size);
city_t* get_slave(unsigned long long id);
void get_slaves(unsigned long long id);

void update_countries(global_data_t* global_data);
void update_cities(global_data_t* global_data);
void update_files(global_data_t* global_data);

void update_master(const country_t country);
void update_masters(country_t* countries, size_t size);
void update_slave(const city_t city);
void update_slaves(const city_t* cities, size_t size);

void delete_master(unsigned long long id);
void delete_slave(unsigned long long id);
void delete_masters();
void delete_slaves();

#endif // FUNCTIONS_H