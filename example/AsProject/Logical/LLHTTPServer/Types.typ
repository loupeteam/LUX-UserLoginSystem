
TYPE
	Task_typ : 	STRUCT 
		status : Task_Status_typ;
		internal : Task_Internal_typ;
	END_STRUCT;
END_TYPE

(* ----- Main Task Sturcture ----- *)
TYPE
	Task_Status_typ : 	STRUCT 
		error : BOOL;
	END_STRUCT;
	
	Task_Internal_typ : 	STRUCT  (* Information not to be accessed outside this task *)
		(*Web Comms*)
		server : LLHttpServer;
		response : LLHttpResponse;
		defaultResponse : LLHttpResponse;
		responseHeader : LLHttpHeader_typ;
		receiveBufferNotFound : STRING[8000];
		
		(*JSON Parser*)
		parser : jsmn_parser;
		tokens : ARRAY[0..15]OF jsmntok_t;
		queryJSON : STRING[500];
		parsedQuery : parsedQuery_typ; 
				
		(*User Level Application Vars*)
		sendBuffer : sendBuffer_typ;
		userLvl : userLvl_enum;
		user : Credential_typ;
		isUser : BOOL;
		admin : Credential_typ;
		isAdmin : BOOL;
		
	END_STRUCT;
	
	Credential_typ : 	STRUCT 
		username : STRING[255];
		password : STRING[255];
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
	userLvl_enum : 
		(
		LOGGED_OUT,
		USER,
		ADMIN
		);
END_TYPE
