
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	server.enable = 1;
	server.https = 0;
	server.numClients = 4;
	server.port = 1238;
	strcpy(server.ipAddress, "127.0.0.1");
	server.bufferSize = 12000;
	
	response.enable = 1;
	response.method = LLHTTP_METHOD_GET;
	
	strcpy(response.uri, "/getUserLvl");
	strcpy(sendBuffer, "Default Response");
	
	
	strcpy(usernameOperator, "user");
	strcpy(passwordOperator, "test");
	
	strcpy(usernameAdmin, "admin");
	strcpy(passwordAdmin, "123test");
}
