#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>

#include "person.h"

void rand_string(char* string, size_t length);
void rand_person(person_t* p);

long long get_eof(FILE* file);

void get_master(void);
void get_slave(void);

void delete_master(void);
void delete_slave(void);

void update_master(void);
void update_slave(void);

void insert_master(void);
void insert_slave(void);

#endif // FUNCTIONS_H