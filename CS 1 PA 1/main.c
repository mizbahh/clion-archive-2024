/* COP 3502C Assignment 1
This program is written by: Mizbah Ateeq */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30

typedef struct RegisteredVehicle { //for one registered vehicle
    char* license_plate; //to be used for string license plate# of the vehicle
    char* owner_name; //to be used for storing owner name
} RegisteredVehicle;

typedef struct Garage { //for one garage
    char* garage_name; //to be used for garage name
    int total_capacity; //total capacity of the garage
    int current_count; // number of vehicles in the garage in a particular time
    RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time
} Garage;

typedef struct Campus {
    Garage** garages; //list of garages in campus
    int total_garages; //number of garages in the campus
    RegisteredVehicle** registered_vehicles; //list of registered vehicles
    int total_registered_vehicles; //total number of registered vehicles
} Campus;

// create new garage
Garage* createGarage(const char* name, int capacity) {
    Garage *newGarage = (Garage*)malloc(sizeof(Garage));
    // allocate mem + copy garage name
    newGarage->garage_name = (char*)malloc(sizeof(char)*strlen(name) + 1);
    strcpy(newGarage->garage_name, name);

    // garage total capacity
    newGarage->total_capacity = capacity;
    newGarage->current_count = 0;

    RegisteredVehicle **vehicles = (RegisteredVehicle**)malloc(capacity*sizeof(RegisteredVehicle*));

    // allocate mem for parked vehicles
    newGarage->parked_vehicles = vehicles;

    if (!newGarage)
        return NULL;

    return newGarage;
}

// register new vehicle
RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner) {
    RegisteredVehicle *newVehicle = (RegisteredVehicle*)malloc(sizeof(RegisteredVehicle));

    // store license plate + owner name
    newVehicle->license_plate = (char*)malloc(sizeof(char)*strlen(license) + 1);
    strcpy(newVehicle->license_plate, license);
    newVehicle->owner_name = (char*)malloc(sizeof(char)*strlen(owner) + 1);
    strcpy(newVehicle->owner_name, owner);

    if (!newVehicle)
        return NULL;

    return newVehicle;
}

// register new vehicle in campus
void registerVehicle(Campus* campus, const char* license, const char* owner) {


    // resize array to accomodate new car
    campus->registered_vehicles = realloc(campus->registered_vehicles,(campus->total_registered_vehicles + 1) * sizeof(RegisteredVehicle*));

    // send details to createRegisteredVehicle function
    campus->registered_vehicles[campus->total_registered_vehicles] = createRegisteredVehicle(license, owner);
    campus->total_registered_vehicles += 1; // adjust total

    printf("REGISTERED\n");

}

// park car in garage
void parkVehicle(Garage* garage, RegisteredVehicle* vehicle) {

    // check if full or space in garage to park
    if(garage->current_count == garage->total_capacity)
        printf("FULL\n");
    else {
        garage->parked_vehicles[garage->current_count] = vehicle;
        garage->current_count++;
        printf("PARKED\n");
    }
}

// remove car from garage
int removeVehicleFromGarage(Garage* garage, const char* license) {

    for(int i = 0; i < garage->current_count; i++) {
        if(strcmp(garage->parked_vehicles[i]->license_plate, license) == 0) {
            // shift remaining cars
            for (int j = i; j < garage->current_count - 1; j++) {
                garage->parked_vehicles[j] = garage->parked_vehicles[j + 1];
            }

            // adjust parked vehicle amount (subtract 1)
            garage->current_count--;
            return 1;
        }
    }
    return 0;
}

// search for car by license plate
RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license) {
    RegisteredVehicle *vehicle;
    for (int i = 0; i < campus->total_garages; i++) {
        for (int j = 0; j < campus->garages[i]->current_count; j++) {
            if (strcmp(license, campus->garages[i]->parked_vehicles[j]->license_plate) == 0) {
                return campus->garages[i]->parked_vehicles[j];
            }
        }
    }
    return NULL;
}

// calculate total num of cars  in garages
int countTotalVehicles(const Campus* campus) {
    int totalNum = 0;
    for(int i = 0; i < campus->total_garages; i++)
        totalNum += campus->garages[i]->current_count;
    return totalNum;
}

// resize garage capacity
int resizeGarage(Garage* garage, int new_capacity) {
    if (new_capacity >= garage->current_count) {
        // update capacity
        garage->total_capacity = new_capacity;
        garage->parked_vehicles = (RegisteredVehicle**)realloc(garage->parked_vehicles, (new_capacity)*sizeof(RegisteredVehicle*));
        return 1;
    } else {
        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
        return 0;
    }
}

