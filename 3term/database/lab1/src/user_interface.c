#include "user_interface.h"

#include <stdio.h>

#include "functions.h"

void run() {
    init_global_data();

    bool run = true;
    short command = -1;

    while (run) {
        printf("chose the command:\n");
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
        printf("13 - delete slaves by master\n");
        printf("14 - exit\n");
        scanf("%hd", &command);

        switch (command) {
            case 0:
                insert_master();
                break;
            case 1:
                insert_slave();
                break;
            case 4:
                print_countries();
                break;
            case 14:
                run = false;
                break;
            default:
                printf("no such command. try again :(\n");
                break;
        }

        printf("----------------------------------------------------------------------------\n");
    }
}
