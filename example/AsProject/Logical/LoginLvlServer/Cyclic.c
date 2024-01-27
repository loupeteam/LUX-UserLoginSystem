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
	// A default Response is used for HTTP requests that are NOT to desired/expected uri's
	// Set up default Response
	task.internal.defaultResponse.enable = 1;
	task.internal.defaultResponse.ident = task.internal.server.ident;
	task.internal.defaultResponse.pRequest = (UDINT)&task.internal.defaultReceiveBuffer;
	task.internal.defaultResponse.requestSize = sizeof(task.internal.defaultReceiveBuffer);
	// Call the HTTP Response FUB on the defaultResponse
	LLHttpResponse(&task.internal.defaultResponse);
	task.internal.defaultResponse.send = task.internal.defaultResponse.send && !task.internal.defaultResponse.done; // Reset after message is sent
	// Check for new Requests on unexpected uri's
	if(task.internal.defaultResponse.newRequest) {
		// Return a default message and 404 error
		task.internal.defaultResponse.pContent = (UDINT)&task.internal.defaultSendBuffer;
		task.internal.defaultResponse.contentLength = strlen(task.internal.defaultSendBuffer);
		task.internal.defaultResponse.send = 1;
		task.internal.defaultResponse.status = LLHTTP_STAT_NotFound;
	}
	
	// ----------------- CUSTOM HTTP RESPONSE -----------------
	// A response for HTTP requests that ARE to the desired/expected uri
	// Set up Response
	task.internal.response.ident = task.internal.server.ident;
	task.internal.response.pContent = (UDINT)&task.internal.sendBuffer.message;
	task.internal.response.contentLength = strlen(task.internal.sendBuffer.message);
	// Call the HTTP Response FUB on the Response
	LLHttpResponse((UDINT)&task.internal.response);
	task.internal.response.send = task.internal.response.send && !task.internal.response.done; // Reset after message is sent
	
	// Check for new Requests on the desired uri
	if(task.internal.response.newRequest) { 

		// ----------------- QUERY PARAMETERS PARSER -----------------
		// Reset queryJson string prior to calling the parser
		memset((UDINT)&task.internal.queryJSON,'\0',sizeof(task.internal.queryJSON));
		// Parse the uri and convert to a json string
		queryToJson(&task.internal.response.requestHeader.uri, &task.internal.queryJSON);
		
		// Re-Initialize the parser object
		task.internal.parser.callback.pFunction = queryCallback;
		task.internal.parser.callback.pUserData = &task.internal.parsedQuery;
		JsmnInit((UDINT)&task.internal.parser);

		// Parse the queryJSON to extract the username and password and set to PV's
		task.internal.parsedQuery.status = JsmnParse((UDINT)&task.internal.parser, (UDINT)&task.internal.queryJSON, brsstrlen((UDINT)&task.internal.queryJSON), (UDINT)&task.internal.tokens, sizeof(task.internal.tokens)/sizeof(task.internal.tokens[0]));
		
		// Set command to authenticate & process newRequest
		task.cmd.authRequest = 1;
	}
	
	// Authenticate & Process New Request
	if (task.cmd.authRequest) {
		
		// ----------------- AUTHENTICATE USER -----------------
		// Set execute command
		task.internal.ArUser.AuthenticatePassword_FB.Execute = 1;
		// Copy the parsed data to the FUB
		memcpy(&task.internal.ArUser.AuthenticatePassword_FB.UserName, &task.internal.parsedQuery.data.userName, strlen(task.internal.parsedQuery.data.userName));
		memcpy(&task.internal.ArUser.AuthenticatePassword_FB.Password, &task.internal.parsedQuery.data.password, strlen(task.internal.parsedQuery.data.password));
		// Call the FUB
		ArUserAuthenticatePassword(&task.internal.ArUser.AuthenticatePassword_FB);
	
		// Check that the FUB is complete
		if (task.internal.ArUser.AuthenticatePassword_FB.Done && !task.internal.ArUser.AuthenticatePassword_FB.Busy) {
			//Stop FUB execution on completion
			task.internal.ArUser.AuthenticatePassword_FB.Execute = 0;
		}
		else if (task.internal.ArUser.AuthenticatePassword_FB.Error) {
			// Check if the error is due to a non-user
			if(task.internal.ArUser.AuthenticatePassword_FB.ErrorID == arUSER_ERR_DOES_NOT_EXIST) {
				// No system error
				task.internal.ArUser.AuthenticatePassword_FB.Execute = 0;
			}
			else {
				task.status.error = 1;
			}
		}
		
		// ----------------- GET USER ROLE -----------------
		if(task.internal.ArUser.AuthenticatePassword_FB.IsAuthentic) { 
			task.status.isAuth = 1;
			
			// --- Create a list of the roles for this user ---
			
			// Copy the parsed data & desired property to the FUB
			task.internal.ArUser.InitList_FB.ListType = arUSER_USERROLES;
			memcpy(&task.internal.ArUser.InitList_FB.ElementName, &task.internal.parsedQuery.data.userName, strlen(task.internal.parsedQuery.data.userName));
			// Set execute command
			task.internal.ArUser.InitList_FB.Execute = 1;
			// Call the FUB
			ArUserInitList(&task.internal.ArUser.InitList_FB);
			// Check that the FUB is complete
			if (task.internal.ArUser.InitList_FB.Done && !task.internal.ArUser.InitList_FB.Busy) {
				//Stop FUB execution on completion
				task.internal.ArUser.InitList_FB.Execute = 0;
			}
			else if (task.internal.ArUser.InitList_FB.Error) {
				// Check if the error is due to a non-user
				if(task.internal.ArUser.InitList_FB.ErrorID == arUSER_ERR_DOES_NOT_EXIST) {
					// No system error
					task.internal.ArUser.InitList_FB.Execute = 0;
				}
				else {
					task.status.error = 1;
				}
			}
			
			// --- Get the first element of the roles list ---
			
			// Copy the parsed data & desired property to the FUB
			task.internal.ArUser.GetFirst_FB.List = task.internal.ArUser.InitList_FB.List;
			// Set execute command
			task.internal.ArUser.GetFirst_FB.Execute = 1;
			// Call the FUB
			ArUserGetFirst(&task.internal.ArUser.GetFirst_FB);
			// Check that the FUB is complete
			if (task.internal.ArUser.GetFirst_FB.Done) {
				//Stop FUB execution on completion
				task.internal.ArUser.GetFirst_FB.Execute = 0;
				// Set the loginLvl to the value of the Role
				strcpy(&task.internal.loginLvl, &task.internal.ArUser.GetFirst_FB.ElementName);
			}
			else if (task.internal.ArUser.GetFirst_FB.Error) {
				// Check if the error is due to the end of the list
				if(task.internal.ArUser.GetFirst_FB.ErrorID == arUSER_WRN_END_OF_LIST) {
					// No system error
					task.internal.ArUser.GetFirst_FB.Execute = 0;
				}
				else {
					task.status.error = 1;
				}
			}
			
			// TODO: DestroyList after done
			
			//			// --- Get the role property (GetProperty) ---
			//			/* This isn't working... Results in: 
			//				arUSER_ERR_DOES_NOT_EXIST	-1070585892
			//				The element (user, role, user-role assignment) passed to the 
			//				function block as a parameter does not exist.
			//				- 
			//				I think the Property ID would be "Role[1]" However it is encasulated in a 
			//				Group ID of "Roles" so I cannout get it to output anything. 
			//				If I replace "Role[1]" with "UserID" (which is not within a parent group)
			//				it yeilds the expected output.
			//			*/
			//
			//			// Copy the parsed data & desired property to the FUB
			//			task.internal.ArUser.GetProperty_FB.ElementType = arUSER_USER_PROPERTIES;
			//			memcpy(&task.internal.ArUser.GetProperty_FB.ElementName, &task.internal.parsedQuery.data.userName, strlen(task.internal.parsedQuery.data.userName));
			//			memcpy(&task.internal.ArUser.GetProperty_FB.PropertyName, &task.internal.ArUser.PropertyName, strlen(task.internal.ArUser.PropertyName)); // PropertyName is set in the init
			//			// Set execute command
			//			task.internal.ArUser.GetProperty_FB.Execute = 1;
			//			// Call the FUB
			//			ArUserGetProperty(&task.internal.ArUser.GetProperty_FB);
			//			
			//			// Check that the FUB is complete
			//			if (task.internal.ArUser.GetProperty_FB.Done && !task.internal.ArUser.GetProperty_FB.Busy) {
			//				//Stop FUB execution on completion
			//				task.internal.ArUser.GetProperty_FB.Execute = 0;
			//				strcpy(&task.internal.loginLvl, &task.internal.ArUser.GetProperty_FB.PropertyValue);
			//			}
			//			else if (task.internal.ArUser.GetProperty_FB.Error) {
			//				// Check if the error is due to a non-user
			//				if(task.internal.ArUser.GetProperty_FB.ErrorID == arUSER_ERR_DOES_NOT_EXIST) {
			//					// No system error
			//					task.internal.ArUser.GetProperty_FB.Execute = 0;
			//				}
			//				else {
			//					task.status.error = 1;
			//				}
			//			}
			
		}
		
		
		
		// ----------------- EXPORT USERS (FOR DEBUG) -----------------
		if(task.cmd.exportUsers) {
			
			// Export List of user to File for debugging
			memcpy(&task.internal.ArUser.Export_FB.FilePath, &task.internal.ArUser.FilePath, strlen(task.internal.ArUser.FilePath));
			task.internal.ArUser.Export_FB.Execute = 1;
			// Run the Export FUB
			ArUserExport(&task.internal.ArUser.Export_FB);
			// Check that the FUB is complete
			if (task.internal.ArUser.Export_FB.Done && !task.internal.ArUser.Export_FB.Busy) {
				task.internal.ArUser.Export_FB.Execute = 0;
				task.cmd.exportUsers = 0;
			} else if (task.internal.ArUser.Export_FB.Error) {
				task.status.error = 1;
			}
			// Reset the command
			task.cmd.exportUsers = 0;
		}	
		
		// ----------------- SEND RESPONSE WITH USER LEVEL -----------------	
		// Create JSON String
		ChopRender((UDINT)&task.internal.sendBuffer.message, (UDINT)&task.internal.sendBuffer.template, sizeof(task.internal.sendBuffer.message),(UDINT)&task.internal.sendBuffer.messageLength);
		// Send the response
		task.internal.response.send = task.internal.response.send && !task.internal.response.done; // Reset after message is sent
		task.internal.response.pContent = task.internal.sendBuffer.message;
		task.internal.response.contentLength = strlen(task.internal.sendBuffer.message);
		task.internal.response.send = 1;
		task.internal.response.status = LLHTTP_STAT_OK;
		
		// Reset the command
		if (task.internal.response.done) { // is this the right check var???
			task.cmd.authRequest = 0;
		}
	}		
	

} // End cyclic

