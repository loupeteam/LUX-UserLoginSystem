/* Automation Studio generated header file */
/* Do not edit ! */
/* MsgBoxErr 0.00.1 */

#ifndef _MSGBOXERR_
#define _MSGBOXERR_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _MsgBoxErr_VERSION
#define _MsgBoxErr_VERSION 0.00.1
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG3
		#include "errorlib.h"
		#include "messagebox.h"
#endif
#ifdef _SG4
		#include "errorlib.h"
		#include "messagebox.h"
#endif
#ifdef _SGC
		#include "errorlib.h"
		#include "messagebox.h"
#endif


/* Datatypes and datatypes of function blocks */
typedef struct MsgBoxAck
{
	/* VAR_INPUT (analog) */
	plcstring Message[321];
	plcstring OKText[21];
	plcstring IgnoreText[21];
	plcstring Password[81];
	unsigned long pErrCol;
	/* VAR (analog) */
	unsigned long MsgID;
	plcstring InputPassword[81];
	struct ErrorCollector_typ* dErrcol;
	struct TON ErrorTimeout;
	unsigned long OkAddr;
	unsigned long OkCmd;
	unsigned long IgnoreAddr;
	unsigned long IgnoreCmd;
	unsigned short addstatus;
	/* VAR_INPUT (digital) */
	plcbit Active;
	plcbit IsError;
	/* VAR_OUTPUT (digital) */
	plcbit Execute;
	plcbit Ignored;
	/* VAR (digital) */
	plcbit Accept;
	plcbit AckError;
	plcbit ErrorAdded;
} MsgBoxAck_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC void MsgBoxAck(struct MsgBoxAck* inst);


#ifdef __cplusplus
};
#endif
#endif /* _MSGBOXERR_ */

