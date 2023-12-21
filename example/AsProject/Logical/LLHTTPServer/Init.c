
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
	task.internal.response.method = LLHTTP_METHOD_GET;
	strcpy(task.internal.response.uri, "/getUserLvl");
	strcpy(task.internal.sendBuffer.message, "Default Response");
	
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
	
	// Initialize & complie the Chopper template 
	strcpy(task.internal.sendBuffer.template.source, "{\"userLevel\":\"{{LLHTTPServ:task.internal.userLvl}}\" }");
	ChopCompile((UDINT)&task.internal.sendBuffer.template, (UDINT)&task.internal.sendBuffer.template.source);
}
