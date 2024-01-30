
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	task.internal.MpUser.Login_FB.MpLink = &gUserXLogin;
	task.internal.MpUser.Login_FB.Enable = 1;
	
	
	brwcsconv(&task.internal.user.userName, "admin", 0); 
	brwcsconv(&task.internal.user.password, "test", 0); // L means each char is a wide char
	task.internal.MpUser.Login_FB.UserName = &task.internal.user.userName;
	task.internal.MpUser.Login_FB.Password = &task.internal.user.password;
	
	
}

void _CYCLIC ProgramCyclic(void)
{

	if(task.cmd.login) {
		task.internal.MpUser.Login_FB.Login = 1;
		task.internal.MpUser.Login_FB.Logout = 0;
		task.cmd.login = 0;
	}
	
	if(task.cmd.logout) {
		task.internal.MpUser.Login_FB.Login = 0;
		task.internal.MpUser.Login_FB.Logout = 1;
		task.cmd.logout = 0;
	}
	
	
	MpUserXLogin(&task.internal.MpUser.Login_FB);
	
	
}


