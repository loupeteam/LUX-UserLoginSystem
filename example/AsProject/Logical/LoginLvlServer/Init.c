#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/*
* This file is used to set up the unchanging or initial values needed in the LoginLvlServer task.
*/

// Create Function prototypes
void ConfigureUsers();

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
	task.internal.defaultResponse.enable = 1;
	task.internal.defaultResponse.pRequest = (UDINT)&task.internal.defaultReceiveBuffer;
	task.internal.defaultResponse.requestSize = sizeof(task.internal.defaultReceiveBuffer);
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
	task.internal.defaultResponse.pContent = (UDINT)&task.internal.defaultSendBuffer;
	task.internal.defaultResponse.contentLength = strlen(task.internal.defaultSendBuffer);
	task.internal.defaultResponse.status = LLHTTP_STAT_NotFound;
	
	// ----------------- CUSTOM HTTP RESPONSE -----------------
	// Set up Response for HTTP requests that ARE to a specific, expected uri
	task.internal.response.enable = 1;
	task.internal.response.method = LLHTTP_METHOD_ANY;
	strcpy(task.internal.response.uri, "/getLoginLvl");
	strcpy(task.internal.sendBuffer.message, "Default Response");
	task.internal.response.pContent = (UDINT)&task.internal.sendBuffer.message;
	task.internal.response.contentLength = strlen(task.internal.sendBuffer.message);
	task.internal.response.pUserHeader = &task.internal.responseHeader.lines;
	task.internal.response.numUserHeaders = sizeof(task.internal.responseHeader.lines)/sizeof(task.internal.responseHeader.lines[0]);
	LLHttpAddHeaderField(&task.internal.responseHeader.lines,26,"Access-Control-Allow-Origin","*");
	
	// ----------------- QUERY PARAMETERS PARSER -----------------
	// Initialize parser values to default
	JsmnInit((UDINT)&task.internal.parser);
	strcpy((UDINT)&task.internal.queryJSON, "");
	
	// ----------------- CONFIGURE USERS & ROLES -----------------
	// Call custom function for User system setup
	ConfigureUsers();
	
	// ----------------- USER LEVEL RESPONSE -----------------
	// Initialize & complie the Chopper template 
	strcpy(task.internal.sendBuffer.template.source, "{\"loginLvl\":\"{{LoginLvlSe:task.internal.loginLvl}}\" }");
	ChopCompile((UDINT)&task.internal.sendBuffer.template, (UDINT)&task.internal.sendBuffer.template.source);
	
}
