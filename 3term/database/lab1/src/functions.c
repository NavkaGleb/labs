#include "functions.h"

#include <stdlib.h>
#include <time.h>

#include "person.h"

void rand_string(char* string, size_t length) {

    static char alphabet[] = { 'a', 'b', 'c', 'd', 'e' };

    for (int i = 0; i < length; ++i)
        string[i] = alphabet[rand() % (sizeof(alphabet) / sizeof(alphabet[0]))];

    string[length] = '\0';

}

void rand_person(person_t* p) {

    p->passport = rand() % 100;
    rand_string(p->name, rand() % (sizeof(p->name) / sizeof(p->name[0]) - 1 - 1) + 1);
    p->sex = male;

}

long long get_eof(FILE* file) {

    long long end;

    fseek(file, 0, SEEK_END);
    end = ftell(file);
    fseek(file, 0, SEEK_SET);

    return end;

}

void get_master(void) {

    FILE* infile = fopen("../files/person.fl", "r");
    FILE* index_infile = fopen("../files/person.ind", "r");

    if (infile == NULL || index_infile == NULL)
        return;

    printf("read from file!\n");

    long long passport;
    int pos;
    person_t person;
    long long end = get_eof(infile);

    while (ftell(infile) != end) {

        fread(&passport, sizeof(passport), 1, index_infile);
        fread(&pos, sizeof(pos), 1, index_infile);
        fread(&person, sizeof(person), 1, infile);

        printf("passport = %lld\n", passport);
        printf("pos = %d\n", pos);

        printf("passport = %lld\n", person.passport);
        printf("name = %s\n", person.name);
        printf("sex = %d\n", person.sex);

        printf("current pos == %ld\n\n", ftell(infile));

    }

    fclose(infile);
    fclose(index_infile);

}

void insert_master(void) {

    // init rand
    srand(time(NULL));

    // simple file
    FILE* outfile = fopen("../files/person.fl", "a");
    FILE* index_outfile = fopen("../files/person.ind", "a");

    if (outfile == NULL || index_outfile == NULL)
        return;

    int current_pos = -1;
    person_t person;

    rand_person(&person);
    current_pos = ftell(outfile);

    printf("passport = %lld\n", person.passport);
    printf("name = %s\n", person.name);
    printf("sex = %d\n", person.sex);
    printf("sizeof person %d\n", sizeof(person));
    printf("current_pos = %d\n\n", current_pos);

    fwrite(&person, sizeof(person), 1, outfile);
    fwrite(&person.passport, sizeof(person.passport), 1, index_outfile);
    fwrite(&current_pos, sizeof(current_pos), 1, index_outfile);

    fclose(outfile);
    fclose(index_outfile);

}