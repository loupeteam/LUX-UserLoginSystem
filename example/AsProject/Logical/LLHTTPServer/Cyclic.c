\
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


// Create Function Prototypes
unsigned long getCallback(parsedQueryData_typ* data, jsmn_callback_data* callbackData);
unsigned long parseQuery(UDINT* requestUri, UDINT* queryJSON);

void _CYCLIC ProgramCyclic(void)
{
			
	/*
	* This section contains the Function Block calls.
	* These calls are ran outside of the state-machine
	* to ensure proper updating for each PLC scan. 
	*/
	
	// Call HTTP Server & Request Function Blocks
	LLHttpServer((UDINT)&task.internal.server);


	// Set up default server/response
	task.internal.defaultResponse.enable = 1;
	task.internal.defaultResponse.ident = task.internal.server.ident;
	task.internal.defaultResponse.pRequest = (UDINT)&task.internal.receiveBufferNotFound;
	task.internal.defaultResponse.requestSize = sizeof(task.internal.receiveBufferNotFound);
	LLHttpResponse(&task.internal.defaultResponse);
	task.internal.defaultResponse.send = task.internal.defaultResponse.send && !task.internal.defaultResponse.done; // Reset after message is sent

	if(task.internal.defaultResponse.newRequest) {
		task.internal.defaultResponse.pContent = sendBuffer;
		task.internal.defaultResponse.contentLength = strlen(sendBuffer);
		task.internal.defaultResponse.send = 1;
		task.internal.defaultResponse.status = LLHTTP_STAT_OK;
	}
	
	// Set up server/response for new request
	task.internal.response.ident = task.internal.server.ident;
//	task.internal.response.pRequest = (UDINT)&task.internal.receiveBuffer;
//	task.internal.response.requestSize = sizeof(task.internal.receiveBuffer);
	task.internal.response.pContent = (UDINT)&task.internal.sendBuffer.message;
	task.internal.response.contentLength = strlen(task.internal.sendBuffer.message);
	LLHttpResponse((UDINT)&task.internal.response);
	task.internal.response.send = task.internal.response.send && !task.internal.response.done; // Reset after message is sent
	
	if(task.internal.response.newRequest) {
		
		
		
		parseQuery(&task.internal.response.requestHeader.uri, &task.internal.queryJSON);
		
		
		// Re-Initialize the parser object
		//		task.internal.parser.callback.pFunction = getCallback;
		//		task.internal.parser.callback.pUserData = &task.internal.parsedBody;
		//		JsmnInit((UDINT)&task.internal.parser);

		// Parse the queryJSON
		//task.internal.parsedQuery.status = JsmnParse((UDINT)&task.internal.parser, (UDINT)&task.internal.receiveBuffer, brsstrlen((UDINT)&task.internal.receiveBuffer), (UDINT)&task.internal.tokens, sizeof(task.internal.tokens)/sizeof(task.internal.tokens[0]));
		
		// Set User Level - using Parsed Body
		task.internal.userLvl = LOGGED_OUT;
	
		// Create JSON String
		ChopRender((UDINT)&task.internal.sendBuffer.message, (UDINT)&task.internal.sendBuffer.template, sizeof(task.internal.sendBuffer.message),(UDINT)&task.internal.sendBuffer.messageLength);
	
		// Send a response
		task.internal.response.send = task.internal.response.send && !task.internal.response.done; // Reset after message is sent

		task.internal.response.pContent = task.internal.sendBuffer.message;
		task.internal.response.contentLength = strlen(task.internal.sendBuffer.message);
		task.internal.response.send = 1;
		task.internal.response.status = LLHTTP_STAT_OK;
		
	}

}

