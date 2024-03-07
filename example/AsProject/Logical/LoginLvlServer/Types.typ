TYPE
	Task_typ : 	STRUCT 
		cmd : Task_Command_typ;
		status : Task_Status_typ;
		internal : Task_Internal_typ;
	END_STRUCT;
END_TYPE

(* ----- Main Task Sturcture ----- *)
TYPE
	Task_Command_typ : 	STRUCT 
		authenticateRequest : BOOL;

	END_STRUCT;
	
	Task_Status_typ : 	STRUCT 
		error : BOOL;
		run : BOOL;
		state : state_enum;
	END_STRUCT;
	
	Task_Internal_typ : 	STRUCT  (* Information not to be accessed outside this task *)
		(*Web Comms*)
		server : LLHttpServer;
		response : LLHttpResponse;
		responseHeader : LLHttpHeader_typ;
		defaultResponse : LLHttpResponse;		
		defaultReceiveBuffer : STRING[8000];
		defaultSendBuffer : STRING[8000];
		
		(*JSON Parser*)
		parser : jsmn_parser;
		tokens : ARRAY[0..15]OF jsmntok_t;
		queryJSON : STRING[500];
		parsedQuery : parsedQuery_typ; 
		chopper_status : DINT;
		
		(*User Level Application Vars*)
		sendBuffer : sendBuffer_typ;
		loginLvl : DINT;
		MpUser : MpUser_typ;
		
		(* Receive Buffer *)
		receiveBuffer : Buffer_typ;
		ReceiveBufferStatus : UINT;
		pTopReceiveBuffer : UDINT;
		
	END_STRUCT;
	
END_TYPE

(* ----- User Types ----- *)
TYPE
	
	MpUser_typ : 	STRUCT 		
		
		Login_FB : MpUserXLogin;
		
	END_STRUCT;
	
END_TYPE

(* ----- JSON Types ----- *)
TYPE
	parsedQuery_typ : 	STRUCT 
		data : parsedQueryData_typ; (* username & password stored as strings - returned from JSMN Parse *)
		convertedData : convertedData_typ; (* username & password stored as WIDE strings - converted using brwcsconv *)
		status : INT;
	END_STRUCT;
	parsedQueryData_typ : 	STRUCT 
		userName : STRING[50];
		password : STRING[50];
	END_STRUCT;
	convertedData_typ : 	STRUCT 
		userName : WSTRING[50];
		password : WSTRING[50];
	END_STRUCT;
	sendBuffer_typ : 	STRUCT 
		template : Chop_Template_typ;
		message : STRING[8000]; (* The complied string returned from Chopper - to use in HTTP request *)
		messageLength : UDINT;
	END_STRUCT;
END_TYPE


(* ----- Enumerations ----- *)
TYPE	
	state_enum : 
		(
		ST_IDLE,
		ST_CONVERT_TO_JSON,
		ST_PARSE,
		ST_LOGIN, 
		ST_RENDER_RESPONSE, 
		ST_SEND_RESPONSE,
		ST_LOGOUT,
		ST_LOGIN_ERROR,
		ST_LOGOUT_ERROR
		);
END_TYPE
