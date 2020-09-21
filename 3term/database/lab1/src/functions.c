#include "functions.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

int rand_int(int left, int right) {

    return rand() % (right - left) + left;

}

void rand_string(char* string, size_t length) {

    static char alphabet[] = { 'a', 'b', 'c', 'd', 'e' };

    for (int i = 0; i < length; ++i)
        string[i] = alphabet[rand() % (sizeof(alphabet) / sizeof(alphabet[0]))];

    string[length] = '\0';

}

void rand_country(country_t* country) {

    country->id = rand() % 100;
    rand_string(country->name, rand_int(1, sizeof(country->name) / sizeof(country->name[0]) - 1));
    country->area = rand_int(10, 1000);
    country->cities_count = 0;
    country->city_pos = -1;

}

void print_country(const country_t country) {

    printf("{ id: %lld, name: %s, area: %d, cities_count: %d, city_pos: % lld }\n",
       country.id, country.name, country.area, country.cities_count, country.city_pos);

}

void print_index_country(const index_country_t index_country) {

    printf("{ id: %lld, pos: %lld }\n", index_country.id, index_country.pos);

}

void print_city(const city_t city) {

    printf("{ id: %lld, name: %s, population: %d, next: %lld }", city.id, city.name, city.population, city.next);

}

long long get_eof(FILE* file) {

    long long end;

    fseek(file, 0, SEEK_END);
    end = ftell(file);
    fseek(file, 0, SEEK_SET);

    return end;

}

void get_master(long long id) {

    FILE* data_infile = fopen("../files/country.fl", "r");
    FILE* index_infile = fopen("../files/country.ind", "r");

    if (data_infile == NULL || index_infile == NULL)
        return;

    printf("read from file!\n");

    index_country_t index_country;
    country_t country;
    long long end = get_eof(index_infile);
    int count = 0;
    short exists = false;

    while (ftell(index_infile) != end) {

        ++count;

        fread(&index_country, sizeof(index_country), 1, index_infile);
        fread(&country, sizeof(country), 1, data_infile);

        print_index_country(index_country);
        print_country(country);

        if (country.id == id) {
            exists = true;
            break;
        }

    }

    printf("\n");
    if (exists)
        print_country(country);
    else
        printf("no record. try to update database!\n");

    fclose(data_infile);
    fclose(index_infile);

}

void insert_master(void) {

    // init rand
    srand(time(NULL));

    // open files
    FILE* data_outfile = fopen("../files/country.fl", "a");
    FILE* index_outfile = fopen("../files/country.ind", "a");

    if (data_outfile == NULL || index_outfile == NULL)
        return;

    country_t country;
    index_country_t index_country;

    rand_country(&country);
    index_country.id = country.id;
    index_country.pos = ftell(data_outfile);

    print_country(country);
    print_index_country(index_country);

    fwrite(&country, sizeof(country), 1, data_outfile);
    fwrite(&index_country, sizeof(index_country), 1, index_outfile);

    fclose(data_outfile);
    fclose(index_outfile);

}

void insert_slave(void) {

    printf("\ninsert slave\n");

    FILE* data_outfile = fopen("../files/city.fl", "a");
    FILE* index_outfile = fopen("../files/city.ind", "a");

    if (data_outfile == NULL || index_outfile == NULL)
        return;

    city_t city;
    index_city_t index_city;

    // init city
    city.id = 1;
    strcpy(city.name, "city_name");
    city.population = rand_int(10, 100);
    city.next = -1;

    print_city(city);

    fclose(data_outfile);
    fclose(index_outfile);

}