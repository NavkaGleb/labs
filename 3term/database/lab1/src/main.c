#include "functions.h"

int main() {

    country_t* country = get_master(0);

    if (country == NULL)
        printf("no record\n");
    else
        print_country(*country);

    free(country);

//    insert_master();
//    insert_master();
//
//    for (int i = 0; i < 3; ++i)
//        insert_slave();
//
//    print_countries();
//
//    printf("\ncities\n");
//    print_cities();

    return 0;

}
