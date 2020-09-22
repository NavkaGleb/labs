#include "functions.h"

#include <time.h>
#include <fort.h>

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

void print_country(const country_t country, const char* title) {
    printf("%s {\n\tid: %lld,\n\tname: %s,\n\tarea: %d,\n\tcities_count: %d,\n\tcity_pos: %lld,\n\tdeleted: %d\n}\n",
       title,
       country.id,
       country.name,
       country.area,
       country.cities_count,
       country.city_pos,
       country.deleted
   );
}

void print_index_country(const index_country_t index_country, const char* title) {
    printf("%s { id: %lld, pos: %lld }\n", title, index_country.id, index_country.pos);
}

void print_city(const city_t city) {

    printf("city: { id: %lld, name: %s, population: %d, country_id: %lld, next: %lld, deleted: %d }\n",
       city.id, city.name, city.population, city.country_id, city.next, city.deleted);

}

void print_index_city(const index_city_t index_city) {

    printf("index_country: { id: %lld, pos: %lld }\n", index_city.id, index_city.pos);

}

void print_countries() {

    FILE* index_infile = fopen("../files/country.ind", "r");
    FILE* data_infile = fopen("../files/country.fl", "r");

    if (index_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'country.ind' | functions::print_countries");
        exit(1);
    }

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'country.fl' | functions::print_countries");
        exit(1);
    }

    long long end = get_eof(index_infile);
    index_country_t index_country;
    country_t country;
    ft_table_t* table = ft_create_table();
    char id_buffer[10];
    char area_buffer[10];
    char cities_count_buffer[10];
    char city_pos_buffer[10];
    char deleted_buffer[2];

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "id", "name", "area", "cities_count", "city_pos", "deleted");

    while(ftell(index_infile) != end) {
        fread(&index_country, sizeof(index_country), 1, index_infile);
        fread(&country, sizeof(country), 1, data_infile);

        itoa(country.id, id_buffer, 10);
        itoa(country.area, area_buffer, 10);
        itoa(country.cities_count, cities_count_buffer, 10);
        itoa(country.city_pos, city_pos_buffer, 10);
        itoa(country.deleted, deleted_buffer, 10);

        ft_write_ln(table, id_buffer, country.name, area_buffer, cities_count_buffer, city_pos_buffer, deleted_buffer);
    }

    printf("%s\n", ft_to_string(table));

    ft_destroy_table(table);
    fclose(index_infile);
    fclose(data_infile);

}

void print_cities() {

    FILE* data_infile = fopen("../files/city.fl", "r");

    if (data_infile == NULL)
        return;

    long long end = get_eof(data_infile);
    city_t city;

    while (ftell(data_infile) != end) {
        fread(&city, sizeof(city), 1, data_infile);
        print_city(city);
    }

    fclose(data_infile);

}

void print_global_data(const global_data_t global_data) {

    printf("{ country_id: %lld, city_id: %lld, countries_count: %lld, cities_count: %lld, deleted_country: %lld, deleted_city: %lld }\n",
       global_data.country_id,
       global_data.city_id,
       global_data.countries_count,
       global_data.cities_count,
       global_data.deleted_countries,
       global_data.deleted_cities
   );

}

long long get_eof(FILE* file) {

    long long end;

    fseek(file, 0, SEEK_END);
    end = ftell(file);
    fseek(file, 0, SEEK_SET);

    return end;

}

void init_global_data() {

    FILE* global_data_outfile = fopen("../files/global_data.dat", "rb+");
    long long end = get_eof(global_data_outfile);

    if (end == 0) {

        global_data_t global_data = { 0, 0, 0, 0, 0, 0 };
        fwrite(&global_data, sizeof(global_data), 1, global_data_outfile);

    }

    fclose(global_data_outfile);

}

