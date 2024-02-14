//
// TODO: Add descriptive comments here
//
//
//

// Define your tmplit functions here and export them to make them globally available
import * as util from "../tmplits-utilities/module.js"



export function DefaultLogin(url, username, password) { // TODO: Maybe named something more cryptic
    const controller = new AbortController();
    const signal = controller.signal;


    // -------------- Using XMLHttpRequest----------------------
    // let request = new XMLHttpRequest();

    // //TODO: Post allows for a body, but do we really want post? Prolly not..
    // request.open('POST', url, true);    
    // request.onload = function () {
    //     // Begin accessing JSON data here
    //     var data = JSON.parse(this.response);
    //     machine.setUserLevel(data.userLevel);
    // }
    // request.send(JSON.stringify({username:username, password:password}));
    // -------------- Using XMLHttpRequest----------------------

fetch(url+'?' + new URLSearchParams({userName: username, password: password}),{
    method: 'GET',
    signal: signal
    }).then((response) => {
        if (response.ok) {
            
            return response.json();
        } else {
            throw new Error('Something went wrong');
        }
    }).then((data) => {
        console.log(data);
    })
    .catch((error) => {
        console.log(error);
    });
}

// Function will get executed on click of submit button 
$(document).on('click', '#submitButton', function(e) {
    e.preventDefault();
    // Get the username and password from the form
    let username;
    let password;

    if(!$('#loginUser').val()){
        username = " ";
    }else {
        username = $('#loginUser').val()
    }
    if(!$('#loginPass').val()){
        password = " ";
    }else {
        password = $('#loginPass').val()
    }
    // Call the default login function    
    DefaultLogin("http://127.0.0.1:1238/getLoginLvl", username, password)
});
 

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
                    
                    <form role="form" id="formID" action=''>
                      <div class="form-group">
                        <label for="loginUser">Username</label>
                        <input type="text" class="form-control lui-loginUser" placeholder="Username" id="loginUser"/>
                      </div>
                      <div class="form-group">
                        <label for="loginPass">Password</label>
                          <input type="password" class="form-control lui-loginPass" placeholder="Password" id="loginPass"/>
                      </div>
                      <button type="submit" id="submitButton" class="btn btn-default">Submit</button>
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