#include "functions.h"

#include <time.h>

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

    printf("country: { id: %lld, name: %s, area: %d, cities_count: %d, city_pos: % lld }\n",
       country.id, country.name, country.area, country.cities_count, country.city_pos);

}

void print_index_country(const index_country_t index_country) {

    printf("index_country: { id: %lld, pos: %lld }\n", index_country.id, index_country.pos);

}

void print_city(const city_t city) {

    printf("city: { id: %lld, name: %s, population: %d, next: %lld }\n", city.id, city.name, city.population, city.next);

}

long long get_eof(FILE* file) {

    long long end;

    fseek(file, 0, SEEK_END);
    end = ftell(file);
    fseek(file, 0, SEEK_SET);

    return end;

}

country_t get_master(long long id) {

    FILE* data_infile = fopen("../files/country.fl", "r");
    FILE* index_infile = fopen("../files/country.ind", "r");

//    if (data_infile == NULL || index_infile == NULL)
//        return NULL;

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

    fclose(data_infile);
    fclose(index_infile);

    if (!exists)
        printf("no record!\n");

    return country;

}

void update_master(const country_t country) {

    FILE* data_infile = fopen("../files/country.fl", "r");
    FILE* temp_outfile = fopen("../files/temp_country.fl", "w");

    long long end = get_eof(data_infile);
    country_t temp_country;

    while (ftell(data_infile) != end) {

        fread(&temp_country, sizeof(temp_country), 1, data_infile);

        if (temp_country.id == country.id)
            fwrite(&country, sizeof(country), 1, temp_outfile);
        else
            fwrite(&temp_country, sizeof(temp_country), 1, temp_outfile);

    }

    fclose(data_infile);
    fclose(temp_outfile);

    // add error messages!
    remove("../files/country.fl");
    rename("../files/temp_country.fl", "../files/country.fl");

}

void insert_master(void) {

    // init rand
    srand(time(NULL));

    // open files
    FILE* data_outfile = fopen("../files/country.fl", "a");
    FILE* index_outfile = fopen("../files/country.ind", "a");

    if (data_outfile == NULL || index_outfile == NULL)
        return;

    fseek(data_outfile, 0, SEEK_END);

    unsigned long long id = ftell(data_outfile) / sizeof(country_t);
    country_t country;
    index_country_t index_country;

    country.id = id;

    printf("enter name of the country:\n");
    scanf("%s", country.name);

    printf("enter area of the country:\n");
    scanf("%d", &country.area);

    country.cities_count = 0;
    country.city_pos = -1;

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

    FILE* data_outfile = fopen("../files/city.fl", "a");
    FILE* index_outfile = fopen("../files/city.ind", "a");

    if (data_outfile == NULL || index_outfile == NULL)
        return;

    unsigned long long id = ftell(data_outfile) / sizeof(country_t);
    city_t city;
    index_city_t index_city;
    country_t country;

    // init city
    city.id = id;
    printf("enter name of the city:\n");
    scanf("%s", city.name);
    printf("enter population of the city:\n");
    scanf("%d", &city.population);
    printf("enter id of the country:\n");
    scanf("%lld", &country.id);
    country = get_master(country.id);
    city.next = country.city_pos;

    fwrite(&city, sizeof(city), 1, data_outfile);

    // init index_city
    index_city.id = city.id;
    index_city.pos = ftell(data_outfile) - sizeof(city);
    fwrite(&index_city, sizeof(index_city), 1, index_outfile);

    // update country
    ++country.cities_count;
    country.city_pos = index_city.pos;
    update_master(country);

    print_city(city);
    print_country(country);

    fclose(data_outfile);
    fclose(index_outfile);

}