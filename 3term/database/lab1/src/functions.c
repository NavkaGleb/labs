#include "functions.h"

#include <time.h>
#include <fort.h>

void scan_country(country_t* country) {
    printf("enter country name:\n");
    scanf("%s", country->name);
    printf("enter country area:\n");
    scanf("%d", &country->area);
}

void scan_city(city_t* city) {
    printf("enter city name:\n");
    scanf("%s", city->name);
    printf("enter city population:\n");
    scanf("%d", &city->population);
}

void print_country(const country_t country, const char* title) {
    printf("%s { id: %lld, name: %s, area: %d, cities_count: %d, city_pos: %lld, deleted: %d }\n",
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

void print_city(const city_t city, const char* title) {
    printf("%s: { id: %lld, name: %s, population: %d, country_id: %lld, next: %lld, deleted: %d }\n",
       title, city.id, city.name, city.population, city.country_id, city.next, city.deleted);
}

void print_index_city(const index_city_t index_city, const char* title) {
    printf("%s: { id: %lld, pos: %lld }\n", title, index_city.id, index_city.pos);
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

    if (end == 0) {
        printf("[ MESSAGE ] no countries :(\n");
        fclose(index_infile);
        fclose(data_infile);
        return;
    }

    index_country_t index_country;
    country_t country;
    ft_table_t* table = ft_create_table();
    char pos_buffer[10];
    char id_buffer[10];
    char area_buffer[10];
    char cities_count_buffer[10];
    char city_pos_buffer[10];
    char deleted_buffer[2];

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "pos", "id", "name", "area", "cities_count", "city_pos", "deleted");

    while(ftell(index_infile) != end) {
        fread(&index_country, sizeof(index_country), 1, index_infile);
        fread(&country, sizeof(country), 1, data_infile);

        itoa(index_country.pos, pos_buffer, 10);
        itoa(country.id, id_buffer, 10);
        itoa(country.area, area_buffer, 10);
        itoa(country.cities_count, cities_count_buffer, 10);
        itoa(country.city_pos, city_pos_buffer, 10);
        itoa(country.deleted, deleted_buffer, 10);

        ft_write_ln(table, pos_buffer, id_buffer, country.name, area_buffer, cities_count_buffer, city_pos_buffer, deleted_buffer);
    }

    printf("%s\n", ft_to_string(table));

    ft_destroy_table(table);
    fclose(index_infile);
    fclose(data_infile);

}

bool print_existing_countries(void) {
    FILE* data_infile = fopen("../files/country.fl", "r");

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'country.fl' | functions::print_countries");
        exit(1);
    }

    long long end = get_eof(data_infile);

    if (end == 0) {
        printf("[ MESSAGE ] no countries :(\n");
        fclose(data_infile);
        return false;
    }

    country_t country;
    ft_table_t* table = ft_create_table();
    char id_buffer[10];
    char area_buffer[10];
    char cities_count_buffer[10];

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "id", "name", "area", "cities_count");

    while(ftell(data_infile) != end) {
        fread(&country, sizeof(country), 1, data_infile);

        if (!country.deleted) {
            itoa(country.id, id_buffer, 10);
            itoa(country.area, area_buffer, 10);
            itoa(country.cities_count, cities_count_buffer, 10);

            ft_write_ln(table, id_buffer, country.name, area_buffer, cities_count_buffer);
        }
    }

    printf("%s", ft_to_string(table));

    ft_destroy_table(table);
    fclose(data_infile);

    return true;
}

