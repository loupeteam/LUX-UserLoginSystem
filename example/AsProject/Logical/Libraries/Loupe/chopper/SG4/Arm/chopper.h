/* Automation Studio generated header file */
/* Do not edit ! */
/* chopper 1.05.0 */

#ifndef _CHOPPER_
#define _CHOPPER_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _chopper_VERSION
#define _chopper_VERSION 1.05.0
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG3
		#include "vartools.h"
		#include "stringext.h"
#endif
#ifdef _SG4
		#include "vartools.h"
		#include "stringext.h"
#endif
#ifdef _SGC
		#include "vartools.h"
		#include "stringext.h"
#endif


/* Constants */
#ifdef _REPLACE_CONST
 #define CHOP_NUM_FLAG_LEN 30U
 #define CHOP_TEMPLATE_MAX_VARIABLES 100U
 #define CHOP_TEMPLATE_MAI_VARIABLES 99U
 #define CHOP_TEMPLATE_STRLEN_SOURCE 5000U
#else
 _GLOBAL_CONST unsigned char CHOP_NUM_FLAG_LEN;
 _GLOBAL_CONST unsigned char CHOP_TEMPLATE_MAX_VARIABLES;
 _GLOBAL_CONST unsigned char CHOP_TEMPLATE_MAI_VARIABLES;
 _GLOBAL_CONST unsigned short CHOP_TEMPLATE_STRLEN_SOURCE;
#endif




/* Datatypes and datatypes of function blocks */
typedef enum Chop_Status_Enum
{	CHOP_ERR_INVALID_INPUT = 50000,
	CHOP_ERR_SOURCE_LENGTH,
	CHOP_ERR_TEMPLATE_FULL,
	CHOP_ERR_NO_END_OF_VAR,
	CHOP_ERR_NOT_COMPILED,
	CHOP_ERR_DEST_LENGTH,
	CHOP_ERR_INTERNAL,
	CHOP_ERR_INVALID_FORMATTER = 51000
} Chop_Status_Enum;

typedef struct Chop_Template_Variable_typ
{	struct varVariable_typ pv;
	unsigned long prefixStart;
	unsigned long prefixLen;
	plcstring flags[31];
	plcbit hasVar;
} Chop_Template_Variable_typ;

typedef struct Chop_Template_typ
{	struct Chop_Template_Variable_typ snippet[100];
	unsigned short iSnippet;
	plcstring source[5001];
	plcbit compiled;
} Chop_Template_typ;

typedef struct Chopper
{
	/* VAR_INPUT (analog) */
	unsigned long pDest;
	unsigned long pTemplate;
	unsigned long pSource;
	unsigned long maxDestLength;
	/* VAR_OUTPUT (analog) */
	unsigned long renderLength;
	signed long status;
	/* VAR (analog) */
	signed long compileStatus;
	/* VAR_INPUT (digital) */
	plcbit recompile;
} Chopper_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC void Chopper(struct Chopper* inst);
_BUR_PUBLIC signed long ChopCompile(unsigned long pTemplate, unsigned long pSource);
_BUR_PUBLIC signed long ChopRender(unsigned long pDest, unsigned long pTemplate, unsigned long maxDestLength, unsigned long pRenderLength);
_BUR_PUBLIC signed long ChopReplace(unsigned long pTemplate, unsigned long pTag, unsigned long address, unsigned long type);
_BUR_PUBLIC signed long ChopReplaceVar(unsigned long pTemplate, unsigned long pTag, unsigned long pVarName);
_BUR_PUBLIC signed long ChopGetErrorMsg(unsigned long pDest, unsigned long maxDestLength, signed long errorID);
_BUR_PUBLIC plcbit ChopGetDefaultFormatter(plcstring* dest, unsigned long type);
_BUR_PUBLIC plcbit ChopVerifyFormatter(plcstring* pSrc, unsigned long type);


#ifdef __cplusplus
};
#endif
#endif /* _CHOPPER_ */

