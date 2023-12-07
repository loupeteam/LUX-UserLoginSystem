
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _CYCLIC ProgramCyclic(void)
{
	// check if incommingUser OR incommingPass has changed since last scan
	// if so, make user lowercase
	if(loginAttempt) {
		// Reset the attempt flag
		loginAttempt = 0;
		//toLower(incomingUser);
		
		// if string compare incommingUser == user && string compare incomingPass == userPassword
		if((!strcmp(&incomingUser,&userName)) && (!strcmp(&incomingPass,&userPass))) {	
			loginLevel = USER;
		}
		// if string compare incommingUser == admin && string compare incomingPass == userPassword
		else if((!strcmp(&incomingUser,&adminName)) && (!strcmp(&incomingPass,&adminPass))) {	
			loginLevel = ADMIN;
		}
		else {
			loginLevel = LOGGED_OUT;
		}
	}
	
	
}
