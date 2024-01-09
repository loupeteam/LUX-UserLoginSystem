
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

unsigned long parseQuery(UDINT *requestUri, UDINT *queryJSON) {
	/*
	* This fuction is used to loop through each element of the uri and parse out the username and password if passed in as query parameters.
	* These parameters will be assigned as Key:Value pairs for later use. 
	*/
	strcat(queryJSON, "{");
	
	return 0; // No errors
}
