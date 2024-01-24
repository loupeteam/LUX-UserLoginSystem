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
		configureUsers : BOOL;
	END_STRUCT;
	
	Task_Status_typ : 	STRUCT 
		error : BOOL;
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
		tokens : ARRAY[0..15] OF jsmntok_t;
		queryJSON : STRING[500];
		parsedQuery : parsedQuery_typ; 
				
		(*User Level Application Vars*)
		sendBuffer : sendBuffer_typ;
		ArUser : ArUser_typ;
		users : ARRAY[0..MAI_USERS] OF Credential_typ;
		loginLvl : loginLvl_enum; (* LoginLvl to be set from ArUser system and returned via HTTP response *)
		
	END_STRUCT;
	
END_TYPE


(* ----- ARUser System Types ----- *)
TYPE
	ArUser_typ : 	STRUCT 		
		ArUserCreate_0 : ArUserCreate; (*Create an user with given name; default no password *)
		ArUserSetPassword_0 : ArUserSetPassword; (*Set user password*)
		ArUserSetProperty_0 : ArUserSetProperty; (*Write a property*)
		ArUserGetProperty_0 : ArUserGetProperty; (*Read property*)
		ArUserInitList_0 : ArUserInitList;
		ArUserList : ArUserIdentType;
		ArUserExport_0 : ArUserExport; (*Export all user and role data*)
		FilePath : STRING[128] := 'C:/Temp/UserDataExport.txt';
		internalState : ArUser_internalState_emun; (* Internal State for managing asynch execution of FUBs in cyclic *)
	END_STRUCT;
	
	Credential_typ : 	STRUCT 
		username : STRING[MAX_USER_ITEM_LEN];
		password : STRING[MAX_USER_ITEM_LEN];
		loginLvl : loginLvl_enum;
	END_STRUCT;
	
END_TYPE


(* ----- Configuration Types ----- *)
TYPE
	Configuration_typ : 	STRUCT 
		users : ARRAY[0..MAI_USERS] OF Configuration_Credential_typ;
	END_STRUCT;
	Configuration_Credential_typ : 	STRUCT 
		username : STRING[MAX_USER_ITEM_LEN];
		password : STRING[MAX_USER_ITEM_LEN];
		loginLvl : loginLvl_enum;
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
	
	ArUser_internalState_emun : 
		(
		CREATE_USER,
		SET_PASSWORD,
		SET_LOGIN_LVL,
		ERROR
	);
END_TYPE
