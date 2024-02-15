//
// TODO: Add descriptive comments here
//
//
//

// Define your tmplit functions here and export them to make them globally available
import * as util from "../tmplits-utilities/module.js"

export function DefaultLogin(url, username, password) { // TODO: Maybe named something more cryptic

fetch(url+'?' + new URLSearchParams({userName: username, password: password}),{
    method: 'GET',
    }).then((response) => {
        if (response.ok) {
            return response.json();
        } else {
            throw new Error('Something went wrong');
        }
    }).then((data) => {
        // TODO: Set UserLvlPV
        console.log(data);
    })
    .catch((error) => {
        console.log(error);
    });
}

// Function will get executed on click of submit button 
export function SubmitForm(e) {
    e.preventDefault();

    let scope = e.target.classList.contains('lui-loginForm') ? e.target : e.target.closest('.lui-loginForm');
    // Get the username and password from the form
    let loginUser = scope.querySelector('.lui-loginUser');
    let loginPass = scope.querySelector('.lui-loginPass');
    
    // If form value is empty set the vars to single character strings (for PLC object to contain something)
    let username = !loginUser.value ? " " : loginUser.value;
    let password = !loginPass.value ? " " : loginPass.value;

    // For debug
    // console.log("User: ", username)
    // console.log("Password: ", password)

    // Call the default login function    
    DefaultLogin("http://127.0.0.1:1238/getUserLvl", username, password)
};
 
export function OpenModal(e) {
    e.preventDefault();
    // Find the scope for this tmplit instance
    let scope = e.target.classList.contains('lui-login-scope') ? e.target : e.target.closest('.lui-login-scope');
    // Get the modal element within the scope
    let modal = scope.querySelector('.lui-loginModal');
    modal.classList.add("show");
    
};

export function CloseModal(e) {
    e.preventDefault();
    // Find the scope for this tmplit instance
    let scope = e.target.classList.contains('lui-login-scope') ? e.target : e.target.closest('.lui-login-scope');
    // Get the modal element within the scope
    let modal = scope.querySelector('.lui-loginModal');
    modal.classList.remove("show");
    
};

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
        <div class="lui-login-scope" >
        <!-- Modal Trigger-->
        <button class="btn btn-primary lui-loginBtn" onclick="OpenModal(event)">
            Login
        </button>

        <button class="btn btn-primary" min-user-level-unlock="1">
            Log Out
        </button>
        
        <!-- Modal -->
           <div class="modal lui-loginModal">                     
            <div class="modal-dialog">                    
                <div class="modal-content">
                    
                    <!-- Modal Header -->
                    <div class="modal-header">
                        <button type="button" class="close" onclick="CloseModal(event)">
                            <span>&times;</span>
                            <span class="sr-only">Close</span>
                        </button>
                        <h4 class="modal-title">
                            HMI Login
                        </h4>
                    </div>
                    
                    <!-- Modal Body -->
                    <div class="modal-body">
                        
                        <form role="form" class="lui-loginForm" action=''>
                        <div class="form-group">
                            <label for="loginUser">Username</label>
                            <input type="text" class="form-control lui-loginUser" placeholder="Username"/>
                        </div>
                        <div class="form-group">
                            <label for="loginPass">Password</label>
                            <input type="password" class="form-control lui-loginPass" placeholder="Password"/>
                        </div>
                        <button type="submit" class="btn btn-default" onclick="SubmitForm(event)">Submit</button>
                        </form>
                        
                    </div>
                    
                    <!-- Modal Footer -->
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" onclick="CloseModal(event)"> Close </button>
                    </div>
                </div>
            </div>
        </div>
    </div>

    `
}