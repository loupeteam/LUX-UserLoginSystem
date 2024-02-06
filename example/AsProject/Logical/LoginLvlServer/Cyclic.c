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
		// Return a default message and 404 error (set in init)
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
		// other option: if this is already true respond with "Server busy try again later" 503
	}
	
	// ----------------- MAPP USER SYSTEM -----------------
	// Call the MpUserXLogin FUB
	MpUserXLogin(&task.internal.MpUser.Login_FB);
		
	
	// ----------------- STATE MACHINE - AUTHENTICATE INCOMING REQUEST -----------------
	switch(task.status.state) {
		case ST_IDLE:
			}
			break;
		
		case ST_CONVERT_TO_JSON:
			// ----------------- QUERY PARAMETERS PARSER -----------------
			// Reset queryJson string prior to calling the parser
			memset((UDINT)&task.internal.queryJSON,'\0',sizeof(task.internal.queryJSON));
				task.status.state = ST_PARSE;
				task.status.state = ST_LOGIN_ERROR;
			break;
		
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
				task.status.state = ST_LOGIN_ERROR;
			}
			
			break;

		case ST_LOGIN:
						
			// Set up FUB inputs
			task.internal.MpUser.Login_FB.Login = 1;
			task.internal.MpUser.Login_FB.Logout = 0;
			task.internal.MpUser.Login_FB.ErrorReset = 0;
			// Set up Pointers to Username & password
			task.internal.MpUser.Login_FB.UserName = &task.internal.parsedQuery.convertedData.userName;
			task.internal.MpUser.Login_FB.Password = &task.internal.parsedQuery.convertedData.password;

			// Check if the login was successful
			if(task.internal.MpUser.Login_FB.CommandDone && !task.internal.MpUser.Login_FB.CommandBusy) {
				// ----------------- GET USER LEVEL -----------------	
				// Set the login Level from the User information
				task.internal.loginLvl = task.internal.MpUser.Login_FB.CurrentLevel;
				
				task.status.state = ST_RENDER_RESPONSE;
			}
			else if(task.internal.MpUser.Login_FB.Error) {		
				// Reset the login Level
				task.internal.loginLvl = 0;
				
				task.status.state = ST_LOGIN_ERROR;
			}			
			
			break;
				
		case ST_RENDER_RESPONSE:
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
			
			task.status.state = ST_LOGOUT;
			
			break;
		
		case ST_LOGOUT:
			
			// Set up FUB inputs
			task.internal.MpUser.Login_FB.Login = 0;
			task.internal.MpUser.Login_FB.Logout = 1;
			task.internal.MpUser.Login_FB.ErrorReset = 0;
			
			// If the logout was successful
			if(task.internal.MpUser.Login_FB.CommandDone && !task.internal.MpUser.Login_FB.CommandBusy) {
				task.status.state = ST_IDLE;
			}
			else if(task.internal.MpUser.Login_FB.Error) {
				task.status.state = ST_LOGOUT_ERROR;
			}
			
			break;

		case ST_LOGIN_ERROR:
			
			if(task.internal.MpUser.Login_FB.StatusID == -1064144896) { // Invalid Password - TODO: This is a const use the enum name
				// Setup the response
				strcpy(&task.internal.sendBuffer.message, "Invalid Password. Try again");
				task.internal.response.pContent = &task.internal.sendBuffer.message;
				task.internal.response.contentLength = strlen(task.internal.sendBuffer.message); 
			}
			task.internal.response.status = LLHTTP_STAT_Unauthorized; 
			task.internal.response.send = 1;
			
			// Set ErrorReset Command
//			task.internal.MpUser.Login_FB.ErrorReset = 1;
			if(task.internal.MpUser.Login_FB.Error) {
				task.internal.MpUser.Login_FB.ErrorReset = 1;
				task.internal.MpUser.Login_FB.Login = 0;
			}
			else {
				task.internal.MpUser.Login_FB.ErrorReset = 0;
				task.status.state = ST_IDLE;
			}
			
			break;
		
		case ST_LOGOUT_ERROR:
			
			// Set ErrorReset Command
//			task.internal.MpUser.Login_FB.ErrorReset = 1;
			if(task.internal.MpUser.Login_FB.Error) {
				task.internal.MpUser.Login_FB.ErrorReset = 1;
				task.internal.MpUser.Login_FB.Logout = 0;
			}
			else {
				task.internal.MpUser.Login_FB.ErrorReset = 0;
				task.status.state = ST_IDLE;
			}
			
			break;
		
		// TODO:Could create a send error msg state or an error reset state. Would limit copied code from login and logout errors (would also allow for other errors)
		
	}
	
	
	
} // End cyclic

