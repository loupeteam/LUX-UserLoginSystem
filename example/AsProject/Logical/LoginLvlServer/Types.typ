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
		loginLvl : loginLvl_enum;
		
	END_STRUCT;
	
END_TYPE

(* ----- Configuration Types ----- *)
TYPE
	Configuration_typ : 	STRUCT 
		user : Configuration_Credential_typ;
		admin : Configuration_Credential_typ;
	END_STRUCT;
	Configuration_Credential_typ : 	STRUCT 
		username : STRING[255];
		password : STRING[255];
	END_STRUCT;
END_TYPE

(* ----- JSON Types ----- *)
TYPE
	parsedQuery_typ : 	STRUCT 
		data : parsedQueryData_typ;
		status : INT;
	END_STRUCT;
	parsedQueryData_typ : 	STRUCT 
		userName : STRING[255];
		password : STRING[255];
	END_STRUCT;
	sendBuffer_typ : 	STRUCT 
		template : Chop_Template_typ;
		message : STRING[8000]; (* The complied string returned from Chopper - to use in HTTP request *)
		messageLength : UDINT;
	END_STRUCT;
END_TYPE

(* ----- Enumerations ----- *)
TYPE
	loginLvl_enum : 
		(
		LOGGED_OUT,
		USER,
		ADMIN
		);
	
	state_enum : 
		(
		IDLE,
		CONVERT_TO_JSON,
		PARSE, 
		GET_LOGIN_LVL, 
		RENDER_RESPONSE, 
		ERROR
		);
END_TYPE
