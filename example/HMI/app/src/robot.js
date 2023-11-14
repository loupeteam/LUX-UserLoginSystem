import { robotData } from './model.js'

var updateRobot = function () {
    try{
  
      if (robotData === undefined || robotData.pose === undefined) {
        // wait for model to finish loading
        return;
      }
  
      // Read all axis positions
      var zAxis = machine['RobotFast:robot']?.Info.JointAxisPosition[0];
      var thetaAxis = machine['RobotFast:robot']?.Info.JointAxisPosition[1];
      var rAxis = machine['RobotFast:robot']?.Info.JointAxisPosition[2];
  
      robotData.pose(zAxis, thetaAxis, rAxis);
  
    }
    catch (e){
      console.log(e)
    }
  }

  WEBHMI.on({
    "update-hmi": function () {
      updateRobot();
    }
  });
  