global_data_t get_global_data() {

    FILE* global_data_infile = fopen("../files/global_data.dat", "r");

    if (global_data_infile == NULL) {

        printf("failed to open the file | functions::get_global_data");
        exit(1);

    }

    global_data_t global_data;
    fread(&global_data, sizeof(global_data), 1, global_data_infile);
    fclose(global_data_infile);

    return global_data;

}

void put_global_data(global_data_t global_data) {

    FILE* global_data_outfile = fopen("../files/global_data.dat", "w");

    if (global_data_outfile == NULL) {

        printf("failed to open the file | functions::put_global_data");
        exit(1);

    }

    update_files(&global_data);

    fwrite(&global_data, sizeof(global_data), 1, global_data_outfile);
    fclose(global_data_outfile);

}

void update_countries() {

    FILE* data_infile = fopen("../files/country.fl", "r");
    FILE* temp_data_outfile = fopen("../files/temp_country.fl", "w");
    FILE* index_outfile = fopen("../files/country.ind", "w");

    if (data_infile == NULL) {

        printf("[ ERROR ] failed to open the file 'country.fl' in functions::update_cities\n");
        exit(1);

    }

    long long end = get_eof(data_infile);
    index_country_t index_country;
    country_t country;
    int deleted = 0;

    while (ftell(data_infile) != end) {

        fread(&country, sizeof(country), 1, data_infile);

        if (country.deleted) {

            ++deleted;

        } else {

            index_country.id = country.id;
            index_country.pos = ftell(data_infile) - (deleted + sizeof(country));

            fwrite(&country, sizeof(country), 1, temp_data_outfile);
            fwrite(&index_country, sizeof(index_country), 1, index_outfile);

        }

    }

    fclose(data_infile);
    fclose(temp_data_outfile);
    fclose(index_outfile);

    remove("../files/country.fl");
    rename("../files/temp_country.fl", "../files/country.fl");

}

void update_cities() {

    FILE* data_infile = fopen("../files/city.fl", "r");
    FILE* temp_data_outfile = fopen("../files/temp_city.fl", "w");
    FILE* index_outfile = fopen("../files/city.ind", "w");

    if (data_infile == NULL) {

        printf("[ ERROR ] failed to open the file 'city.fl' in functions::update_cities\n");
        exit(1);

    }

    long long end = get_eof(data_infile);
    index_city_t index_city;
    city_t city;
    int deleted = 0;

    while (ftell(data_infile) != end) {

        fread(&city, sizeof(city), 1, data_infile);

        if (city.deleted) {

            ++deleted;

        } else {

            index_city.id = city.id;
            index_city.pos = ftell(data_infile) - (deleted + sizeof(city));

            fwrite(&city, sizeof(city), 1, temp_data_outfile);
            fwrite(&index_city, sizeof(index_city), 1, index_outfile);

        }

    }

    fclose(data_infile);
    fclose(temp_data_outfile);
    fclose(index_outfile);

    remove("../files/city.fl");
    rename("../files/temp_city.fl", "../files/city.fl");

}

void update_files(global_data_t* global_data) {

    if (global_data->deleted_countries >= 3) {

        update_countries();
        global_data->deleted_countries = 0;

    }

    if (global_data->deleted_cities >= 3) {

        update_cities();
        global_data->deleted_cities = 0;

    }

}

country_t* get_master(unsigned long long id) {

    FILE* index_infile = fopen("../files/country.ind", "r");

    if (index_infile == NULL) {
        printf("failed to open the file | functions::get_master");
        exit(1);
    }

    index_country_t index_country;
    long long end = get_eof(index_infile);
    short exists = false;

    while (ftell(index_infile) != end) {

        fread(&index_country, sizeof(index_country), 1, index_infile);
        print_index_country(index_country, "fuck index");

        if (index_country.id == id) {
            exists = true;
            break;
        }

    }

    fclose(index_infile);

    if (!exists)
       return NULL;

    country_t* country = (country_t*)malloc(sizeof(country_t));
    FILE* data_infile = fopen("../files/country.fl", "r");
    fseek(data_infile, index_country.pos, SEEK_SET);
    fread(&country[0], sizeof(*country), 1, data_infile);
    fclose(data_infile);

    if (country->deleted) {
        free(country);
        return NULL;
    }

    return country;

}

