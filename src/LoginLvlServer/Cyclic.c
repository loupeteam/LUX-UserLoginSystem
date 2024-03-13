#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// Create Function Prototypes
unsigned long queryCallback(parsedQueryData_typ* data, jsmn_callback_data* callbackData);
unsigned long queryToJson(UDINT* requestUri, UDINT* queryJSON);

void _CYCLIC ProgramCyclic(void)
{	
	// Set the run status to enter the loop each CPU cycle
	task.status.run = 1;
	while(task.status.run) {
		// ----------------- STATE MACHINE - AUTHENTICATE INCOMING REQUEST -----------------
		switch(task.status.state) {
			case ST_IDLE:
				// Check the buffer for incomming requests
				if(task.internal.receiveBuffer.NumberValues > 0) {
					task.status.state = ST_CONVERT_TO_JSON;
				}
				else {
					// Break the loop (allow FUBs to run)
					task.status.run = 0;
					break;
				}
			// no break
		
			case ST_CONVERT_TO_JSON:
				// Reset queryJson string prior to calling the parser
				memset((UDINT)&task.internal.queryJSON,'\0',sizeof(task.internal.queryJSON));
			
				// Get the address of the top of the Buffer
				task.internal.pTopReceiveBuffer = BufferGetItemAdr((UDINT)&task.internal.receiveBuffer,0,&task.internal.ReceiveBufferStatus);
				// Check that the data is valid
				if(task.internal.pTopReceiveBuffer != 0) {
					// Parse the uri and convert to a json string
					queryToJson(task.internal.pTopReceiveBuffer, &task.internal.queryJSON);
					// Pop Internal Buffer
					BufferRemoveTop((UDINT)&task.internal.receiveBuffer);
					
					task.status.state = ST_PARSE;
				}
				else {
					// No data in the top of the buffer; Move to an ERROR state
					task.status.error = 1;
					task.status.state = ST_ERROR_RESPONSE;
					break;
				}
			
			// no break
		
			case ST_PARSE:
				// Re-Initialize the parser object
				task.internal.parser.callback.pFunction = queryCallback;
				task.internal.parser.callback.pUserData = &task.internal.parsedQuery;
				JsmnInit((UDINT)&task.internal.parser);

				// Parse the queryJSON to extract the username and password and set to PV's
				task.internal.parsedQuery.status = JsmnParse((UDINT)&task.internal.parser, (UDINT)&task.internal.queryJSON, brsstrlen((UDINT)&task.internal.queryJSON), (UDINT)&task.internal.tokens, sizeof(task.internal.tokens)/sizeof(task.internal.tokens[0]));
				// Check for parsing errors
				if(task.internal.parsedQuery.status > 0) {
					// Convert json strings to wide strings for use in the Function Block
					brwcsconv(&task.internal.parsedQuery.convertedData.userName, &task.internal.parsedQuery.data.userName, 0);
					brwcsconv(&task.internal.parsedQuery.convertedData.password, &task.internal.parsedQuery.data.password, 0);
				
					task.status.state = ST_LOGIN;
				} 
				else {
					// No parsed data output from Jsmn
					task.status.error = 1;
					task.status.state = ST_ERROR_RESPONSE;
					break;
				}
			
			// no break

			case ST_LOGIN:
				// Set up FUB inputs
				task.internal.MpUser.Login_FB.Login = 1;
				
				// Check if the login was successful
				if(task.internal.MpUser.Login_FB.CommandDone) {
					// ----------------- GET USER LEVEL -----------------	
					// Set the login Level from the User information
					task.internal.loginLvl = task.internal.MpUser.Login_FB.CurrentLevel;
					task.status.state = ST_RENDER_RESPONSE;
				}
				else if(task.internal.MpUser.Login_FB.Error) {		
					// Reset the login Level
					task.internal.loginLvl = 0;
					task.status.state = ST_ERROR_RESPONSE;
					break;
				}			
				else if(!task.internal.MpUser.Login_FB.CommandBusy) {
					// Set up Pointers to Username & password
					task.internal.MpUser.Login_FB.UserName = &task.internal.parsedQuery.convertedData.userName;
					task.internal.MpUser.Login_FB.Password = &task.internal.parsedQuery.convertedData.password;
					break;
				}
				else { 
					// MpUser.Login_FB is busy and needs another cycle (Break the loop)
					task.status.run = 0;
					break;
				}
			
			// no break
				
			case ST_RENDER_RESPONSE:
				// Create JSON String
				task.internal.chopper_status = ChopRender((UDINT)&task.internal.sendBuffer.message, (UDINT)&task.internal.sendBuffer.template, sizeof(task.internal.sendBuffer.message),(UDINT)&task.internal.sendBuffer.messageLength);
			
				// Check for errors from Chopper
				if(task.internal.chopper_status != 0) {
					task.status.state = ST_ERROR_RESPONSE;
					break;
				}
				else {
					task.status.state = ST_SEND_RESPONSE;
				}
			
			// no break
		
			case ST_SEND_RESPONSE:
			
				// Setup the response
				task.internal.response.pContent = &task.internal.sendBuffer.message;
				task.internal.response.contentLength = strlen(task.internal.sendBuffer.message); 
				task.internal.response.status = LLHTTP_STAT_OK; 
				task.internal.response.send = 1;
				
				// Set up FUB inputs here so the check values within the next state are not stale
				task.internal.MpUser.Login_FB.Logout = 1;
				task.status.state = ST_LOGOUT;	
				// Break the state machine to allow the MpUser FUB to run again before making checks in ST_LOGOUT
				break; 
		
			case ST_LOGOUT:
					
				if(task.internal.MpUser.Login_FB.CommandDone) {
					task.internal.MpUser.Login_FB.Logout = 0;	
					task.status.state = ST_IDLE;
					break;
				}
				else if(task.internal.MpUser.Login_FB.Error) {
					task.status.state = ST_IDLE;
				}
				else if(!task.internal.MpUser.Login_FB.CommandBusy) {
					// Set up FUB inputs
					task.internal.MpUser.Login_FB.Logout = 1;
					break;
				}
				else { 
					// MpUser.Login_FB is busy and needs another cycle (Break the loop)
					task.status.run = 0;
					break;
				}
				
			// no break

			case ST_ERROR_RESPONSE:
			
				// Setup the error response
				strcpy(&task.internal.sendBuffer.message, "Login Error");
				task.internal.response.pContent = &task.internal.sendBuffer.message;
				task.internal.response.contentLength = strlen(task.internal.sendBuffer.message); 
				task.internal.response.status = LLHTTP_STAT_Unauthorized; 
				task.internal.response.send = 1;

				task.status.state = ST_IDLE;
				task.status.run = 0;
			
			// no break
	
		} // End State Machine
		
		// ----------------- MAPP USER SYSTEM -----------------
		// Set ErrorReset Command
		if(task.internal.MpUser.Login_FB.Error) {
			task.internal.MpUser.Login_FB.ErrorReset = 1;
		}
		// Call the MpUserXLogin FUB
		MpUserXLogin(&task.internal.MpUser.Login_FB);
		// Reset FUB inputs
		task.internal.MpUser.Login_FB.Login = 0;
		task.internal.MpUser.Login_FB.Logout = 0;
		task.internal.MpUser.Login_FB.ErrorReset = 0;
	
	} // End while(task.status.run) loop
	
	
	// ----------------- CALL FUNCTION BLOCKS -----------------
	// Call the HTTP Server FUB
	LLHttpServer((UDINT)&task.internal.server);
		
	// ----------------- DEFAULT HTTP RESPONSE -----------------
	task.internal.defaultResponse.ident = task.internal.server.ident;
	task.internal.defaultResponse.send = task.internal.defaultResponse.send && !task.internal.defaultResponse.done; // Reset after message is sent
	// Call the HTTP Response FUB on the defaultResponse
	LLHttpResponse(&task.internal.defaultResponse);
	// Check for new Requests on unexpected uri's
	if(task.internal.defaultResponse.newRequest) {
		// Return a default message and 404 error (set in init)
		task.internal.defaultResponse.send = 1;		
	}
	
	// ----------------- CUSTOM HTTP RESPONSE -----------------
	task.internal.response.ident = task.internal.server.ident;
	task.internal.response.send = task.internal.response.send && !task.internal.response.done; // Reset after message is sent
	// Call the HTTP Response FUB on the Response
	LLHttpResponse((UDINT)&task.internal.response);
	task.internal.response.send = 0;
	// Check for new Requests on the desired uri
	if(task.internal.response.newRequest) {
		// Add Request uri (with parameters) to an internall buffer for authentication later
		BufferAddToBottom((UDINT)&task.internal.receiveBuffer,(UDINT)&task.internal.response.requestHeader.uri);
	}
		
	
} // End cyclic

