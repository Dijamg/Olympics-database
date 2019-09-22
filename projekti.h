#ifndef PROJEKTI_H
#define PROJEKTI_H

typedef struct {
    char name[21];
    int gold, silver, bronze;
} Country;

typedef struct {
    Country *countries;
    int numOfCountries;
} Database;

int handleCommand(Database *data, char *command);
int addCountry(Database *data, char *name);
int addMedals(Database *data, char *name, int gold, int silver, int bronze);
int initDatabase(Database *data);
void printCountries(Database *data);
void freeAll(Database *data);
int saveMedals(Database *data, const char *filename);
int loadMedals(Database *data, const char *filename);
int compareMedals(const void* a, const void* b);



#endif /* PROJEKTI_H */