city_t* get_slave(unsigned long long id) {

    FILE* index_infile = fopen("../files/city.ind", "r");

    if (index_infile == NULL) {
        printf("failed to open the file | functions::get_slave");
        exit(1);
    }

    index_city_t index_city;
    long long end = get_eof(index_infile);
    short exists = false;

    while (ftell(index_infile) != end) {

        fread(&index_city, sizeof(index_city), 1, index_infile);
        print_index_city(index_city);

        if (index_city.id == id) {
            exists = true;
            break;
        }

    }

    fclose(index_infile);

    if (!exists)
        return NULL;

    city_t* city = (city_t*)malloc(sizeof(city_t));
    FILE* data_infile = fopen("../files/city.fl", "r");
    fseek(data_infile, index_city.pos, SEEK_SET);
    fread(&city[0], sizeof(*city), 1, data_infile);
    fclose(data_infile);

    if (city->deleted) {
        free(city);
        return NULL;
    }

    return city;

}

void delete_master(unsigned long long id) {

    global_data_t global_data = get_global_data();
    country_t* country = get_master(id);

    country->deleted = true;
    --global_data.countries_count;
    ++global_data.deleted_countries;

    FILE* data_infile = fopen("../files/city.fl", "r");
    city_t* cities = (city_t*)malloc(sizeof(city_t) * country->cities_count);
    int current = 0;
    long long offset = country->city_pos;

    for (int i = 0; i < country->cities_count; ++i) {

        fseek(data_infile, offset, SEEK_SET);
        fread(&cities[current], sizeof(cities[current]), 1, data_infile);

        offset = cities[current].next;
        cities[current].deleted = true;
        --global_data.cities_count;
        ++global_data.deleted_cities;

        printf("current city = ");
        print_city(cities[current]);

        ++current;

    }

    fclose(data_infile);

    update_master(*country);
    update_slaves(cities, country->cities_count);

    free(country);
    free(cities);
    put_global_data(global_data);

}

