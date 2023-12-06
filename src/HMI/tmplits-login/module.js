//DO NOT DELETE THIS FILE 
//- Doing so will cause 404 errors on the client side which will not break anything, but will throw errors in the console.

//This file will get loaded as a javascript module, meaning you can import other modules from here.
//You can also export functions from here, which will be available to the client side.

//import * from "./module2.js"//Import relative to this file inside node_modules/this-module-name/
//import * from "../tmplits-some-other/module.js"//Import relative to this file inside node_modules/tmplits-some-other/
//import * from "/somewhere.js"//Import from the root of the project

//Define your tmplit functions here and export them to make them globally available
import * as util from "../tmplits-utilities/module.js"

// //---------------------------------------------
// // User level (taken form @loupeteam/webhmi/webhmi.js)
// //---------------------------------------------

// var userLevelPV;
// var currentUserLevel = 0;

// function setUserLevel(level) {
//     currentUserLevel = level;
// }

// function setUserLevelPV(levelPV) {
//     if (levelPV !== undefined) { // Don't set PV to something that doesn't exist; that's what clearUserLevelPV is for
//         userLevelPV = levelPV;
//         initCyclicRead(levelPV);
//     }
// }

// function clearUserLevelPV() {
//     userLevelPV = undefined;
// }

// export function getUserLevel() {
//     // return thisMachine.value(userLevelPV);
//     console.log("1000");
// }


// document.getElementById("loginForm").onsubmit = function getUserLevel() {
//     if (userLevelPV === undefined) {
//         // Not using PV; use internal value instead
//         return currentUserLevel;
//     }
//     // return thisMachine.value(userLevelPV);
//     console.log("1000");
// };


export function TmplitLogin(context, args) {

    // Get Current Login Level PV
    var userLevelPV;
    var currentUserLevel = 0;

    function getUserLevel() {
        // return thisMachine.value(userLevelPV);
        console.log("1000");
    }
    
    console.log("in the module function")
    
    return `    `

    
}