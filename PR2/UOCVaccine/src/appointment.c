#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "appointment.h"
#include <malloc.h>

// Initializes a vaccination appointment data list
void appointmentData_init(tAppointmentData* list) {
    //////////////////////////////////
    // Ex PR3 1a
    /////////////////////////////////
	list->elems=NULL;
	list->count = 0;
}


void appointment_init(tAppointment* appointment, tPerson* person, tVaccine* vaccine, tDateTime timestamp){
	appointment->person = person;
	appointment->vaccine = vaccine;
	appointment->timestamp = timestamp;
}

void appointment_free(tAppointment* data){
    
    // Release vaccine data
    data->vaccine = NULL;
	free(data->vaccine);
    
    // Release person data
    data->person = NULL;
	free(data->person);
}

// Insert a new vaccination appointment
void appointmentData_insert(tAppointmentData* list, tDateTime timestamp, tVaccine* vaccine, tPerson* person){
	//////////////////////////////////
    // Ex PR3 1b
    /////////////////////////////////
	int start_pos = 0, idx =0, i;
	idx = appointmentData_findTimestamp(list, timestamp, start_pos);

	// If timestamp is last
    if (idx < 0) {   
        // Allocate memory for new element
		if (list->count == 0) {
			// Request new memory space
			list->elems = (tAppointment*) malloc(sizeof(tAppointment));            
		} else {
			// Modify currently allocated memory
			list->elems = (tAppointment*) realloc(list->elems, (list->count + 1) * sizeof(tAppointment));            
		}
        appointment_init(&(list->elems[list->count]), person, vaccine, timestamp);
        // Increase the number of elements
        list->count ++;
    } else {
		start_pos = idx;
		idx = appointmentData_findDocument(list, timestamp, person->document, start_pos);
		
		if (idx < 0){
			if (list->count == 0) {
				// Request new memory space
				list->elems = (tAppointment*) malloc(sizeof(tAppointment));            
			} else {
				// Modify currently allocated memory
				list->elems = (tAppointment*) realloc(list->elems, (list->count + 1) * sizeof(tAppointment));            
			}
			appointment_init(&(list->elems[list->count]), person, vaccine, timestamp);
			// Increase the number of elements
			list->count ++;
		} else {
			list->elems = (tAppointment*) realloc(list->elems, (list->count + 1) * sizeof(tAppointment));
			for (i=list->count; i>=idx; i--){
				appointment_init(&(list->elems[i]), list->elems[i-1].person, list->elems[i-1].vaccine, list->elems[i-1].timestamp);
			}
			appointment_init(&(list->elems[idx]), person, vaccine, timestamp);
			// Increase the number of elements
			list->count ++;
		}
	}
}

// Remove a vaccination appointment
void appointmentData_remove(tAppointmentData* list, tDateTime timestamp, tPerson* person) {
    //////////////////////////////////
    // Ex PR3 1c
    /////////////////////////////////
	int idx =0, start_pos=0, i=0, j=0;
	bool found=false;

	
	while ((i < list->count) && (!found)){
		if ((dateTime_cmp(list->elems[i].timestamp, timestamp) == 0) && 
			(list->elems[i].person == person)){
			found = true;
		}
		i++;
	}
	
	
	if (found){
		i = i-1;
		if (i>=0 && i<=list->count){
			for (j=i; j < list->count-1; j++){
				/*appointment_free(&(list->elems[j]));*/
				appointment_init(&(list->elems[j]), list->elems[j+1].person, list->elems[j+1].vaccine, list->elems[j+1].timestamp);
				/*list->elems[j] = list->elems[j+1];*/
			}
			/*appointment_free(&(list->elems[list->count]));*/
			list->count--;
			if (list->count == 0){
				free(list->elems);
				list->elems = NULL;
			} else {
				list->elems = (tAppointment*)realloc(list->elems, list->count * sizeof(tAppointment));
				if (list->elems == NULL) {
					printf("Error, resize memory fail/n");
				}
			}
		}
	}
}


// Find the first instance of a vaccination appointment for given person
int appointmentData_find(tAppointmentData list, tPerson* person, int start_pos) {
    //////////////////////////////////
    // Ex PR3 1d
    /////////////////////////////////
	if (start_pos >= list.count  || start_pos < 0){
		return -1;
	} else {
		if (list.elems[start_pos].person == person){
			return start_pos;
		} else {
			start_pos++;
			appointmentData_find(list, person, start_pos);
		}
	}
	
}

// Find the first instance of a vaccination appointment with bigger or equal timestamp
int appointmentData_findTimestamp(tAppointmentData *list, tDateTime timestamp, int start_pos) {
    //////////////////////////////////
    // Ex PR3 1d
    /////////////////////////////////   
	if (start_pos >= list->count  || start_pos < 0){
		return -1;
	} else {
		if (dateTime_cmp(list->elems[start_pos].timestamp, timestamp)>=0){
			return start_pos;
		} else {
			start_pos++;
			appointmentData_findTimestamp(list, timestamp, start_pos);
		}
	}
}

// Find the first instance of a vaccination appointment with bigger or equal timestamp by document
int appointmentData_findDocument(tAppointmentData *list, tDateTime timestamp, const char* document, int start_pos) {
    //////////////////////////////////
    // Ex PR3 1d
    /////////////////////////////////   
	if (start_pos >= list->count  || start_pos < 0){
		return -1;
	} else {
		if (strcmp(list->elems[start_pos].person->document, document)>=0 || dateTime_cmp(list->elems[start_pos].timestamp, timestamp)!=0){
			return start_pos;
		} else {
			start_pos++;
			appointmentData_findDocument(list, timestamp, document, start_pos);
		}
	}
}

// Release a vaccination appointment data list
void appointmentData_free(tAppointmentData* list) {
    //////////////////////////////////
    // Ex PR3 1e
    /////////////////////////////////
	if (list->elems != NULL){
		free(list->elems);
		list->count=0;
		list->elems=NULL;
	}
}
