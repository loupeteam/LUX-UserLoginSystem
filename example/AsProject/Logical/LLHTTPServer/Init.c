
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	/*
	* This file is used to set up the unchanging or initial values needed 
	* in the UserLoginServer cyclic function.
	*/
	
	// Set up the HTTP Server
	task.internal.server.enable = 1;
	task.internal.server.https = 0;
	task.internal.server.numClients = 4;
	task.internal.server.port = 1238;
	strcpy(task.internal.server.ipAddress, "127.0.0.1");
	task.internal.server.bufferSize = 12000;
	
	// Set up the HTTP Response
	task.internal.response.enable = 1;
	task.internal.response.method = LLHTTP_METHOD_ANY;
	strcpy(task.internal.response.uri, "/getUserLvl");
	strcpy(task.internal.sendBuffer.message, "Default Response");
	task.internal.response.pUserHeader = &task.internal.responseHeader.lines;
	task.internal.response.numUserHeaders = sizeof(task.internal.responseHeader.lines)/sizeof(task.internal.responseHeader.lines[0]);
	LLHttpAddHeaderField(&task.internal.responseHeader.lines,26,"Access-Control-Allow-Origin","*");

	
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
	
	// Set app values (from Configuration)
	if(useConfig) {
		strcpy(task.internal.user.username, Configuration.user.username);
		strcpy(task.internal.user.password, Configuration.user.password);
		strcpy(task.internal.admin.username, Configuration.admin.username);
		strcpy(task.internal.admin.password, Configuration.admin.password);
	} else {
		strcpy(task.internal.user.username, "user");
		strcpy(task.internal.user.password, "test");
		strcpy(task.internal.admin.username, "admin");
		strcpy(task.internal.admin.password, "123test");
	}
	
	// Initialize parser values to default
	JsmnInit((UDINT)&task.internal.parser);
	strcpy((UDINT)&task.internal.queryJSON, "");
	
//	// Initialize & complie the Chopper template 
	strcpy(task.internal.sendBuffer.template.source, "{\"userLevel\":\"{{LLHTTPServ:task.internal.userLvl}}\" }");
	ChopCompile((UDINT)&task.internal.sendBuffer.template, (UDINT)&task.internal.sendBuffer.template.source);
}
