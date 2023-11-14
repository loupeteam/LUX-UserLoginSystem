(********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: CSVFileLib_enum.typ
 * Author: David Blackburn
 * Created: March 08, 2012
 ********************************************************************
 * Data types of library CSVFileLib
 ********************************************************************)

TYPE
	CSV_ERR_enum : 
		(
		CSV_ERR_PVITEM_ENUM := 14713,
		CSV_ERR_INVALIDINPUT := 50000,
		CSV_ERR_MEMCFG,
		CSV_ERR_MEMALLOC,
		CSV_ERR_NOTINITIALIZED,
		CSV_ERR_LINEFAILURE,
		CSV_ERR_BUFFERFULL,
		CSV_ERR_LOGNOTAVAILABLE,
		CSV_ERR_LOGFULL,
		CSV_ERR_INVALIDOPERATION,
		CSV_ERR_COMMENT,
		CSV_ERR_NOVARNAMEFOUND,
		CSV_ERR_NOVALUEFOUND,
		CSV_ERR_INVALIDVALUE,
		CSV_ERR_UNSUPPORTEDTYPE,
		CSV_ERR_INVALIDTYPE,
		CSV_ERR_MAXLEVELREACHED,
		CSV_ERR_PREFIXNOTSTRUCTURE,
		CSV_ERR_EMPTYFILENAME,
		CSV_ERR_BUSY := 65535
		);
	CSV_TYPE_enum : 
		(
		CSV_TYPE_STRUCT := 0, (*0*)
		CSV_TYPE_BOOL,
		CSV_TYPE_SINT,
		CSV_TYPE_INT,
		CSV_TYPE_DINT,
		CSV_TYPE_USINT, (*5*)
		CSV_TYPE_UINT,
		CSV_TYPE_UDINT,
		CSV_TYPE_REAL,
		CSV_TYPE_STRING,
		CSV_TYPE_ULINT, (*10*)
		CSV_TYPE_DATE_AND_TIME,
		CSV_TYPE_TIME,
		CSV_TYPE_DATE,
		CSV_TYPE_LREAL,
		CSV_TYPE_ARRAY_OF_STRUCT, (*15*)
		CSV_TYPE_TIME_OF_DAY,
		CSV_TYPE_BYTE,
		CSV_TYPE_WORD,
		CSV_TYPE_DWORD,
		CSV_TYPE_LWORD, (*20*)
		CSV_TYPE_WSTRING, (*21*)
		CSV_TYPE_LINT := 23 (*23*)
		);
	CSV_INFO_enum : 
		(
		CSV_INFO_NOVARNAMEFOUND, (*0*)
		CSV_INFO_NOVALUEFOUND,
		CSV_INFO_ADDRESSNOTFOUND,
		CSV_INFO_DATATYPENOTFOUND,
		CSV_INFO_INVALIDVALUE,
		CSV_INFO_UNSUPPORTEDTYPE, (*5*)
		CSV_INFO_INVALIDTYPE,
		CSV_INFO_INVALIDINPUT,
		CSV_INFO_FILEFULL,
		CSV_INFO_INFONOTFOUND,
		CSV_INFO_MAXLEVELREACHED, (*10*)
		CSV_INFO_ITEMNOTFOUND,
		CSV_INFO_PREFIXNOTSTRUCTURE,
		CSV_INFO_VERBOSE_VARNAME,
		CSV_INFO_VERBOSE_VALUE,
		CSV_INFO_VERBOSE_DESCRIPTION, (*15*)
		CSV_INFO_VERBOSE_VARADDRESS,
		CSV_INFO_VERBOSE_VARTYPE,
		CSV_INFO_VERBOSE_VARLENGTH,
		CSV_INFO_VERBOSE_VARDIMENSION,
		CSV_INFO_VERBOSE_COMMENT, (*20*)
		CSV_INFO_DONE (*21*)
		);
	CSV_FILELIST_ST_enum : 
		(
		CSV_FILELIST_ST_IDLE,
		CSV_FILELIST_ST_DIRINFO,
		CSV_FILELIST_ST_DIRREAD,
		CSV_FILELIST_ST_
		);
	CSV_ST_enum : 
		(
		CSV_ST_IDLE := 0, (*0*)
		CSV_ST_GET_TEMPLATE,
		CSV_ST_EXPANDVARS,
		CSV_ST_OPEN,
		CSV_ST_PROCESS,
		CSV_ST_DELETELOG,
		CSV_ST_SAVELOG, (*4*)
		CSV_ST_DELETETEMP := 100, (*100*)
		CSV_ST_SAVETEMP,
		CSV_ST_DELETEORIGINAL,
		CSV_ST_RENAMETEMP, (*103*)
		CSV_ST_DELETEFILE := 200, (*200*)
		CSV_ST_ERROR := 999 (*999*)
		);
	CSV_OP_enum : 
		(
		CSV_OP_INVALID := 0,
		CSV_OP_OPEN,
		CSV_OP_SAVE,
		CSV_OP_DELETE
		);
END_TYPE
