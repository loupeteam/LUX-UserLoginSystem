#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// Create Function Prototypes
unsigned long queryCallback(parsedQueryData_typ* data, jsmn_callback_data* callbackData);
unsigned long queryToJson(UDINT* requestUri, UDINT* queryJSON);

void _CYCLIC ProgramCyclic(void)
{
	// ----------------- HTTP SERVER -----------------
	// Call the HTTP Server FUB
	LLHttpServer((UDINT)&task.internal.server);
	
	// ----------------- DEFAULT HTTP RESPONSE -----------------
	task.internal.defaultResponse.ident = task.internal.server.ident;
	// Call the HTTP Response FUB on the defaultResponse
	LLHttpResponse(&task.internal.defaultResponse);
	task.internal.defaultResponse.send = task.internal.defaultResponse.send && !task.internal.defaultResponse.done; // Reset after message is sent
	// Check for new Requests on unexpected uri's
	if(task.internal.defaultResponse.newRequest) {
		// Return a default message and 404 error
		task.internal.defaultResponse.send = 1;		
	}
	
	// ----------------- CUSTOM HTTP RESPONSE -----------------
	task.internal.response.ident = task.internal.server.ident;
	// Call the HTTP Response FUB on the Response
	LLHttpResponse((UDINT)&task.internal.response);
	task.internal.response.send = task.internal.response.send && !task.internal.response.done; // Reset after message is sent
	// Check for new Requests on the desired uri
	if(task.internal.response.newRequest) {
		// Initiate state machine for parsing & accessing login level
		// TODO: Instead of using a cmd. Add the receiedData to a ringbuffer that will be checked for contents in the IDLE state of the stae machine
		task.cmd.authenticateRequest = 1;
	}
	
	// ----------------- AUTHENTICATE INCOMING REQUEST -----------------
	switch(task.status.state) {
		case IDLE:
			// This is where we would check if there is something in the recieve buffer (ie a newRequest has came in)
			// instead of using a cmd
			if(task.cmd.authenticateRequest) {
				task.cmd.authenticateRequest = 0;
				task.status.state = CONVERT_TO_JSON;
			}
			break;
		
		case CONVERT_TO_JSON:
			// ----------------- QUERY PARAMETERS PARSER -----------------
			// Reset queryJson string prior to calling the parser
			memset((UDINT)&task.internal.queryJSON,'\0',sizeof(task.internal.queryJSON));
			// Parse the uri and convert to a json string
			queryToJson(&task.internal.response.requestHeader.uri, &task.internal.queryJSON);
			
			task.status.state = PARSE;
			break;
		
		case PARSE:
			// Re-Initialize the parser object
			task.internal.parser.callback.pFunction = queryCallback;
			task.internal.parser.callback.pUserData = &task.internal.parsedQuery;
			JsmnInit((UDINT)&task.internal.parser);

			// Parse the queryJSON to extract the username and password and set to PV's
			task.internal.parsedQuery.status = JsmnParse((UDINT)&task.internal.parser, (UDINT)&task.internal.queryJSON, brsstrlen((UDINT)&task.internal.queryJSON), (UDINT)&task.internal.tokens, sizeof(task.internal.tokens)/sizeof(task.internal.tokens[0]));
			// Check for parsing errors
			if(task.internal.parsedQuery.status > 0) {
				task.status.state = GET_LOGIN_LVL;
			} 
			else { 
				task.status.state = ERROR;
			}
			
			break;

		case GET_LOGIN_LVL:
			// ----------------- GET USER LEVEL -----------------				
			
			task.internal.MpUser.Login_FB.Login = 1;
			// Convert data. then pass address to FUB
			task.internal.convertStatus = brwcsconv(&task.internal.parsedQuery.convertedData.userName, &task.internal.parsedQuery.data.userName, 0);
			task.internal.convertStatusP = brwcsconv(&task.internal.parsedQuery.convertedData.password, &task.internal.parsedQuery.data.password, 0);
			
			task.internal.MpUser.Login_FB.UserName = &task.internal.parsedQuery.convertedData.userName;
			task.internal.MpUser.Login_FB.Password = &task.internal.parsedQuery.convertedData.password;
			
			// TODO: Fix User not found error - is the string converting to a WSTRING properly? Is there a way to watch this?
			MpUserXLogin(&task.internal.MpUser.Login_FB);
			
			// If the login was succesful - Is this the best way to check for success?
			if(brwcscmp(&task.internal.MpUser.Login_FB.UserName, &task.internal.MpUser.Login_FB.CurrentUser) == 0) {
				task.internal.loginLvl = task.internal.MpUser.Login_FB.CurrentLevel;
				task.status.state = RENDER_RESPONSE;
			}
			else if(task.internal.MpUser.Login_FB.Error) {
				task.internal.loginLvl = 0;
				task.status.state = ERROR;
			}
			
			// TODO: Set up the Log out sequence (Go to new state?)
			// task.internal.MpUser.Login_FB.Login = 0;
			// task.internal.MpUser.Login_FB.Logout = 1;
			
			
			break;
		
		case RENDER_RESPONSE:
			// ----------------- SEND RESPONSE WITH USER LEVEL -----------------	
			// Create JSON String
			task.internal.chopper_status = ChopRender((UDINT)&task.internal.sendBuffer.message, (UDINT)&task.internal.sendBuffer.template, sizeof(task.internal.sendBuffer.message),(UDINT)&task.internal.sendBuffer.messageLength);
			
			// TODO: Make sure Chopper is done rendering (If Error go to Error State)
			// Else {
			// Setup the response
			task.internal.response.pContent = &task.internal.sendBuffer.message;
			task.internal.response.contentLength = strlen(task.internal.sendBuffer.message); 
			task.internal.response.status = LLHTTP_STAT_OK; 
			task.internal.response.send = 1;
			
			task.status.state = LOGOUT;
			
			break;
		
		case ERROR:
			
			task.status.error = 1;
			break;
		
	}

} // End cyclic

