(*
 * File: MsgBoxErr.fun
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of MsgBoxErr, licensed under the MIT License.
 *)
FUNCTION_BLOCK MsgBoxAck (*TODO: Add your comment here*)
	VAR_INPUT
		Active : BOOL;
		IsError : BOOL;
		Message : STRING[320];
		OKText : STRING[20];
		IgnoreText : STRING[20];
		Password : STRING[80];
		pErrCol : UDINT;
	END_VAR
	VAR_OUTPUT
		Execute : BOOL;
		Ignored : BOOL;
	END_VAR
	VAR
		MsgID : UDINT;
		Accept : BOOL;
		AckError : BOOL;
		ErrorAdded : BOOL;
		InputPassword : STRING[80];
		dErrcol : REFERENCE TO ErrorCollector_typ;
		ErrorTimeout : TON;
		OkAddr : UDINT;
		OkCmd : UDINT;
		IgnoreAddr : UDINT;
		IgnoreCmd : UDINT;
		addstatus : UINT;
	END_VAR
END_FUNCTION_BLOCK