/*
 * File: queryCallback.c
 * Copyright (c) 2024 Loupe
 * https://loupe.team
 * 
 * This file is part of LUX-UserLoginSystem, licensed under the MIT License.
 * 
 */
#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

unsigned long queryCallback(parsedQueryData_typ* data, jsmn_callback_data* callbackData) {
	/*
	*
	* This Callback is used by the parser to loop through each line of a recieved JSON string
	* and extract the desired information. 
	* The Key:value pairs are directly compared to the hard coded strings below
	*
	*/
	
	if(callbackData->Size == 0) { 

		if((brsstrcmp((UDINT)callbackData->Structure[1], (UDINT)"userName") == 0)) { 			// Check if Key is "userName"
			brsstrcpy((UDINT)data->userName, (UDINT)callbackData->Value); 						// Apply json value to PLC var
		}
			
		else if(brsstrcmp((UDINT)callbackData->Structure[1], (UDINT)"password") == 0 ) { 	// Check if Key is "password"
			brsstrcpy((UDINT)data->password, (UDINT)callbackData->Value); 					// Apply json value to PLC var
		}
	}

	return 0; // No errors
}

/*

*/