void print_cities() {
    FILE* index_infile = fopen("../files/city.ind", "r");
    FILE* data_infile = fopen("../files/city.fl", "r");

    if (index_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.ind' | functions::print_countries");
        exit(1);
    }

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::print_countries");
        exit(1);
    }

    long long end = get_eof(index_infile);

    if (end == 0) {
        printf("[ MESSAGE ] no cities :(\n");
        fclose(index_infile);
        fclose(data_infile);
        return;
    }

    index_city_t index_city;
    city_t city;
    ft_table_t* table = ft_create_table();
    char pos_buffer[10];
    char id_buffer[10];
    char population_buffer[10];
    char country_id_buffer[10];
    char next_buffer[10];
    char deleted_buffer[2];

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "pos", "id", "name", "population", "country_id", "next", "deleted");

    while (ftell(index_infile) != end) {
        fread(&index_city, sizeof(index_city), 1, index_infile);
        fread(&city, sizeof(city), 1, data_infile);

        itoa(index_city.pos, pos_buffer, 10);
        itoa(city.id, id_buffer, 10);
        itoa(city.population, population_buffer, 10);
        itoa(city.country_id, country_id_buffer, 10);
        itoa(city.next, next_buffer, 10);
        itoa(city.deleted, deleted_buffer, 10);

        ft_write_ln(table, pos_buffer, id_buffer, city.name, population_buffer, country_id_buffer, next_buffer, deleted_buffer);
    }

    printf("%s\n", ft_to_string(table));

    fclose(data_infile);
    ft_destroy_table(table);
}

bool print_existing_cities(void) {
    FILE* data_infile = fopen("../files/city.fl", "r");

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::print_countries");
        exit(1);
    }

    long long end = get_eof(data_infile);

    if (end == 0) {
        printf("[ MESSAGE ] no cities :(\n");
        fclose(data_infile);
        return false;
    }

    city_t city;
    ft_table_t* table = ft_create_table();
    char id_buffer[10];
    char population_buffer[10];
    char country_id_buffer[10];

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "id", "name", "population", "country_id");

    while(ftell(data_infile) != end) {
        fread(&city, sizeof(city), 1, data_infile);

        if (!city.deleted) {
            itoa(city.id, id_buffer, 10);
            itoa(city.population, population_buffer, 10);
            itoa(city.country_id, country_id_buffer, 10);

            ft_write_ln(table, id_buffer, city.name, population_buffer, country_id_buffer);
        }
    }

    printf("%s", ft_to_string(table));

    ft_destroy_table(table);
    fclose(data_infile);

    return true;
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

void insert_master(bool input, char name[COUNTRY_NAME_LENGTH], int area) {
    global_data_t global_data = get_global_data();

    // open files
    FILE* data_outfile = fopen("../files/country.fl", "a");
    FILE* index_outfile = fopen("../files/country.ind", "a");

    if (data_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::insert_slave");
        exit(1);
    }

    if (index_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.ind' | functions::insert_slave");
        exit(1);
    }

    country_t country;
    index_country_t index_country;

    country.id = global_data.country_id++;

    if (!input) {
        strcpy(country.name, name);
        country.area = area;
    } else {
        scan_country(&country);
    }

    country.cities_count = 0;
    country.city_pos = -1;
    country.deleted = false;
    ++global_data.countries_count;

    fwrite(&country, sizeof(country), 1, data_outfile);

    // init index_country
    index_country.id = country.id;
    index_country.pos = ftell(data_outfile) - sizeof(country);
    fwrite(&index_country, sizeof(index_country), 1, index_outfile);

    print_index_country(index_country, "new country in index file");
    print_country(country, "new country");

    fclose(data_outfile);
    fclose(index_outfile);
    put_global_data(global_data);
}

bool insert_slave(bool entry, char name[CITY_NAME_LENGTH], int population) {
    global_data_t global_data = get_global_data();

    // open files
    FILE* data_outfile = fopen("../files/city.fl", "a");
    FILE* index_outfile = fopen("../files/city.ind", "a");

    if (data_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::insert_slave");
        exit(1);
    }

    if (index_outfile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.ind' | functions::insert_slave");
        exit(1);
    }

    if (global_data.countries_count == 0) {
        printf("[ ERROR ] no countries. first add master!\n");
        return false;
    }

    city_t city;
    index_city_t index_city;

    // init city
    city.id = global_data.city_id++;

    if (!entry) {
        strcpy(city.name, name);
        city.population = population;
    } else {
        scan_city(&city);
    }

    printf("choose id of the country:\n");
    print_existing_countries();
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

    print_index_city(index_city, "new city in index file");
    print_city(city, "new city");

    fclose(data_outfile);
    fclose(index_outfile);
    put_global_data(global_data);
    free(country);

    return true;
}