void delete_slave(unsigned long long id) {

    global_data_t global_data = get_global_data();
    city_t* city = get_slave(id);
    city_t prev_city;
    city_t current_city;
    country_t* country = NULL;

    if (city == NULL) {
        printf("[ ERROR ] no city exist with key %lld. please, try again\n", id);
        return;
    }

    country = get_master(city->country_id);

    city->deleted = true;
    --country->cities_count;
    --global_data.cities_count;
    ++global_data.deleted_cities;

    FILE* data_infile = fopen("../files/city.fl", "r");
    int offset = country->city_pos;

    if (data_infile == NULL) {
        printf("failed to open the file | functions::delete_slave");
        exit(1);
    }

    fseek(data_infile, country->city_pos, SEEK_SET);
    fread(&current_city, sizeof(current_city), 1, data_infile);

    int i = 0;
    while (i < country->cities_count + 1) {
        prev_city = current_city;
        fseek(data_infile, offset, SEEK_SET);
        fread(&current_city, sizeof(current_city), 1, data_infile);

        if (i == 0 && current_city.id == city->id) {
            country->city_pos = city->next;
            break;
        } else if (current_city.id == city->id) {
            prev_city.next = current_city.next;
            break;
        }

        offset = current_city.next;
        ++i;
    }

    fclose(data_infile);

    update_master(*country);
    update_slave(*city);

    if (i != 0)
        update_slave(prev_city);

    put_global_data(global_data);
    free(city);

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

void update_slave(const city_t city) {

    FILE* data_infile = fopen("../files/city.fl", "r");
    FILE* temp_outfile = fopen("../files/temp_city.fl", "w");

    long long end = get_eof(data_infile);
    city_t temp_city;

    while (ftell(data_infile) != end) {

        fread(&temp_city, sizeof(temp_city), 1, data_infile);

        if (temp_city.id == city.id)
            fwrite(&city, sizeof(city), 1, temp_outfile);
        else
            fwrite(&temp_city, sizeof(temp_city), 1, temp_outfile);

    }

    fclose(data_infile);
    fclose(temp_outfile);

    remove("../files/city.fl");
    rename("../files/temp_city.fl", "../files/city.fl");

}

void update_slaves(const city_t* cities, size_t size) {

    FILE* data_infile = fopen("../files/city.fl", "r");
    FILE* temp_outfile = fopen("../files/temp_city.fl", "w");

    long long end = get_eof(data_infile);
    int current = size - 1;
    city_t temp_city;

    while (ftell(data_infile) != end) {

        fread(&temp_city, sizeof(temp_city), 1, data_infile);

        if (temp_city.id == cities[current].id) {

            fwrite(&cities[current], sizeof(cities[current]), 1, temp_outfile);
            --current;

        } else {

            fwrite(&temp_city, sizeof(temp_city), 1, temp_outfile);

        }

    }

    fclose(data_infile);
    fclose(temp_outfile);

    remove("../files/city.fl");
    rename("../files/temp_city.fl", "../files/city.fl");

}

bool insert_master(void) {

    global_data_t global_data = get_global_data();

    // open files
    FILE* data_outfile = fopen("../files/country.fl", "a");
    FILE* index_outfile = fopen("../files/country.ind", "a");

    if (data_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::insert_slave");
        return false;
    }

    if (index_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.ind' | functions::insert_slave");
        return false;
    }

    country_t country;
    index_country_t index_country;

    country.id = global_data.country_id++;
    printf("enter name of the country:\n");
    scanf("%s", country.name);
    printf("enter area of the country:\n");
    scanf("%d", &country.area);
    country.cities_count = 0;
    country.city_pos = -1;
    country.deleted = false;

    ++global_data.countries_count;

    index_country.id = country.id;
    index_country.pos = ftell(data_outfile);

    print_index_country(index_country, "new country in index file");
    print_country(country, "new country");

    fwrite(&country, sizeof(country), 1, data_outfile);
    fwrite(&index_country, sizeof(index_country), 1, index_outfile);

    fclose(data_outfile);
    fclose(index_outfile);
    put_global_data(global_data);

    return true;

}

bool insert_slave(void) {

    global_data_t global_data = get_global_data();

    FILE* data_outfile = fopen("../files/city.fl", "a");
    FILE* index_outfile = fopen("../files/city.ind", "a");

    if (data_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::insert_slave");
        return false;
    }

    if (index_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.ind' | functions::insert_slave");
        return false;
    }

    if (global_data.countries_count == 0) {
        printf("[ ERROR ] no countries. first add master!\n");
        return false;
    }

    city_t city;
    index_city_t index_city;

    // init city
    city.id = global_data.city_id++;
    printf("enter name of the city:\n");
    scanf("%s", city.name);
    printf("enter population of the city:\n");
    scanf("%d", &city.population);
    printf("choose id of the country:\n");
    print_countries();
    scanf("%lld", &city.country_id);

    ++global_data.cities_count;

    country_t* country = get_master(city.country_id);
    city.next = country->city_pos;
    city.deleted = false;

    fwrite(&city, sizeof(city), 1, data_outfile);

    // init index_city
    index_city.id = city.id;
    index_city.pos = ftell(data_outfile) - sizeof(city);
    fwrite(&index_city, sizeof(index_city), 1, index_outfile);

    // update country
    ++country->cities_count;
    country->city_pos = index_city.pos;
    update_master(*country);

    print_city(city);
    print_country(*country, "fuck");

    fclose(data_outfile);
    fclose(index_outfile);
    put_global_data(global_data);
    free(country);

    return true;
}