int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name) {
    RegisteredVehicle *vehicle;
    Garage *initialGarage = NULL, *finalGarage = NULL;
    int relocate = 0, findGarage = 0;

    // serach final garage
    for (int i = 0; i < campus->total_garages; i++) {
        if (strcmp(target_garage_name, campus->garages[i]->garage_name) == 0) {
            finalGarage = campus->garages[i];
            findGarage = 1;

            // check if space
            if (finalGarage->current_count < finalGarage->total_capacity) {

                // find license plate
                vehicle = searchVehicleByLicense(campus, license);
                if (vehicle == NULL) {
                    printf("%s NOT IN CAMPUS.\n", license);
                    return 0;
                }

                // find initial garage
                for (int j = 0; j < campus->total_garages; j++) {
                    for (int k = 0; k < campus->garages[j]->current_count; k++) {
                        if (strcmp(license, campus->garages[j]->parked_vehicles[k]->license_plate) == 0) {
                            initialGarage = campus->garages[j];
                            break;
                        }
                    }
                    if (initialGarage != NULL) break;
                }

                if (initialGarage == NULL) {
                    printf("%s NOT FOUND IN ANY GARAGE.\n", license);
                    return 0;
                }

                // remove vehicle from initial garage and park in final
                relocate = removeVehicleFromGarage(initialGarage, vehicle->license_plate);
                parkVehicle(finalGarage, vehicle);

                return relocate;
            } else {
                // if garage  full
                printf("%s IS FULL.\n", target_garage_name);
            }
        }
    }

    // if garage not found
    if (findGarage == 0) {
        printf("%s NOT FOUND.\n", target_garage_name);
    }

    return 0;
}

// identify car owner using license plate
void displayVehiclesByOwner(const Campus* campus, const char* owner_name) {
    Garage *garage = NULL;
    char licensePL[MAX];
    int findCar = 0;

    for (int i = 0; i < campus->total_registered_vehicles; i++) {
        if (strcmp(owner_name, campus->registered_vehicles[i]->owner_name) == 0) {
            findCar = 1;
            strcpy(licensePL, campus->registered_vehicles[i]->license_plate);

            // find garage where car is parked
            for (int j = 0; j < campus->total_garages; j++) {
                for (int k = 0; k < campus->garages[j]->current_count; k++) {
                    if (strcmp(licensePL, campus->garages[j]->parked_vehicles[k]->license_plate) == 0) {
                        garage = campus->garages[j];
                        break;
                    }
                }
                if (garage != NULL) break;
            }

            if (garage == NULL) {
                printf("%s NOT ON CAMPUS\n", licensePL);
            } else {
                // output license plate + garage name if found
                printf("%s %s\n", licensePL, garage->garage_name);
            }

            // reset garage for next one
            garage = NULL;
        }
    }

    // if no cars with owner name found
    if (findCar == 0) {
        printf("NO REGISTERED CAR BY THIS OWNER\n");
    }
}


// remove garage from campus
int removeGarage(Campus* campus, const char* garage_name) {
    Garage *garageToRemove;
    // search for garage
    for (int i = 0; i < campus->total_garages; i++) {
        if (strcmp(garage_name, campus->garages[i]->garage_name) == 0) {
            garageToRemove = campus->garages[i];
            if (i < campus->total_garages - 1) {
                for (int j = i; j < campus->total_garages - 1; j++)
                    campus->garages[j] = campus->garages[j + 1];
                // adjust garage num with removal
                campus->total_garages = (campus->total_garages) - 1;
                return 1;
            }
        }
    }
    return 0;
}

// utilization report of garages
void generateGarageUtilizationReport(const Campus* campus) {
    double percent = 0, leastPercent = 100;
    int leastIndex = 0;

    for (int i = 0; i < campus->total_garages; i++) {
        // get current occupied spots + total capacity
        float occupied = campus->garages[i]->current_count;
        float totalCapacity = campus->garages[i]->total_capacity;
        // calculate utilization %
        percent = 100*(occupied/totalCapacity);
        if((int)percent < (int)leastPercent) {
            leastIndex = i;
            leastPercent = percent;
        }

        // output garage details
        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.2lf%%\n",
               campus->garages[i]->garage_name, campus->garages[i]->total_capacity,
               campus->garages[i]->current_count, percent);
    }
    // print least utilized
    printf("Least Utilized: %s\n", campus->garages[leastIndex]->garage_name);
}

