//
// TODO: Add descriptive comments here
//
//
//

//Define your tmplit functions here and export them to make them globally available
import * as util from "../tmplits-utilities/module.js"

function DefaultLogin(evt) { // TODO: Maybe named something more cryptic

    // Look for the login-scope and find the class labeled loginUser (or loginPass) and set as the target so the value can accessed
    // This is done to avoid the use of id attributes 
    // Probably don't need this when using HTTP as it will be directly used by the request and doesn't need to be accessed
    let scope = evt.target.classList.contains('lui-login-scope') ? evt.target : evt.target.closest('.lui-login-scope')
    
    // These values are the ones to be passed to the PLC for verification
    let loginUser = scope.querySelector('.lui-loginUser')
    console.log("User: ", loginUser.value)
    let loginPass = scope.querySelector('.lui-loginPass')
    console.log("Password: ", loginPass.value)

}

export function TmplitLogin(context, args) {

    // Extract the most important args and apply default values if not specified by user
    let { 
        onLogin = 'DefaultLogin', 
        userlevelPV, 
        ..._args
    } = args.hash //_args now does NOT include the extracted vars

    //Get cleaned up values from args
    let {
        classList,              // convert any class attributes to classList (array of strings)
        attr                    // convert everything else to a single attribute string for later use (join, string interpolation)
    } = util.cleanArgs(_args)


    // TODO: Add support if user has passed in a PV (immediatly setUserPV)


    // TODO: onClick of Logout button set user level to 0 

    return ` 
    <!-- Modal Trigger-->
    <button class="btn btn-primary" data-toggle="modal" data-target="#loginModal">
        Login
    </button>

    <button class="btn btn-primary" min-user-level-unlock="1">
        Log Out
    </button>
    
    <!-- Modal -->
    <div class="modal fade" id="loginModal" tabindex="-1" role="dialog" aria-labelledby="loginModalLabel" aria-hidden="true">
        
        <div class="modal-dialog">
            <div class="modal-content">
                
                <!-- Modal Header -->
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">
                           <span aria-hidden="true">&times;</span>
                           <span class="sr-only">Close</span>
                    </button>
                    <h4 class="modal-title" id="loginModalLabel">
                        HMI Login Level
                    </h4>
                </div>
                
                <!-- Modal Body -->
                <div class="modal-body">
                    
                    <form role="form" method="GET" action="">
                      <div class="form-group">
                        <label for="loginUser">Username</label>
                        <input type="text" class="form-control lui-loginUser" placeholder="Username"/>
                      </div>
                      <div class="form-group">
                        <label for="loginPass">Password</label>
                          <input type="password" class="form-control lui-loginPass" placeholder="Password"/>
                      </div>
                      <button type="submit" class="btn btn-default">Submit</button>
                    </form>
                    
                </div>
                
                <!-- Modal Footer -->
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal"> Close </button>
                </div>
            </div>
        </div>
    </div>

    `
}