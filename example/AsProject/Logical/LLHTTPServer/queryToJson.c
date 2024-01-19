
#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

unsigned long queryToJson(UDINT *requestUri, UDINT *queryJSON) {
	/*
	* 
	* This fuction is used to parse the uri and convert the query parameters to a JSON string.
	* This assumes the destination string (queryJSON) is empty when the funciton is called. 
	*
	*/
	
	// Add the begining JSON bracket
	strcat(queryJSON, "{ ");
	
	// Create a substring of just the query parameters
	char* queryStr;
	char* tempUriCopy;
	strcpy(tempUriCopy, requestUri);
	queryStr = strchr(tempUriCopy, '?');
	
	// Set up vars for use with strtok_r function from the string.h library
	char* token;
	const char outer_delimiters[] = "&";
	const char inner_delimiters[] = "=";
	char* outer_saveptr = NULL;
	char* inner_saveptr = NULL;
 	
	// Find the first set of tokens (key:value pairs) using the outer delimiter (&)
	token = strtok_r(queryStr+1, outer_delimiters, &outer_saveptr);
	// Loop through the tokens
	while (token != NULL) {
		
		// Find the first set of inner tokens (keys or values) using the inner delimter (=)
		char* inner_token = strtok_r(token, inner_delimiters, &inner_saveptr);
 		// Loop through the inner tokens 
		while (inner_token != NULL) {
			
			// Add the inner token to the output JSON string
			strcat(queryJSON, "\"");
			strcat(queryJSON, inner_token);
			
			// Find the next inner token (key or value)
			inner_token = strtok_r(NULL, inner_delimiters,&inner_saveptr);
			
			// Check if any more inner tokens exist
			if(inner_token == NULL){
				// Add a comma to the output JSON to separate from the next key:value pair
				strcat(queryJSON, "\",");
			} else{
				// Add a colon to the output JSON to separate keys from values
				strcat(queryJSON, "\":");
			}
			
		}
		
 		// Find the next outer token (key:value pair)
		token = strtok_r(NULL, outer_delimiters,&outer_saveptr);
	}
	
	// Add the ending bracket to the output JSON string
	strcat(queryJSON, " }");
	
	return 0; // No errors
}
