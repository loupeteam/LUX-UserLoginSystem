\
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


// Create Function Prototypes
unsigned long queryCallback(parsedQueryData_typ* data, jsmn_callback_data* callbackData);
unsigned long queryToJson(UDINT* requestUri, UDINT* queryJSON);

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
	task.internal.defaultResponse.pRequest = (UDINT)&task.internal.defaultReceiveBuffer;
	task.internal.defaultResponse.requestSize = sizeof(task.internal.defaultReceiveBuffer);
	LLHttpResponse(&task.internal.defaultResponse);
	task.internal.defaultResponse.send = task.internal.defaultResponse.send && !task.internal.defaultResponse.done; // Reset after message is sent

	if(task.internal.defaultResponse.newRequest) {
		task.internal.defaultResponse.pContent = (UDINT)&task.internal.defaultSendBuffer;
		task.internal.defaultResponse.contentLength = strlen(task.internal.defaultSendBuffer);
		task.internal.defaultResponse.send = 1;
		task.internal.defaultResponse.status = LLHTTP_STAT_NotFound;
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
		
		// Reset queryJson string prior to calling the parser
		memset((UDINT)&task.internal.queryJSON,'\0',sizeof(task.internal.queryJSON));
		// Parse the uri and convert to a json string
		queryToJson(&task.internal.response.requestHeader.uri, &task.internal.queryJSON);
		
		
		// Re-Initialize the parser object
		task.internal.parser.callback.pFunction = queryCallback;
		task.internal.parser.callback.pUserData = &task.internal.parsedQuery;
		JsmnInit((UDINT)&task.internal.parser);

		// Parse the queryJSON to extract the username and password
		task.internal.parsedQuery.status = JsmnParse((UDINT)&task.internal.parser, (UDINT)&task.internal.queryJSON, brsstrlen((UDINT)&task.internal.queryJSON), (UDINT)&task.internal.tokens, sizeof(task.internal.tokens)/sizeof(task.internal.tokens[0]));
		
		
		
		
		// Set User Level - using Parsed Query
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

