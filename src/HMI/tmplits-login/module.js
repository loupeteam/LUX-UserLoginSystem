//Define your tmplit functions here and export them to make them globally available
import * as util from "../tmplits-utilities/module.js"

function DefaultLogin(evt) {

    let scope = evt.target.classList.contains('lui-login-scope') ? evt.target : evt.target.closest('.lui-login-scope')
    

    let target = scope.querySelector('.lui-loginPass')
    target.value 


}

export function TmplitLogin(context, args) {

    let {
        onLogin = 'DefaultLogin', // amybe more cryptic
        levelPV, 
        ..._args
    } = args.hash //_args now does NOT include onLogin
    args.hash = {
        onLogin:'value'
        levelPV:'value'
        anotherthing:'value'
    }
    //Get cleaned up values from args
    let {
        classList,              // convert class to classList
        attr                    // everything else in args as a string
    } = util.cleanArgs(_args)

    // ([document.getElementById('loginUser').value, document.getElementById('loginPass').value])
    //scpoe find class login user 
    


    // Get Current HMI number (for use as index in login array, could be passed in as an arg?)
    // machine.writeVariable("LoginLvl:requestedHmi", 1)


    // they've passed in a PV I know I need to do a setPV 


    // onClick of Logout button set user level to 0 


    // machine.writeVariable("LoginLvl:loginAttempt", 1) // Set loginAttempt to TRUE
    // machine.writeVariable("LoginLvl:incomingUser", context[0]) // Send Username input
    // machine.writeVariable("LoginLvl:incomingPass", context[1])  // Sent Password input

    // machine.readVariable("LoginLvl:loginLevel") // Receive Login Level

    // console.log("Machine.LoginLevel: ", machine.value("LoginLvl:loginLevel")) 

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