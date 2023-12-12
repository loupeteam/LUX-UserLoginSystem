
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _CYCLIC ProgramCyclic(void)
{
	// Set up server/response
	response.ident = server.ident;
	response.pRequest = &receiveBuffer;
	response.requestSize = sizeof(receiveBuffer);
	response.pContent = &sendBuffer;
	response.contentLength = strlen(sendBuffer);
	
	//	for(i = 0; i < 25; i++) {
	//		if(!strcmp(response.requestHeader.lines.lines[i].name,"user")) {
	//			strcpy(sendBuffer,response.requestHeader.lines.lines[i].name)
	//		}
	//	}
	isUser = LLHttpHeaderContains(&response.requestHeader.lines,&usernameOperator, &passwordOperator);
	isAdmin = LLHttpHeaderContains(&response.requestHeader.lines,&usernameAdmin, &passwordAdmin);
	
	if(isAdmin) {
		userLvl = ADMIN;
	}
	else if(isUser && !isAdmin) {
		userLvl = OPERATOR;	
	}
	else {
		userLvl = LOGGED_OUT;	
	}
	
//	strcpy(sendBuffer,itoa(userLvl));
	strcpy(sendBuffer,"DID IT");
	
	response.send = response.send && !response.done; // Reset after message is sent

	if(response.newRequest) {
		response.pContent = sendBuffer;
		response.contentLength = strlen(sendBuffer);
		response.send = 1;
		response.status = LLHTTP_STAT_OK;
	}
	
	// Call FUBs
	LLHttpServer(&server);
	LLHttpResponse(&response);
}

