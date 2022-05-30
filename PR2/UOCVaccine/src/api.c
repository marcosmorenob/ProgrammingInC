#include <stdio.h>
#include <assert.h>
#include "csv.h"
#include "api.h"

#include <string.h>
#include "person.h"
#include "vaccine.h"
#include "appointment.h"


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
    // Check input data structure
    assert(data != NULL);
    
    // Initialize data structures
    population_init(&(data->population));
    vaccineList_init(&(data->vaccines));
    vaccineLotData_init(&(data->vaccineLots));
    
    
    //////////////////////////////////
    // Ex PR1 2b
    /////////////////////////////////
    centerList_init(&(data->centers));
    /////////////////////////////////
    
    return E_SUCCESS;
    
    /////////////////////////////////
    // return E_NOT_IMPLEMENTED;
}

// Add a new vaccines lot
tApiError api_addVaccineLot(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2c
    /////////////////////////////////
    tVaccine vaccine;
    tVaccineLot lot;
    tVaccine *pVaccine;
    
    //////////////////////////////////
    // Ex PR2 3c
    /////////////////////////////////
    tHealthCenter *pCenter;
    /////////////////////////////////
    
    // Check input data structure
    assert(data != NULL);
    
    // Check the entry type
    if (strcmp(csv_getType(&entry), "VACCINE_LOT") != 0) {
        return E_INVALID_ENTRY_TYPE;
    }
    
    // Check the number of fields
    if(csv_numFields(entry) != 7) {
        return E_INVALID_ENTRY_FORMAT;
    }
    
    // Parse the entry
    vaccineLot_parse(&vaccine, &lot, entry);
    
    // Check if vaccine exists
    pVaccine = vaccineList_find(data->vaccines, vaccine.name);
    if (pVaccine == NULL) {
        // Add the vaccine
        vaccineList_insert(&(data->vaccines), vaccine);
        pVaccine = vaccineList_find(data->vaccines, vaccine.name);
    }
    assert(pVaccine != NULL);
    
    // Assign this vaccine to the lot
    lot.vaccine = pVaccine;
    
    // Add the lot to the data
    vaccineLotData_add(&(data->vaccineLots), lot);
    
    
    //////////////////////////////////
    // Ex PR2 3c
    /////////////////////////////////
    pCenter = centerList_find(&(data->centers), lot.cp);
    if (pCenter == NULL) {
        centerList_insert(&(data->centers), lot.cp);
        pCenter = centerList_find(&(data->centers), lot.cp);
    }
    stockList_update(&(pCenter->stock), lot.timestamp.date, lot.vaccine, lot.doses);
    /////////////////////////////////
    
    
    // Release temporal data
    vaccine_free(&vaccine);
    vaccineLot_free(&lot);
    
    return E_SUCCESS;
    
    /////////////////////////////////
    
    //return E_NOT_IMPLEMENTED;
}

// Get the number of persons registered on the application
int api_populationCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return population_len(data.population);
    /////////////////////////////////
    //return -1;
}

// Get the number of vaccines registered on the application
int api_vaccineCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return vaccineList_len(data.vaccines);    
    /////////////////////////////////
    //return -1;
}

// Get the number of vaccine lots registered on the application
int api_vaccineLotsCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return vaccineLotData_len(data.vaccineLots);
    /////////////////////////////////
    //return -1;
}


// Free all used memory
tApiError api_freeData(tApiData* data) {
    //////////////////////////////////
    // Ex PR1 2e
    /////////////////////////////////
    population_free(&(data->population));
    vaccineLotData_free(&(data->vaccineLots));
    vaccineList_free(&(data->vaccines));
    
    //////////////////////////////////
    // Ex PR2 3d
    /////////////////////////////////
    centerList_free(&(data->centers));
    /////////////////////////////////
    
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}


// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) { 
    //////////////////////////////////
    // Ex PR1 2f
    /////////////////////////////////
    int personIdx;
    tPerson person;
        
    assert(data != NULL);
    
    // Initialize the person object
    person_init(&person);
        
    if (strcmp(csv_getType(&entry), "PERSON") == 0) {
        // Check the number of fields
        if(csv_numFields(entry) != 7) {
            return E_INVALID_ENTRY_FORMAT;
        }
        // Parse the data
        person_parse(&person, entry);
        
        // Check if this person already exists
        if (population_find(data->population, person.document) >= 0) {
            // Release person object
            person_free(&person);
            return E_DUPLICATED_PERSON;
        }
        
        // Add the new person
        population_add(&(data->population), person);
        
        // Release person object
        person_free(&person);
        
    } else if (strcmp(csv_getType(&entry), "VACCINE_LOT") == 0) {
        return api_addVaccineLot(data, entry);        
    } else {
        return E_INVALID_ENTRY_TYPE;
    }
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get vaccine data
tApiError api_getVaccine(tApiData data, const char *name, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3a
    /////////////////////////////////
    char buffer[2048];
    tVaccine* vaccine = NULL;
        
    assert(name != NULL);
    assert(entry != NULL);
    
    // Search vaccine
    vaccine = vaccineList_find(data.vaccines, name);
    
    if (vaccine == NULL) {
        return E_VACCINE_NOT_FOUND;
    }
    
    // Print data in the buffer
    sprintf(buffer, "%s;%d;%d", vaccine->name, vaccine->required, vaccine->days);
    
    // Initialize the output structure
    csv_initEntry(entry);
    csv_parseEntry(entry, buffer, "VACCINE");
    
    return E_SUCCESS;
    
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get vaccine lot data
tApiError api_getVaccineLot(tApiData data, const char* cp, const char* vaccine, tDateTime timestamp, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3b
    /////////////////////////////////
    char buffer[2048];
    int idx;
        
    assert(cp != NULL);
    assert(vaccine != NULL);
    assert(entry != NULL);
    
    // Search vaccine
    idx = vaccineLotData_find(data.vaccineLots, cp, vaccine, timestamp);
        
    if (idx < 0) {
        return E_LOT_NOT_FOUND;
    }
    
    // Print data in the buffer
    sprintf(buffer, "%02d/%02d/%04d;%02d:%02d;%s;%s;%d;%d;%d", 
        data.vaccineLots.elems[idx].timestamp.date.day, data.vaccineLots.elems[idx].timestamp.date.month, data.vaccineLots.elems[idx].timestamp.date.year,
        data.vaccineLots.elems[idx].timestamp.time.hour, data.vaccineLots.elems[idx].timestamp.time.minutes,
        data.vaccineLots.elems[idx].cp,
        data.vaccineLots.elems[idx].vaccine->name, data.vaccineLots.elems[idx].vaccine->required, data.vaccineLots.elems[idx].vaccine->days,
        data.vaccineLots.elems[idx].doses
    );
    
    // Initialize the output structure
    csv_initEntry(entry);
    csv_parseEntry(entry, buffer, "VACCINE_LOT");
    
    return E_SUCCESS;
    
    
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get registered vaccines
tApiError api_getVaccines(tApiData data, tCSVData *vaccines) {
    //////////////////////////////////
    // Ex PR1 3c
    /////////////////////////////////
    char buffer[2048];
    tVaccineNode *pNode = NULL;
    
    csv_init(vaccines);
        
    pNode = data.vaccines.first;
    while(pNode != NULL) {
        sprintf(buffer, "%s;%d;%d", pNode->vaccine.name, pNode->vaccine.required, pNode->vaccine.days);
        csv_addStrEntry(vaccines, buffer, "VACCINE");
        pNode = pNode->next;
    }    
    
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get vaccine lots
tApiError api_getVaccineLots(tApiData data, tCSVData *lots) {
    //////////////////////////////////
    // Ex PR1 3d
    /////////////////////////////////
    char buffer[2048];
    int idx;
    
    csv_init(lots);
    for(idx=0; idx<data.vaccineLots.count ; idx++) {
        sprintf(buffer, "%02d/%02d/%04d;%02d:%02d;%s;%s;%d;%d;%d", 
            data.vaccineLots.elems[idx].timestamp.date.day, data.vaccineLots.elems[idx].timestamp.date.month, data.vaccineLots.elems[idx].timestamp.date.year,
            data.vaccineLots.elems[idx].timestamp.time.hour, data.vaccineLots.elems[idx].timestamp.time.minutes,
            data.vaccineLots.elems[idx].cp,
            data.vaccineLots.elems[idx].vaccine->name, data.vaccineLots.elems[idx].vaccine->required, data.vaccineLots.elems[idx].vaccine->days,
            data.vaccineLots.elems[idx].doses
        );
        csv_addStrEntry(lots, buffer, "VACCINE_LOT");
    }
    
    return E_SUCCESS;
    
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get the number of health centers registered on the application
int api_centersCount(tApiData data) {
    //////////////////////////////////
    // Ex PR2 3e
    
    return data.centers.count;
    
    /////////////////////////////////
    // return -1;
}

// Print center stock
void api_printCenterStock(tApiData data, const char* cp) {
    tHealthCenter *pCenter;
    
    // Check input data    
    assert(cp != NULL);
    
    // Search the health center
    pCenter = centerList_find(&(data.centers), cp);
    if (pCenter != NULL) {
        printf("==============================\n");
        printf("STOCK FOR CENTER %s\n", cp);
        printf("==============================\n");
        stockList_print(pCenter->stock);
        printf("==============================\n\n");
    }    
}

// Add a new vaccination appointment
tApiError api_addAppointment(tApiData* data, const char* cp, const char* document, const char* vaccine, tDateTime timestamp) {
    //////////////////////////////////
    // Ex PR3 2c
    /////////////////////////////////
	int person_idx, i = 0;
	tVaccine* vaccineFound;
	tHealthCenter* hcenterFound;
	
	person_idx = population_find(data->population, document);
	vaccineFound = vaccineList_find(data->vaccines, vaccine);
	hcenterFound = centerList_find(&(data->centers), cp);
	if (person_idx == -1){
		return E_PERSON_NOT_FOUND;
	} else if (vaccineFound == NULL){
		return E_VACCINE_NOT_FOUND;
	} else if (hcenterFound == NULL){
		return E_HEALTH_CENTER_NOT_FOUND;
	} 
	
	appointmentData_insert(&(hcenterFound->appointments), timestamp, vaccineFound, &(data->population.elems[person_idx]));

	for (i=1; i<vaccineFound->required; i++){
		dateTime_addDay(&timestamp, vaccineFound->days);
		appointmentData_insert(&(hcenterFound->appointments), timestamp, vaccineFound, &(data->population.elems[person_idx]));
	}
	return E_SUCCESS;
}

// Get person appointments
tApiError api_getPersonAppointments(tApiData data, const char* document, tCSVData *appointments) {
    //////////////////////////////////
    // Ex PR3 2d
    /////////////////////////////////
	int idx=0, person_idx, start_pos=0;
	tHealthCenter* hcenterFound;
	char buffer[FILE_READ_BUFFER_SIZE];
	
	person_idx = population_find(data.population, document);
	if (person_idx == -1){
		return E_PERSON_NOT_FOUND;
	}
	
	hcenterFound = centerList_find(&(data.centers),data.population.elems[person_idx].cp);
	idx = appointmentData_find(hcenterFound->appointments, &(data.population.elems[person_idx]), start_pos);
	while (idx >= 0){
		start_pos = idx;
		sprintf(buffer, "%02d/%02d/%04d;%02d:%02d;%s;%s", 
            hcenterFound->appointments.elems[start_pos].timestamp.date.day, hcenterFound->appointments.elems[start_pos].timestamp.date.month, hcenterFound->appointments.elems[start_pos].timestamp.date.year,
            hcenterFound->appointments.elems[start_pos].timestamp.time.hour, hcenterFound->appointments.elems[start_pos].timestamp.time.minutes,
            hcenterFound->cp,
            hcenterFound->appointments.elems[start_pos].vaccine->name);
		csv_addStrEntry(appointments, buffer, "APPOINTMENT");
		idx = appointmentData_find(hcenterFound->appointments, &(data.population.elems[person_idx]), start_pos+1);
	}
    return E_SUCCESS; 
}

// Check availability of a vaccine in a given health center
/*bool api_checkAvailability(tApiData data, const char* cp, const char* vaccine, tDate date) {
    //////////////////////////////////
    // Ex PR3 3a
    /////////////////////////////////
	int vaccine_pos, i;
	tVaccine* vaccineFound;
	tHealthCenter* hcenterFound;
	
	vaccineFound = vaccineList_find(data.vaccines, vaccine);
	hcenterFound = centerList_find(&(data.centers), cp);
	vaccine_pos = vaccineLotData_findbyTime(data.vaccineLots, cp, vaccine, date);
	if (vaccine_pos < 0 || vaccine == NULL || hcenterFound == NULL ){
		return false;
	} else {
		if (vaccineFound->required == 1) {
			if (data.vaccineLots.elems[vaccine_pos].doses>=1){
				return true;
			} else {
				return false;
			}
		} else {
			date_addDay(&date, vaccineFound->days);
			vaccine_pos = vaccineLotData_findbyTime(data.vaccineLots, cp, vaccine, date);
			if (data.vaccineLots.elems[vaccine_pos].doses >= vaccineFound->required){
				return true;
			} else {
				return false;
			}
		}
	}
}*/


bool api_checkAvailability(tApiData data, const char* cp, const char* vaccine, tDate date) {
    //////////////////////////////////
    // Ex PR3 3a
    /////////////////////////////////
	int vaccine_pos, i;
	tDate mod_date;
	tHealthCenter* hcenterFound;
	tVaccineDailyStock* dailyStock;
	tVaccineStockNode* stock;
	tVaccine* vaccineFound;
	
	hcenterFound = centerList_find(&(data.centers), cp);
	
	if (hcenterFound->stock.count == 0 || hcenterFound == NULL){
		return false;
	}
	dailyStock = stockList_find(&(hcenterFound->stock), date);
	if (dailyStock == NULL){
		return false;
	} 
	vaccineFound = vaccineList_find(data.vaccines, vaccine);
	if (vaccine == NULL){
		return false;
	} 
	stock = dailyStock_find(dailyStock, vaccineFound);
	if (stock == NULL){
		return false;
	}
	if (stock->elem.vaccine->required == 1){
		if (stock->elem.doses >= 1){
			return true;
		} else {
			return false;
		}
	} else {
		date_addDay(&date, vaccineFound->days);
		stockList_expandRight(&(hcenterFound->stock), date);
		dailyStock = stockList_find(&(hcenterFound->stock), date);
		if (dailyStock == NULL){
			return false;
		} 
		stock = dailyStock_find(dailyStock, vaccineFound);
		if (stock == NULL){
			return false;
		}
		if (stock->elem.doses >= vaccineFound->required){
			return true;
		} else {
			return false;
		}
	}
	return false;
}

// Check availability of a vaccine in a given health center
tVaccineStockNode* api_checkAvailabilityanyVaccine(tApiData *data, const char* cp, tDate date) {
    //////////////////////////////////
    // Ex PR3 3a
    /////////////////////////////////
	int vaccine_pos, i;
	tDate mod_date;
	tHealthCenter* hcenterFound;
	tVaccineDailyStock* dailyStock;
	tVaccineStockNode* stock;
	
	hcenterFound = centerList_find(&(data->centers), cp);
	
	if (hcenterFound->stock.count == 0 || hcenterFound == NULL){
		return NULL;
	}
	dailyStock = stockList_find(&(hcenterFound->stock), date);
	if (dailyStock == NULL){
		return NULL;
	} 
	
	stock = dailyStock->first;
	while (stock != NULL && (stock->elem.doses < stock->elem.vaccine->required)){
		stock = stock->next;
	} 
	if (stock!=NULL){
		return stock;
	}
	return NULL;
}

	

// Find available vaccination appointment
tApiError api_findAppointmentAvailability(tApiData* data, const char* cp, const char* document, tDateTime timestamp) {
    //////////////////////////////////
    // Ex PR3 3b
    /////////////////////////////////
	int person_idx, i = 0, vaccine_pos, appointment_idx, start_pos=0;
	tHealthCenter* hcenterFound;
	tVaccineStockNode* stock;
	tVaccineDailyStock* dailyStock;
	tVaccineStockData* stockData;

	
	person_idx = population_find(data->population, document);
	hcenterFound = centerList_find(&(data->centers), cp);
	if (person_idx == -1){
		return E_PERSON_NOT_FOUND;
	} else if (hcenterFound == NULL){
		return E_HEALTH_CENTER_NOT_FOUND;
	} else if (hcenterFound->stock.count == 0){
		return E_NO_VACCINES;
	} else {
		appointment_idx = appointmentData_find(hcenterFound->appointments, &(data->population.elems[person_idx]), start_pos);
		if (appointment_idx >= 0 ){
			return E_DUPLICATED_PERSON;
		}
	}
	
	for (i=0; i<6; i++){
		stock = api_checkAvailabilityanyVaccine(data, cp, timestamp.date);
		if (stock != NULL) {
			api_addAppointment(data, cp, document, stock->elem.vaccine->name, timestamp);
			stockData = &(hcenterFound->stock);
			dailyStock = stockList_find(stockData, timestamp.date);
			dailyStock_update(dailyStock, stock->elem.vaccine, 1);
			if (stock->elem.vaccine->required > 1){
				date_addDay(&(timestamp.date), stock->elem.vaccine->required);
				api_addAppointment(data, cp, document, stock->elem.vaccine->name, timestamp);
				stockData = &(hcenterFound->stock);
				dailyStock = stockList_find(stockData, timestamp.date);
				dailyStock_update(dailyStock, stock->elem.vaccine, 1);
			}
			return E_SUCCESS;
		}
		dateTime_addDay(&timestamp, 1);
	}
	
	if (i==6){
		for (i=0; i<6; i++){
		stock = api_checkAvailabilityanyVaccine(data, cp, timestamp.date);
		if (stock != NULL) {
			api_addAppointment(data, cp, document, stock->elem.vaccine->name, timestamp);
			stockData = &(hcenterFound->stock);
			dailyStock = stockList_find(stockData, timestamp.date);
			dailyStock_update(dailyStock, stock->elem.vaccine, 1);
			if (stock->elem.vaccine->required > 1){
				date_addDay(&(timestamp.date), stock->elem.vaccine->required);
				api_addAppointment(data, cp, document, stock->elem.vaccine->name, timestamp);
				stockData = &(hcenterFound->stock);
				dailyStock = stockList_find(stockData, timestamp.date);
				dailyStock_update(dailyStock, stock->elem.vaccine, 1);
			}
			return E_SUCCESS;
		}
		dateTime_addDay(&timestamp, 1);
	}
		return E_NO_VACCINES;
	}
	return E_NO_VACCINES;
}
