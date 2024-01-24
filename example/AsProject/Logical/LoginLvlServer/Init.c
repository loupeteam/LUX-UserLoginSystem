#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/*
* This file is used to set up the unchanging or initial values needed in the LoginLvlServer task.
*/

void _INIT ProgramInit(void)
{
	// ----------------- HTTP SERVER -----------------
	// Set up the HTTP Server FUB Parameters
	task.internal.server.enable = 1;
	task.internal.server.https = 0;
	task.internal.server.numClients = 4;
	task.internal.server.port = 1238;
	strcpy(task.internal.server.ipAddress, "127.0.0.1");
	task.internal.server.bufferSize = 12000;
	
	// ----------------- DEFAULT HTTP RESPONSE -----------------
	// Set up default Response for HTTP requests that are NOT to desired/expected uri's
	task.internal.defaultResponse.method = LLHTTP_METHOD_DEFAULT;
	strcpy(task.internal.defaultResponse.uri, "**");
	task.internal.defaultResponse.pUserHeader = &task.internal.responseHeader.lines;
	task.internal.defaultResponse.numUserHeaders = sizeof(task.internal.responseHeader.lines)/sizeof(task.internal.responseHeader.lines[0]);
	// Add custom 404 page
	strcpy(task.internal.defaultSendBuffer, ""
		"<!DOCTYPE html>"
		"<html lang=\"en\">"
		"<head>"
		"<meta charset=\"utf-8\" /><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
		"<title>We&#39;ve got some trouble | 404 - Resource not found</title>"
		"</head>"
		"<body>"
		"<div class=\"cover\"><h1>Resource not found <small>404</small></h1><p class=\"lead\">The requested resource could not be found but may be available again in the future.</p></div>"
		"<footer><p>Technical Contact: <a href=\"mailto:support@loupe.team\">support@loupe.team</a></p></footer>"
		"</body>"
		"</html>"
		);
	
	// ----------------- CUSTOM HTTP RESPONSE -----------------
	// Set up Response for HTTP requests that ARE to a specific, expected uri
	task.internal.response.enable = 1;
	task.internal.response.method = LLHTTP_METHOD_ANY;
	strcpy(task.internal.response.uri, "/getUserLvl");
	strcpy(task.internal.sendBuffer.message, "Default Response");
	task.internal.response.pUserHeader = &task.internal.responseHeader.lines;
	task.internal.response.numUserHeaders = sizeof(task.internal.responseHeader.lines)/sizeof(task.internal.responseHeader.lines[0]);
	LLHttpAddHeaderField(&task.internal.responseHeader.lines,26,"Access-Control-Allow-Origin","*");
	
	// ----------------- QUERY PARAMETERS PARSER -----------------
	// Initialize parser values to default
	JsmnInit((UDINT)&task.internal.parser);
	strcpy((UDINT)&task.internal.queryJSON, "");
	
	// ----------------- CONFIGURE USERS & ROLES -----------------
	// Set app values (from Configuration)
	if(useConfig) {
		//for(int i = 0; i<MAI_USERS; i++) {
		strcpy(task.internal.users[0].username, Configuration.users[0].username);
		strcpy(task.internal.users[0].password, Configuration.users[0].password);
		strcpy(task.internal.users[0].loginLvl, Configuration.users[0].loginLvl);
		//}
	} else {
		strcpy(task.internal.users[0].username, "admin2");
		strcpy(task.internal.users[0].password, "test");
		task.internal.users[0].loginLvl = ADMIN;
		strcpy(task.internal.users[1].username, "admin1");
		strcpy(task.internal.users[1].password, "test1");
		task.internal.users[1].loginLvl = ADMIN;
	}
	//	// Set Command to call custom function to set up users in the system
		task.cmd.configureUsers = 0;
	//	
	//	// Export List of user to File for debugging
	//	task.internal.ArUser.ArUserInitList_0.Execute = 1; 
	//	task.internal.ArUser.ArUserInitList_0.ListType = arUSER_USER;
	//	memcpy(&task.internal.ArUser.ArUserInitList_0.ElementName, &task.internal.users[0].username, strlen(task.internal.users[0].username));
	//	// Run the Export FUB
	//	ArUserInitList(&task.internal.ArUser.ArUserInitList_0);
	//	// Check that the FUB is complete
	//	if (&task.internal.ArUser.ArUserInitList_0.Done && !&task.internal.ArUser.ArUserInitList_0.Busy) {
	//		task.internal.ArUser.ArUserInitList_0.Execute = 0;
	//		task.cmd.configureUsers = 0;
	//	} else if (&task.internal.ArUser.ArUserInitList_0.Error) {
	//		task.status.error = 1;
	//	}

	// Loop Through User Array and Create ArUsers with LoginLvl Properties
//	for(int i = 0; i<MAI_USERS; i) {
//		ConfigureUser(&task.internal.ArUser, &task.internal.users[1]);
////	}
//			
//	// Export List of user to File for debugging
//	//	strcpy(&task.internal.ArUser.path, "C:\\Users\\curti\\TEMP");
//	memcpy(&task.internal.ArUser.ArUserExport_0.FilePath, &task.internal.ArUser.FilePath, strlen(task.internal.ArUser.FilePath));
//	task.internal.ArUser.ArUserExport_0.Execute = 1;
//	// Run the Export FUB
//	ArUserExport(&task.internal.ArUser.ArUserExport_0);
//	// Check that the FUB is complete
//	if (&task.internal.ArUser.ArUserExport_0.Done && !&task.internal.ArUser.ArUserExport_0.Busy) {
//		task.internal.ArUser.ArUserExport_0.Execute = 0;
//		task.cmd.configureUsers = 0;
//	} else if (&task.internal.ArUser.ArUserExport_0.Error) {
//		task.status.error = 1;
//	}

	// ----------------- USER LEVEL RESPONSE -----------------
	// Initialize & complie the Chopper template 
	strcpy(task.internal.sendBuffer.template.source, "{\"userLevel\":\"{{LLHTTPServ:task.internal.userLvl}}\" }");
	ChopCompile((UDINT)&task.internal.sendBuffer.template, (UDINT)&task.internal.sendBuffer.template.source);
	
}
