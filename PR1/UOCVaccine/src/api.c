#include <stdio.h>
#include <assert.h>
#include "csv.h"
#include "api.h"

#include <string.h>
#include "person.h"
#include "vaccine.h"


#define FILE_READ_BUFFER_SIZE 2048

// Get the API version information
const char* api_version() {
    return "UOC PP 20212";
}

// Load data from a CSV file. If reset is true, remove previous data
tApiError api_loadData(tApiData* data, const char* filename, bool reset) {
    tApiError error;
    FILE *fin;    
    char buffer[FILE_READ_BUFFER_SIZE];
    tCSVEntry entry;
    
    // Check input data
    assert( data != NULL );
    assert(filename != NULL);
    
    // Reset current data    
    if (reset) {
        // Remove previous information
        error = api_freeData(data);
        if (error != E_SUCCESS) {
            return error;
        }
        
        // Initialize the data
        error = api_initData(data);
        if (error != E_SUCCESS) {
            return error;
        }
    }

    // Open the input file
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return E_FILE_NOT_FOUND;
    }
    
    // Read file line by line
    while (fgets(buffer, FILE_READ_BUFFER_SIZE, fin)) {
        // Remove new line characte   error = api_initData(data);
        if (error != E_SUCCESS) {
            return error;
        }
    }

    // Open the input file
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return E_FILE_NOT_FOUND;
    }
    
    // Read file line by line
    while (fgets(buffer, FILE_READ_BUFFER_SIZE, fin)) {
        // Remove new line character     
        buffer[strcspn(buffer, "\n\r")] = '\0';
        
        csv_initEntry(&entry);
        csv_parseEntry(&entry, buffer, NULL);
        // Add this new entry to the api Data
        error = api_addDataEntry(data, entry);
        if (error != E_SUCCESS) {
            return error;
        }
        csv_freeEntry(&entry);
    }
    
    fclose(fin);
    
    return E_SUCCESS;
}

// Initialize the data structure
tApiError api_initData(tApiData* data) {            
    //////////////////////////////////
    // Ex PR1 2b
    /////////////////////////////////
	// Check input data
    assert(data != NULL);
	
	population_init(&data->population);
	vaccineList_init(&data->vaccines);
	vaccineLotData_init(&data->vaccineLots);
	
    return E_SUCCESS;
}

// Function that returns if a vaccine exists in a vaccineList by his name
bool api_existsVaccine(tVaccineList list, const char *name) {
	bool found = false;
	tVaccineNode *pNode = NULL;
	int i=0;
	
	// Point the first element
    pNode = list.first;
    
	while ((i < list.count) && (found == false)){
		if (strcmp(pNode->vaccine.name, name) == 0) {
			found = true;
		}
		pNode = pNode->next;
		i++;
	}
	
	return found;
}

// Add a new vaccines lot
tApiError api_addVaccineLot(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2c
    /////////////////////////////////
	tVaccine vaccine;
	tVaccineNode *pNode = NULL;
	tVaccineLot vaccineLot;
	int i = 0;
	bool found = false;
	
	if (strcmp(csv_getType(&entry), "VACCINE_LOT") != 0) {
		return E_INVALID_ENTRY_TYPE;
	}
	
	if (csv_numFields(entry) != 7) {
		return E_INVALID_ENTRY_FORMAT;
	}
	
	// get info from entry
	vaccineLot_parse(&vaccine, &vaccineLot, entry);
	// check if vaccine exists in list
    found = api_existsVaccine(data->vaccines, vaccine.name);
	//if it does not, add it
    if (found == false){
		vaccineList_insert(&data->vaccines, vaccine);
	}
	// find it and add the vaccine lot
	vaccineLot.vaccine = vaccineList_find(data->vaccines, vaccine.name);
	vaccineLotData_add(&data->vaccineLots, vaccineLot);
	vaccineLot_free(&vaccineLot);
	vaccine_free(&vaccine);
	
    return E_SUCCESS;
}

// Get the number of persons registered on the application
int api_populationCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return data.population.count;
}

// Get the number of vaccines registered on the application
int api_vaccineCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return data.vaccines.count;
}

// Get the number of vaccine lots registered on the application
int api_vaccineLotsCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return data.vaccineLots.count;
}


// Free all used memory
tApiError api_freeData(tApiData* data) {
    //////////////////////////////////
    // Ex PR1 2e
    /////////////////////////////////
	population_free(&data->population);
	vaccineLotData_free(&data->vaccineLots);
	vaccineList_free(&data->vaccines);
    return E_SUCCESS;
}


// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) { 
    //////////////////////////////////
    // Ex PR1 2f
    /////////////////////////////////
	tPerson person;
	int idx;
	
	// check if entry is of desired type
	if ((strcmp(csv_getType(&entry), "VACCINE_LOT") != 0) && (strcmp(csv_getType(&entry), "PERSON") != 0)){
		return E_INVALID_ENTRY_TYPE;
	}
	
	// both types of entries have 7 fields
	if (csv_numFields(entry) != 7) {
		return E_INVALID_ENTRY_FORMAT;
	}
	
	// if it is person, parse then find if it is not duplicated, add it
	if (strcmp(csv_getType(&entry), "PERSON") == 0){
		person_init(&person);
		person_parse(&person, entry);
		idx = population_find(data->population, person.document);
		if (idx >= 0){
			return E_DUPLICATED_PERSON;
		}
		population_add(&data->population, person);
	}
	
	//if it is of type vaccine, add it with vaccine lot
	if (strcmp(csv_getType(&entry), "VACCINE_LOT") == 0) {
		api_addVaccineLot(data, entry);
	}
	
	return E_SUCCESS;
}
	
// Function that returns the data from a vaccine formatted as requested
char* api_getVaccineAsStr(tVaccineList list, const char* name){
	tVaccine *vaccine;
	static char input[FILE_READ_BUFFER_SIZE];
	
	vaccine = vaccineList_find(list, name);
	sprintf(input, "%s;%d;%d", name, vaccine->required, vaccine->days);
	return input;
}

// Get vaccine data
tApiError api_getVaccine(tApiData data, const char *name, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3a
    /////////////////////////////////
	bool found;
	char input[FILE_READ_BUFFER_SIZE];
	
	// check if vaccine exists
	found = api_existsVaccine(data.vaccines, name);
	
	// if exists, parse into entry
	if (found == false) {
		return E_VACCINE_NOT_FOUND;
	} else {
		strcpy(input, api_getVaccineAsStr(data.vaccines, name));
		csv_parseEntry(entry, input, "VACCINE");
		return E_SUCCESS;
	}
}

// Function that returns the data from a vaccineLot formatted as requested
char* api_getVaccineLotAsStr(tVaccineLotData vaccineLots, int idx){
	static char input[FILE_READ_BUFFER_SIZE];
	sprintf(input, "%02d/%02d/%02d;%02d:%02d;%s;%s;%d;%d;%d", vaccineLots.elems[idx].timestamp.date.day,
												 vaccineLots.elems[idx].timestamp.date.month,
												 vaccineLots.elems[idx].timestamp.date.year,
												 vaccineLots.elems[idx].timestamp.time.hour, 
												 vaccineLots.elems[idx].timestamp.time.minutes,
												 vaccineLots.elems[idx].cp,
												 vaccineLots.elems[idx].vaccine->name, 
												 vaccineLots.elems[idx].vaccine->required,
												 vaccineLots.elems[idx].vaccine->days, 
												 vaccineLots.elems[idx].doses);
	return input;
}

// Get vaccine lot data
tApiError api_getVaccineLot(tApiData data, const char* cp, const char* vaccine, tDateTime timestamp, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3b
    //////////////////////////////////
	int idx;
	char input[FILE_READ_BUFFER_SIZE];
	bool found;
	
	// check if vaccine lot exists, if exists parse it into entry
	idx = vaccineLotData_find(data.vaccineLots, cp, vaccine, timestamp);
	if (idx >= 0) {
		strcpy(input, api_getVaccineLotAsStr(data.vaccineLots, idx));
		csv_parseEntry(entry, input, "VACCINE_LOT");
		return E_SUCCESS;
	} else {
		return E_LOT_NOT_FOUND;
	}	
}

// Get registered vaccines
tApiError api_getVaccines(tApiData data, tCSVData *vaccines) {
    //////////////////////////////////
    // Ex PR1 3c
    /////////////////////////////////
	tVaccineNode *pNode = NULL;
	int i=0;
	char input[FILE_READ_BUFFER_SIZE];
	
	// Point the first element
    pNode = data.vaccines.first;
    
	// for vaccine in list parse into entry format then add it to CSVData
	while (i < data.vaccines.count){
		strcpy(input, api_getVaccineAsStr(data.vaccines, pNode->vaccine.name));
		csv_addStrEntry(vaccines, input,  "VACCINE");
		pNode = pNode->next;
		i++;
	}
    return E_SUCCESS; 
}

// Get vaccine lots
tApiError api_getVaccineLots(tApiData data, tCSVData *lots) {
    //////////////////////////////////
    // Ex PR1 3d
    /////////////////////////////////
	int i;
	char input[FILE_READ_BUFFER_SIZE];
	
    // for vaccine lot parse into entry format then add to CSVData
	for (i=0; i < data.vaccineLots.count; i++){
		strcpy(input, api_getVaccineLotAsStr(data.vaccineLots, i));
		csv_addStrEntry(lots, input,  "VACCINE_LOT");
	}
    return E_SUCCESS; 
}