country_t* get_master(unsigned long long id) {

    FILE* index_infile = fopen("../files/country.ind", "r");

    if (index_infile == NULL) {
        printf("[ ERROR ] failed to open the file | functions::get_master");
        exit(1);
    }

    long long end = get_eof(index_infile);
    index_country_t index_country;
    short exists = false;

    while (ftell(index_infile) != end) {
        fread(&index_country, sizeof(index_country), 1, index_infile);

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

void get_masters(country_t* countries, size_t size) {
    FILE* data_infile = fopen("../files/country.fl", "r");

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'country.fl' | functions::get_masters\n");
        exit(1);
    }

    fread(countries, sizeof(*countries), size, data_infile);
    fclose(data_infile);
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

void get_slaves(unsigned long long id) {
    country_t* country = get_master(id);

    if (country == NULL) {
        printf("[ MESSAGE ] no country by %lld\n", id);
        return;
    }

    FILE* data_infile = fopen("../files/city.fl", "r");

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::get_slaves\n");
        exit(1);
    }

    city_t city;
    long long offset = country->city_pos;
    ft_table_t* table = ft_create_table();
    char id_buffer[10];
    char population_buffer[10];
    char country_id_buffer[10];
    char next_buffer[10];
    char deleted_buffer[2];

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "id", "name", "population", "country_id", "next", "deleted");

    printf("END = %lld\n", get_eof(data_infile));

    for (int i = 0; i < country->cities_count; ++i) {
        printf("offset = %lld\n", offset);
        fseek(data_infile, offset, SEEK_SET);
        printf("current pos = %ld", ftell(data_infile));
        fread(&city, sizeof(city), 1, data_infile);
        offset = city.next;

        print_city(city, "FUCKING CURRENT CITY");

        itoa(city.id, id_buffer, 10);
        itoa(city.population, population_buffer, 10);
        itoa(city.country_id, country_id_buffer, 10);
        itoa(city.next, next_buffer, 10);
        itoa(city.deleted, deleted_buffer, 10);

        ft_write_ln(table, id_buffer, city.name, population_buffer, country_id_buffer, next_buffer, deleted_buffer);
    }

    print_country(*country, "found country");
    printf("%s\n", ft_to_string(table));

    free(country);
    fclose(data_infile);
    ft_destroy_table(table);

}

void update_countries(global_data_t* global_data) {

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

    global_data->deleted_countries = 0;
}

