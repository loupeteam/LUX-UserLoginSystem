
#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

unsigned long queryToJson(UDINT *requestUri, UDINT *queryJSON) {
	/*
	* This fuction is used to loop through each element of the uri and parse out the username and password if passed in as query parameters.
	* These parameters will be assigned as Key:Value pairs for later use. 
	*/
	
	strcat(queryJSON, "{ ");
	
	char* queryStr;	
	queryStr = strchr(requestUri, '?');

	char* token;
	const char outer_delimiters[] = "&";
	const char inner_delimiters[] = "=";
	char* outer_saveptr = NULL;
	char* inner_saveptr = NULL;
 
	token = strtok_r(queryStr+1, outer_delimiters, &outer_saveptr);
 
	while (token != NULL) {
		
		char* inner_token = strtok_r(token, inner_delimiters, &inner_saveptr);
 
		while (inner_token != NULL) {
			strcat(queryJSON, "\"");
			strcat(queryJSON, inner_token);
			
			inner_token = strtok_r(NULL, inner_delimiters,&inner_saveptr);
			
			if(inner_token == NULL){
				strcat(queryJSON, "\",");
			}
			else{
				strcat(queryJSON, "\":");
			}
			
		}
 
		token = strtok_r(NULL, outer_delimiters,&outer_saveptr);
	
	}

	strcat(queryJSON, " }");
	
	return 0; // No errors
}
