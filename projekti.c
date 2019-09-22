#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "projekti.h"

//This function handles users commands.
int handleCommand(Database *data, char *command){
    char com;                       //Used to store the command letter.
    char name[20];                  //Used to store the parameter after the command letter.
    char filename[20];              //Used to store the name of file.
    int gold, silver, bronze;       //Used to store the number of medals in the parameters.
    int res;                        //Used to store sscanf return value.

    switch(command[0]) {
	case 'A':
                res = sscanf(command,"%c %s", &com, name );
                if(res != 2){
                    printf("invalid parameters. Give command in format 'A countryname'\n\n");
                    return 1;
                }
                if(strlen(name) > 20){
                    printf("Name of country can at maximum be 20 letters long.\n\n");
                    return 1;
                }
                res = addCountry(data, name);
                if(res == 1){
                    printf("Country %s added succesfully!\n\n", name);
                }
		break;
	case 'M':
                res = sscanf(command,"%c %s %d %d %d", &com, name, &gold, &silver, &bronze );
                if(res != 5){
                printf("invalid parameters. Give command in format 'M countryname golds silvers bronzes'\n\n");
                return 1;
            }
		res = addMedals(data, name, gold, silver, bronze);
                if(res == 1){
                    printf("Medals of %s has been updated succesfully!\n\n", name);
                }
		break;
	case 'L':
                printCountries(data);
		break;
	case 'W':
                res = sscanf(command,"%c %s", &com, filename);
                if(res != 2){
                    printf("invalid parameters. Give command in format 'W filename'\n\n");
                    return 1;
                }
                res = saveMedals(data, filename);
                if(res == 1){
                    printf("Medals have been saved succesfully!\n\n");
                }
		break;
        case 'O':
		res = sscanf(command,"%c %s", &com, filename);
                if(res != 2){
                    printf("invalid parameters. Give command in format 'O filename'\n\n");
                    return 1;
                }
                res = loadMedals(data, filename);
                if(res == 1){
                    printf("Medals have been loaded succesfully!\n\n");
                }
		break;
        case 'Q':
                return 0; 
                
	default:
		printf("Invalid command.\n");
		break;
	}
    return 1;
}

//Used for sorting the countries by medals.
int compareMedals(const void* a, const void* b){
    const Country *countryA = a;
    const Country *countryB = b;
    if(countryA->gold == countryB->gold){
        if(countryA->silver == countryB->silver){
            if(countryA->bronze > countryB->bronze){
                return -1;
            }else{
                return 1;
            }
        }else if(countryA->silver > countryB->silver){
            return -1;
        }else{
            return 1;
        }
    }else if(countryA->gold > countryB->gold){
        return -1;
    }else{
        return 1;
    }
}


void freeMemory(Database *data){
    if(data->countries){
        free(data->countries);
    }
    data->numOfCountries = 0;
}

//Reallocates memory and adds a new country.
int addCountry(Database *data, char *name){
    if(data->numOfCountries != 0){
        data->countries = realloc(data->countries, (data->numOfCountries +1)*sizeof(Country));
        if(data == NULL){
            printf("Memory allocation for addCountry failed\n\n");
            freeMemory(data);
	    exit(1);
        }
    }
    Country *current = data->countries + data->numOfCountries;
    strncpy(current->name, name, 20);
    current->gold = 0;
    current->silver = 0;
    current->bronze = 0;
    
    data->numOfCountries++;
    return 1;
}

//Updates the medals of an existing country.
int addMedals(Database *data, char *name, int gold, int silver, int bronze){
    Country *current = data->countries;
    int i = 0;
    while((i < data->numOfCountries) && (strcmp(current->name, name)!= 0)){
        current++;
        i++;
    }
    //If whole array has been checked and the wanted name didnt occur.
    if(i == data->numOfCountries){
        printf("Country not found. Country needs to be added with A command.\n\n");
        return 0;
    }
    current->gold += gold;
    current->silver += silver;
    current->bronze += bronze;
    return 1;
}

//Saves the current database in memory to a file.
int saveMedals(Database *data, const char *filename){
    FILE *f = fopen(filename,"w");
    if(!f){
        printf("Failed to open file.");
        return 0;
    }
    if(data->numOfCountries == 0){
        printf("There are currently no countries to save");
        return 0;
    }else{
        Country *current = data->countries;
        for(int i = 0; i < data->numOfCountries; i++){
            fprintf(f,"%s %d %d %d\n", current->name, current->gold, current->silver, current->bronze);
            current++;
        }
        fclose(f);
        return 1;
    }
}

//FIX ORDER 
void printCountries(Database *data){
    if(data->numOfCountries == 0){
        printf("There are currently no countries to print.\n\n");
    }else{
        qsort(data->countries, data->numOfCountries, sizeof(Country), compareMedals);
        Country *current = data->countries;
        printf("List of countries:\n");
        for(int i = 0; i < data->numOfCountries; i++){
            printf("%s %d %d %d\n", current->name, current->gold, current->silver, current->bronze);
            current++;
        }
        printf("\n");
    }
}

//Allocates memory for the array which stores the countries. Returns 0 if failed, 1 else.
int initDatabase(Database *data){
    //First initialize memory for one country.
    data->countries = malloc(sizeof(Country));
    if(data->countries == NULL){
        printf("%s\n", "Failed to allocate memory");
        return 0;
    }
    memset(data->countries, 0, sizeof(Country));
    data->numOfCountries = 0;
    return 1;
}

// Loads and existing table from file. Assumes that it is in right format.
int loadMedals(Database *data, const char *filename){
    FILE *f = fopen(filename,"r");
    char name[21];
    int gold, silver, bronze;
    if(!f){
        printf("Failed to open file.\n\n");
        return 0;
    }
    //Resets existing memory.
    freeMemory(data);
    initDatabase(data);
    
    //Read and add country.
    while(fscanf(f,"%s %d %d %d",name, &gold, &silver, &bronze) != EOF){
            addCountry(data, name);
            addMedals(data, name, gold, silver, bronze);
    }
    fclose(f);
    return 1;
}



int main()
{
    Database data;
    memset(&data, 0, sizeof(data));
    int running = initDatabase(&data);
    //Used to check if fgets returns EOF.
    char *c;
    while(running){
        char buffer[80];
        printf("command >");
        c = fgets(buffer, sizeof(buffer), stdin);
	if(c == NULL){
	    running = 0;
	}else{
            running = handleCommand(&data, buffer);
	}
    }
    printf("Freeing memory and exiting program.");
    freeMemory(&data);
    return 0;
}
