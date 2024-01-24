#include <bur/plctypes.h>
#include <string.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

void ConfigureUser(ArUser_typ* ArUser, Credential_typ* user) {
	/*
	* 
	* This fuction is used to set up an ArUser using know credentials for the user
	*
	*/
	switch(ArUser->internalState) {
		
		case CREATE_USER:
			// ----------------- CREATE USER -----------------
			// Pass temp values to ArUser FUB
			memcpy(ArUser->ArUserCreate_0.UserName, user->username, strlen(user->username));
			ArUser->ArUserCreate_0.Execute = 1;
			// Create the ArUser		
			ArUserCreate(&ArUser->ArUserCreate_0);
			// Check that the FUB is complete
			if (ArUser->ArUserCreate_0.Done && !ArUser->ArUserCreate_0.Busy) {
				ArUser->ArUserCreate_0.Execute = 0;
				ArUser->internalState = SET_PASSWORD;
			} else if (ArUser->ArUserCreate_0.Error) {
				ArUser->internalState = ERROR;
			}
			
			break;

		case SET_PASSWORD:
			// ----------------- SET USER PASSWORD -----------------
			// Pass temp values to ArUser FUB
			strcpy(ArUser->ArUserSetPassword_0.UserName, user->username);
			strcpy(ArUser->ArUserSetPassword_0.Password, user->password);
			ArUser->ArUserSetPassword_0.Execute = 1;
			// Set the ArUser Password
			ArUserSetPassword(&ArUser->ArUserSetPassword_0);
			// Check that the FUB is complete
			if (ArUser->ArUserSetPassword_0.Done && !ArUser->ArUserSetPassword_0.Busy) {
				ArUser->ArUserSetPassword_0.Execute = 0;
				ArUser->internalState = SET_LOGIN_LVL;
			} else if (ArUser->ArUserSetPassword_0.Error) {
				ArUser->internalState = ERROR;
			}
			
			break;
		
		case SET_LOGIN_LVL:
			// ----------------- SET USER LOGIN LEVEL -----------------
			// Pass temp values to ArUser FUB
			ArUser->ArUserSetProperty_0.ElementType = arUSER_USER_PROPERTIES;
			strcpy(ArUser->ArUserSetProperty_0.ElementName, user->username);
			strcpy(ArUser->ArUserSetProperty_0.PropertyName, "loginLvl");
			itoa(user->loginLvl,ArUser->ArUserSetProperty_0.PropertyValue);
			ArUser->ArUserSetProperty_0.Execute = 1;
			// Set the ArUser Property
			ArUserSetProperty(&ArUser->ArUserSetProperty_0);
			// Check that the FUB is complete
			if (ArUser->ArUserSetProperty_0.Done && !ArUser->ArUserSetProperty_0.Busy) {
				ArUser->ArUserSetProperty_0.Execute = 0;
				/*ArUser->internalState = CREATE_USER;*/
				return 0; // No errors
			} else if (ArUser->ArUserSetProperty_0.Error) {
				ArUser->internalState = ERROR;
			}
			break;
		
		case ERROR:
			// ----------------- ERROR -----------------
			return -1; // No errors
	}
	
}
