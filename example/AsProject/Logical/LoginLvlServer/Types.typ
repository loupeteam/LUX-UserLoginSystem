TYPE
	Task_typ : 	STRUCT 
		cmd : Task_Command_typ;
		status : Task_Status_typ;
		internal : Task_Internal_typ;
	END_STRUCT;
END_TYPE

(* ----- Main Task Sturcture ----- *)
TYPE
	Task_Command_typ : STRUCT
		exportUsers : BOOL;
		authRequest : BOOL;
		
	END_STRUCT;
	
	Task_Status_typ : 	STRUCT 
		error : BOOL;
		isAuth : BOOL;
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
				
		(*User Level Application Vars*)
		sendBuffer : sendBuffer_typ;
		loginLvl : STRING[MAX_USER_ITEM_LEN];
		ArUser : ArUser_typ;
		
	END_STRUCT;
	
END_TYPE

(* ----- MpUserX Types ----- *)
TYPE
	
	ArUser_typ : 	STRUCT 		
		
		AuthenticatePassword_FB : ArUserAuthenticatePassword;
		InitList_FB : ArUserInitList;
		DestoryList_FB : ArUserDestroyList;
		GetFirst_FB : ArUserGetFirst;
		GetNext_FB : ArUserGetNext;
		
		GetProperty_FB : ArUserGetProperty;
		SetProperty_FB : ArUserSetProperty;
		PropertyName : STRING[MAX_USER_ITEM_LEN];
		
		Export_FB : ArUserExport;
		FilePath : STRING[MAX_USER_ITEM_LEN] := 'C:/Temp/UserDataExport.txt';
		
	END_STRUCT;
	
	
END_TYPE

(* ----- JSON Types ----- *)
TYPE
	parsedQuery_typ : 	STRUCT 
		data : parsedQueryData_typ;
		status : INT;
	END_STRUCT;
	parsedQueryData_typ : 	STRUCT 
		userName : STRING[MAX_USER_ITEM_LEN];
		password : STRING[MAX_USER_ITEM_LEN];
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
END_TYPE