void update_cities(global_data_t* global_data) {
    FILE* data_infile = fopen("../files/city.fl", "r");
    FILE* temp_data_outfile = fopen("../files/temp_city.fl", "w");
    FILE* index_outfile = fopen("../files/city.ind", "w");

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'city.fl' | functions::update_cities\n");
        exit(1);
    }

    long long end = get_eof(data_infile);
    index_city_t index_city;
    city_t city;
    country_t* countries = (country_t*)malloc(sizeof(country_t) * global_data->countries_count);
    int* ids = (int*)malloc(sizeof(int) * global_data->countries_count);
    int deleted = 0;

    get_masters(countries, global_data->countries_count);

    for (int i = 0; i < global_data->countries_count; ++i) {
        fseek(data_infile, countries[i].city_pos, SEEK_SET);
        fread(&city, sizeof(city), 1, data_infile);
        ids[i] = city.id;
    }

    fseek(data_infile, 0, SEEK_SET);

    while (ftell(data_infile) != end) {
        fread(&city, sizeof(city), 1, data_infile);

        if (city.deleted) {
            ++deleted;
        } else {
            index_city.id = city.id;
            index_city.pos = ftell(data_infile) - ((deleted + 1) * sizeof(city));

            print_city(city, "CURRENT FUCKING CITY");

            printf("NOT DELETED CITY\n");
            printf("deleted = %d\n", deleted);
            printf("current pos of city %lld -> %lld\n", index_city.id, index_city.pos);

//            if (city.next != -1)
//                city.next -= (deleted * sizeof(city));

            print_city(city, "CITY AFTER UPDATE!!!!");

            fwrite(&city, sizeof(city), 1, temp_data_outfile);
            fwrite(&index_city, sizeof(index_city), 1, index_outfile);
        }
    }

    fclose(data_infile);
    fclose(temp_data_outfile);
    fclose(index_outfile);

    FILE* index_infile = fopen("../files/city.ind", "r");
    end = get_eof(index_infile);

    for (int i = 0; i < global_data->countries_count; ++i) {
        while (ftell(index_infile) != end) {
            fread(&index_city, sizeof(index_city), 1, index_infile);

            if (index_city.id == ids[i]) {
                countries[i].city_pos = index_city.pos;
                break;
            }
        }
    }

    fclose(index_infile);

    remove("../files/city.fl");
    rename("../files/temp_city.fl", "../files/city.fl");

    update_masters(countries, global_data->countries_count);
    global_data->deleted_cities = 0;
}

void update_files(global_data_t* global_data) {

    if (global_data->deleted_countries >= 3)
        update_countries(global_data);

    if (global_data->deleted_cities >= 3)
        update_cities(global_data);

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

void update_masters(country_t* countries, size_t size) {
    FILE* data_infile = fopen("../files/country.fl", "w");

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'country.fl' | functions::get_masters\n");
        exit(1);
    }

    fwrite(countries, sizeof(country_t), size, data_infile);
    fclose(data_infile);
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
        ++current;
    }

    fclose(data_infile);

    update_master(*country);
    update_slaves(cities, country->cities_count);

    print_country(*country, "deleted country");
    printf("[ MESSAGE ] country was successfully deleted\n");
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
            printf("FUCK THIS SHIT\n");
            printf("city id %lld\n", current_city.id);
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

    print_city(*city, "deleted city");
    printf("[ MESSAGE ] city was successfully deleted\n");
    free(city);

    put_global_data(global_data);
}

void delete_masters() {
    // clear global data
    FILE* global_data_outfile = fopen("../files/global_data.dat", "w");
    global_data_t global_data = { 0, 0, 0, 0, 0, 0 };
    fwrite(&global_data, sizeof(global_data), 1, global_data_outfile);
    fclose(global_data_outfile);

    // clear masters
    fclose(fopen("../files/country.ind", "w"));
    fclose(fopen("../files/country.fl", "w"));

    // clear slaves
    fclose(fopen("../files/city.ind", "w"));
    fclose(fopen("../files/city.fl", "w"));

    printf("[ MESSAGE ] all masters was successfully deleted\n");
}

void delete_slaves() {
    // delete slaves
    fclose(fopen("../files/city.ind", "w"));
    fclose(fopen("../files/city.fl", "w"));

    // update masters
    FILE* data_infile = fopen("../files/country.fl", "r");
    FILE* temp_outfile = fopen("../files/temp_country.fl", "w");

    if (data_infile == NULL) {
        printf("[ ERROR ] failed to open the file 'country.fl' | functions::get_master");
        exit(1);
    }

    long long end = get_eof(data_infile);
    country_t country;

    while (ftell(data_infile) != end) {
        fread(&country, sizeof(country), 1, data_infile);

        country.cities_count = 0;
        country.city_pos = -1;

        fwrite(&country, sizeof(country), 1, temp_outfile);
    }

    fclose(data_infile);
    fclose(temp_outfile);

    remove("../files/country.fl");
    rename("../files/temp_country.fl", "../files/country.fl");

    printf("[ MESSAGE ] all slaves was successfully deleted\n");
}