#include "user_interface.h"

#include <stdio.h>

unsigned long long ui_scan_id(const char* title) {
    unsigned long long id;

    printf("enter %s id:\n", title);
    scanf("%lld", &id);

    return id;
}

void print_commands() {
    printf("0  - insert master(country)\n");
    printf("1  - insert slave(city)\n");
    printf("2  - get master\n");
    printf("3  - get slave\n");
    printf("4  - get all masters\n");
    printf("5  - get all slaves\n");
    printf("6  - get all slaves by master\n");
    printf("7  - update master\n");
    printf("8  - update slave\n");
    printf("9  - delete master\n");
    printf("10 - delete slave\n");
    printf("11 - delete all masters\n");
    printf("12 - delete all slaves\n");
    printf("13 - exit\n");
}

bool ui_print_country(country_t* country) {
    if (country == NULL) {
        printf("[ MESSAGE ] no records found\n");
        return false;
    }

    print_country(*country, "found country");
    return true;
}

bool ui_print_city(city_t* city) {
    if (city == NULL) {
        printf("[ MESSAGE ] no records found\n");
        return false;
    }

    print_city(*city, "found city");
    return true;
}

//void ui_insert_country() {
//    char name[COUNTRY_NAME_LENGTH];
//    int area;
//
//    printf("enter country name:\n");
//    scanf("%s", name);
//    printf("enter country area:\n");
//    scanf("%d", &area);
//
//    insert_master(name, area);
//}
//
//void ui_insert_city() {
//    char name[CITY_NAME_LENGTH];
//    int population;
//
//    printf("enter city name:\n");
//    scanf("%s", name);
//    printf("enter city population:\n");
//    scanf("%d", &population);
//
//    insert_slave(name, population);
//}

void ui_update_country() {
    if (!print_existing_countries()) {
        printf("[ MESSAGE ] no countries in database. first add one\n");
        return;
    }

    unsigned long long id = ui_scan_id("country");
    country_t* country = get_master(id);

    if (country == NULL) {
        printf("[ MESSAGE ] no records found\n");
        return;
    }

    short field;

    printf("choose one field:\n");
    printf("0 - name\n");
    printf("1 - area\n");
    scanf("%hd", &field);

    switch (field) {
        case 0:
            printf("enter new name:\n");
            scanf("%s", country->name);
            break;
        case 1:
            printf("enter new area:\n");
            scanf("%d", &country->area);
            break;
        default:
            printf("[ MESSAGE ] you entered not existing field\n");
            break;
    }

    update_master(*country);
    free(country);
}

void ui_update_city() {
    if (!print_existing_cities()) {
        printf("[ MESSAGE ] no cities in database. first add one\n");
        return;
    }

    unsigned long long id = ui_scan_id("city");
    city_t* city = get_slave(id);

    if (city == NULL) {
        printf("[ MESSAGE ] no records found\n");
        return;
    }

    short field;

    printf("choose one field:\n");
    printf("0 - name\n");
    printf("1 - population\n");
    scanf("%hd", &field);

    switch (field) {
        case 0:
            printf("enter new name:\n");
            scanf("%s", city->name);
            break;
        case 1:
            printf("enter new population:\n");
            scanf("%d", &city->population);
            break;
        default:
            printf("[ MESSAGE ] you entered not existing field\n");
            break;
    }

    update_slave(*city);
    free(city);
}

void ui_delete_country() {
    if (!print_existing_countries()) {
        printf("[ MESSAGE ] no countries in database. first add one\n");
        return;
    }

    delete_master(ui_scan_id("country"));
}

void ui_delete_city() {
    if (!print_existing_cities()) {
        printf("[ MESSAGE ] no cities in database. first add one\n");
        return;
    }

    delete_slave(ui_scan_id("city"));
}

void ui_run() {
    init_global_data();

    bool run = true;
    short command = -1;

    while (run) {
        printf("enter the command (print commands -> -1):\n");
        scanf("%hd", &command);

        if (command == -1) {
            print_commands();
            scanf("%hd", &command);
        }

        switch (command) {
            case 0:
                insert_master(true, NULL, -1);
                break;
            case 1:
                insert_slave(true, NULL, -1);
                break;
            case 2:
                ui_print_country(get_master(ui_scan_id("country")));
                break;
            case 3:
                ui_print_city(get_slave(ui_scan_id("city")));
                break;
            case 4:
                print_countries();
                break;
            case 5:
                print_cities();
                break;
            case 6:
                get_slaves(ui_scan_id("country"));
                break;
            case 7:
                ui_update_country();
                break;
            case 8:
                ui_update_city();
                break;
            case 9:
                ui_delete_country();
                break;
            case 10:
                ui_delete_city();
                break;
            case 11:
                delete_masters();
                break;
            case 12:
                delete_slaves();
                break;
            case 13:
                run = false;
                break;
            default:
                printf("no such command. try again :(\n");
                break;
        }

        printf("----------------------------------------------------------------------------\n\n");
    }
}