// process commands
void commands(Campus* campus, int commandsNum) {
    char garageName[MAX], command[MAX], licensePL[MAX], owner[MAX];

    for (int i = 0; i < commandsNum; i++) {
        Garage *garage;
        scanf("%s", command);

        // PARK command
        if (strcmp(command, "PARK") == 0) {
            // initialize pointers
            RegisteredVehicle *vehicle = NULL;
            Garage *garage = NULL;
            scanf("%s %s", licensePL, garageName);

            // find car to park
            for (int j = 0; j < campus->total_registered_vehicles; j++) {
                if (strcmp(licensePL, campus->registered_vehicles[j]->license_plate) == 0) {
                    vehicle = campus->registered_vehicles[j];
                }
            }
            // find garage to park in
            for (int j = 0; j < campus->total_garages; j++) {
                if (strcmp(garageName, campus->garages[j]->garage_name) == 0) {
                    garage = campus->garages[j];
                }
            }
            parkVehicle(garage, vehicle);

        } // UTILIZATION_REPORT command
        else if (strcmp(command, "UTILIZATION_REPORT") == 0) {
            generateGarageUtilizationReport(campus);

        } // RESIZE command
        else if (strcmp(command, "RESIZE") == 0) {
            int capacityNew;
            scanf("%s %d", garageName, &capacityNew);

            // find garage
            for (int j = 0; j < campus->total_garages; j++) {
                if (strcmp(garageName, campus->garages[j]->garage_name) == 0) {
                    int passOrFail = resizeGarage(campus->garages[j], capacityNew);
                    if (passOrFail == 1)
                        printf("SUCCESS\n");
                }
            }

        } // SEARCH_OWNER command
        else if (strcmp(command, "SEARCH_OWNER") == 0) {
            scanf("%s", owner);
            displayVehiclesByOwner(campus, owner);

        } // RELOCATE command
        else if (strcmp(command, "RELOCATE") == 0) {
            int relocate;
            scanf("%s %s", licensePL, garageName);
            relocate = relocateVehicle(campus, licensePL, garageName);

            if (relocate == 1) {
                printf("RELOCATION SUCCESSFUL.\n");
            }

        } // COUNT_TOTAL command
        else if (strcmp(command, "COUNT_TOTAL") == 0) {
            int total = countTotalVehicles(campus);
            printf("%d\n", total);

        } // REGISTER_VEHICLE command
        else if (strcmp(command, "REGISTER_VEHICLE") == 0) {
            scanf("%s %s", licensePL, owner);
            registerVehicle(campus, licensePL, owner);

        } // REMOVE_VEHICLE_GARAGE command
        else if (strcmp(command, "REMOVE_VEHICLE_GARAGE") == 0) {
            int passOrFail = 0;
            scanf("%s", licensePL);

            // find  garage where vehicle  parked
            for (int i = 0; i < campus->total_garages; i++) {
                for (int j = 0; j < campus->garages[i]->current_count; j++) {
                    if (strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, licensePL) == 0) {
                        garage = campus->garages[i];
                        break;
                    }
                }
                if (garage != NULL) break;
            }

            if (garage != NULL) {
                passOrFail = removeVehicleFromGarage(garage, licensePL);
                if (passOrFail == 1)
                    printf("REMOVED FROM %s\n", garage->garage_name);
                else
                    printf("NOT FOUND IN CAMPUS\n");
            }


    } // REMOVE_GARAGE command
        else if (strcmp(command, "REMOVE_GARAGE") == 0) {
            int remove;
            scanf("%s", garageName);
            remove = removeGarage(campus, garageName);

            if (remove == 1) {
                printf("%s REMOVED\n", garageName);
            } else if (remove == 0) {
                printf("%s NOT FOUND\n", garageName);
            }
        }
    }
}

// initialize campus data
void initializeCampus(Campus* campus) {
    int commandsNum, garageCapacity;
    char garageName[MAX], licensePL[MAX], owner[MAX];

    // input data
    scanf("%d %d %d", &campus->total_garages, &campus->total_registered_vehicles, &commandsNum);

    // allocate memory for garage + vehicle pointers
    campus->garages = (Garage**)malloc(campus->total_garages * sizeof(Garage*));
    campus->registered_vehicles = (RegisteredVehicle**)malloc(campus->total_registered_vehicles * sizeof(RegisteredVehicle*));

    // initialize each garage with name + capacity
    for (int i = 0; i < campus->total_garages; i++) {
        scanf("%s %d", garageName, &garageCapacity);
        campus->garages[i] = createGarage(garageName, garageCapacity);
    }

    // initialize each registered vehicle
    for (int i = 0; i < campus->total_registered_vehicles; i++) {
        scanf("%s %s", licensePL, owner);
        campus->registered_vehicles[i] = createRegisteredVehicle(licensePL, owner);
    }

    // process commands
    commands(campus, commandsNum);
}

// free all mem
void freeMem(Campus* campus) {
    // free each vehicle
    for (int i = 0; i < campus->total_registered_vehicles; i++) {
        free(campus->registered_vehicles[i]->license_plate);
        free(campus->registered_vehicles[i]->owner_name);
        free(campus->registered_vehicles[i]);
    }

    // free each garage
    for (int i = 0; i < campus->total_garages; i++) {
        free(campus->garages[i]->garage_name);
        free(campus->garages[i]->parked_vehicles);
        free(campus->garages[i]);
    }

    free(campus->registered_vehicles);
    free(campus->garages);
    free(campus);
}


int main() {
    Campus *campus = (Campus*)malloc(sizeof(Campus));

    // initialize campus data
    initializeCampus(campus);

    // free allocated memory
    freeMem(campus);

    return 0;
}

