\
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


// Create Function Prototypes
unsigned long getCallback(parsedBodyData_typ* data, jsmn_callback_data* callbackData);

void _CYCLIC ProgramCyclic(void)
{
			
	// Set up server/response
	task.internal.response.ident = task.internal.server.ident;
	task.internal.response.pRequest = (UDINT)&task.internal.receiveBuffer;
	task.internal.response.requestSize = sizeof(task.internal.receiveBuffer);
	task.internal.response.pContent = (UDINT)&task.internal.sendBuffer.message;
	task.internal.response.contentLength = strlen(task.internal.sendBuffer.message);
	
//	// Parse the response
//	task.internal.parsedBody.status = JsmnParse((UDINT)&task.internal.parser, (UDINT)&task.internal.receiveBuffer, brsstrlen((UDINT)&task.internal.receiveBuffer), (UDINT)&task.internal.tokens, sizeof(task.internal.tokens)/sizeof(task.internal.tokens[0]));
//	// Re-Initialize the parser object
//	JsmnInit((UDINT)&task.internal.parser);
//	
//	// Set User Level - using Parsed Body
//	if(strcmp(task.internal.parsedBody.data.username, task.internal.admin.username) == 0) {
//		if(strcmp(task.internal.parsedBody.data.password, task.internal.admin.password) == 0) {
//			task.internal.userLvl = ADMIN;
//		}
//	}				
//	else if(strcmp(task.internal.parsedBody.data.username, task.internal.user.username) == 0) {
//		if(strcmp(task.internal.parsedBody.data.password, task.internal.user.password) == 0) {
//			task.internal.userLvl = USER;
//		}
//	}
//	else {
//		task.internal.userLvl = LOGGED_OUT;	
//	}
	
	// Set User Level - Using Headers
	task.internal.isUser = LLHttpHeaderContains((UDINT)&task.internal.response.requestHeader.lines,(UDINT)&task.internal.user.username, (UDINT)&task.internal.user.password);
	task.internal.isAdmin = LLHttpHeaderContains((UDINT)&task.internal.response.requestHeader.lines,(UDINT)&task.internal.admin.username, (UDINT)&task.internal.admin.password);

	if(task.internal.isAdmin) {
		task.internal.userLvl = ADMIN;
	}
	else if(task.internal.isUser && !task.internal.isAdmin) {
		task.internal.userLvl = USER;	
	}
	else {
		task.internal.userLvl = LOGGED_OUT;	
	}
	
	// Create JSON String
	ChopRender((UDINT)&task.internal.sendBuffer.message, (UDINT)&task.internal.sendBuffer.template, sizeof(task.internal.sendBuffer.message),(UDINT)&task.internal.sendBuffer.messageLength);
	
	// Send a response
	task.internal.response.send = task.internal.response.send && !task.internal.response.done; // Reset after message is sent
	
	if(task.internal.response.newRequest) {
		task.internal.response.pContent = task.internal.sendBuffer.message;
		task.internal.response.contentLength = strlen(task.internal.sendBuffer.message);
		task.internal.response.send = 1;
		task.internal.response.status = LLHTTP_STAT_OK;
	}
	

			


	
	/*
	* This section contains the Function Block calls.
	* These calls are ran outside of the state-machine
	* to ensure proper updating for each PLC scan. 
	*/
	
	// Call HTTP Server & Request Function Blocks
	LLHttpServer((UDINT)&task.internal.server);
	LLHttpResponse((UDINT)&task.internal.response);
}

