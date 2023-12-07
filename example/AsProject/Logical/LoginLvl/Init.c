
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	// Start as Logged out
	loginLevel = LOGGED_OUT;
	loginAttempt = 0;
	
	strcpy(userName, "user");
	strcpy(userPass, "test");
	strcpy(adminName, "admin");
	strcpy(adminPass, "test2");
}
