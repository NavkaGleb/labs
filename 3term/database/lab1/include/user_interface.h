#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdlib.h>
#include <stdbool.h>

#include "functions.h"

unsigned long long ui_scan_id(const char* title);
void print_commands();

bool ui_print_country(country_t* country);
bool ui_print_city(city_t* city);

//void ui_insert_country();
//void ui_insert_city();

void ui_update_country();
void ui_update_city();

void ui_delete_country();
void ui_delete_city();

void ui_run();

#endif // USER_INTERFACE_H
