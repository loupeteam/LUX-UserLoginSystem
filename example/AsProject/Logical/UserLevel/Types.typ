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
		login : BOOL;
		getLevel : BOOL;
		logout : BOOL;
	END_STRUCT;
	
	Task_Status_typ : 	STRUCT 
		error : BOOL;
		isAuth : BOOL;
	END_STRUCT;
	
	Task_Internal_typ : 	STRUCT  (* Information not to be accessed outside this task *)
		
		loginLvl : STRING[128];
		MpUser : MpUser_typ;
		user : parsedQueryData_typ;
		
	END_STRUCT;
	
END_TYPE

(* ----- User Types ----- *)
TYPE
	
	MpUser_typ : 	STRUCT 		
		
		Login_FB : MpUserXLogin;
		
	END_STRUCT;
	
	parsedQueryData_typ : 	STRUCT 
		userName : WSTRING[50];
		password : WSTRING[50];
	END_STRUCT;
	
	
END_TYPE
