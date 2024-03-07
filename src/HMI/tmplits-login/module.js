//
// TODO: Add descriptive comments here
//
//
//

// Define your tmplit functions here and export them to make them globally available
import * as util from "../tmplits-utilities/module.js"


export function TmplitLoginDefaultFetch(url, username, password, onSuccess) { 
// this is the object thtat the method is owned by

fetch(url+'?' + new URLSearchParams({userName: username, password: password}),{
    method: 'GET'
    }).then((response) => {
        if (response.ok) {
            return response.json();
        } else {
            throw new Error('Something went wrong');
        }
    }).then((data) => {
        if(typeof onSuccess === "function") { // TODO: test with and without passing in an onSuccess callback function
            // Callback function for successfull http response
            onSuccess(data.loginLvl); 
        }
        console.log(data);
    })
    .catch((error) => {
        console.log(error);
    });
}

// Function will get executed on click of submit button 
export function TmplitLoginSubmitForm(e) {
    e.preventDefault();

    // Find the scope for this form instance
    let scopeForm = e.target.classList.contains('lui-loginForm') ? e.target : e.target.closest('.lui-loginForm');
    // Get the username and password from the form
    let loginUser = scopeForm.querySelector('.lui-loginUser');
    let loginPass = scopeForm.querySelector('.lui-loginPass');
    
    // If form value is empty set the vars to single character strings (for PLC object to contain something)
    let username = !loginUser.value ? " " : loginUser.value;
    let password = !loginPass.value ? " " : loginPass.value;

    // Find the scope for the overall tmplit instance
    let scopeLogin = e.target.classList.contains('lui-login-scope') ? e.target : e.target.closest('.lui-login-scope');
    // Get the machine from the scope
    let localMachineName = scopeLogin.getAttribute('data-machine-name') // localMachineName is the string assigned to the dataMachineName js var via aliasing with the data-machine-name attribute
    let localMachine = window[localMachineName] //returns the golbal variable object with the same name as the string stored in localMachineName
    // Get the server ip from the scope
    let localLoginServerIp = scopeLogin.getAttribute('data-server-ip') // localLoginServerIp is the string assigned to the LoginServerIp js var via aliasing with the data-server-ip attribute
    
    // Call the default login function with a callback function   
    TmplitLoginDefaultFetch("http://"+ localLoginServerIp + ":1238/getLoginLvl", username, password,(level)=>{
        // Set machine user level from response           
        localMachine.setUserLevel(level);
    }) // Define the callback using a Closure to allow for lexical scoping
};
 
export function TmplitLoginOpenModal(e) {
    e.preventDefault();
    // Find the scope for this tmplit instance
    let scope = e.target.classList.contains('lui-login-scope') ? e.target : e.target.closest('.lui-login-scope');
    // Get the modal element within the scope
    let modal = scope.querySelector('.lui-loginModal');
    modal.classList.add("show");
    
};

export function TmplitLoginCloseModal(e) {
    e.preventDefault();
    // Find the scope for this tmplit instance
    let scope = e.target.classList.contains('lui-login-scope') ? e.target : e.target.closest('.lui-login-scope');
    // Get the modal element within the scope
    let modal = scope.querySelector('.lui-loginModal');
    modal.classList.remove("show");
    
};
export function TmplitLoginLogoutBtn(e) {
    e.preventDefault();
    // Find the scope for the overall tmplit instance
    let scopeLogin = e.target.classList.contains('lui-login-scope') ? e.target : e.target.closest('.lui-login-scope');
    let localMachineName = scopeLogin.getAttribute('data-machine-name') // localMachineName is the string assigned to the dataMachineName js var via aliasing with the data-machine-name attribute
    let localMachine = window[localMachineName] //returns the golbal variable object with the same name as the string stored in localMachineName
    // Reset machine user level
    localMachine.setUserLevel(0);
    
};

export function TmplitLogin(context, args) {

    // Extract the most important args and apply default values if not specified by user
    let { 
        userlevelPV, // TODO: Add support if user has passed in a PV (immediatly setUserPV)
        ['data-machine-name']:dataMachineName = "machine", // dataMachineName can be passed from the user using the alias 'data-machine-name' or assigned the default string here
        ['data-server-ip']:loginServerIp = "127.0.0.1",
        ..._args
    } = args.hash //_args now does NOT include the extracted vars

    // Get cleaned up values from args
    let {
        classList,              // convert any class attributes to classList (array of strings)
        attr                    // convert everything else to a single attribute string for later use (join, string interpolation)
    } = util.cleanArgs(_args)
 

    


    return `
        <div class="lui-login-scope" data-machine-name=${dataMachineName} data-server-ip=${loginServerIp} >
        <!-- Modal Trigger-->
        <button class="btn btn-primary lui-loginBtn" onclick="TmplitLoginOpenModal(event)">
            Login
        </button>

        <button class="btn btn-primary lui-logoutBtn" min-user-level-unlock="1" onclick="TmplitLoginLogoutBtn(event)">
            Log Out
        </button>
        
        <!-- Modal -->
           <div class="modal lui-loginModal">                     
            <div class="modal-dialog">                    
                <div class="modal-content">
                    
                    <!-- Modal Header -->
                    <div class="modal-header">
                        <button type="button" class="close" onclick="TmplitLoginCloseModal(event)">
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
                        <button type="submit" class="btn btn-default" onclick="TmplitLoginSubmitForm(event)">Submit</button>
                        </form>
                        
                    </div>
                    
                    <!-- Modal Footer -->
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" onclick="TmplitLoginCloseModal(event)"> Close </button>
                    </div>
                </div>
            </div>
        </div>
    </div>

    `
}