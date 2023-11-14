/**
 *	
 */

import * as THREE from '../modules/three.module.js';
import {
	GLTFLoader
} from '../modules/GLTFLoader.js'
import {
	OrbitControls
} from '../modules/OrbitControls.js'

let scene, orbit, renderer, camera, canvasHeight, canvasWidth;

// THREE.js
scene = new THREE.Scene();

// link glb model files

const loader = new GLTFLoader();
var robotData = {
	joints: {
		objects: [],		// populated in loader
		pose: undefined,	// populated in loader
		set: undefined		// populated in loader
	},
}
loader.load('../model/Robot.glb', function (gltf) {

	scene.add(gltf.scene);

	// Load objects
	robotData.scene = gltf.scene

	// Set the opacity of certain objects
	scene.getObjectByName('Transfer_Chamber').material = new THREE.MeshPhongMaterial({
		color: 0xFFFFFF,
		opacity: 0.5,
		transparent: true,
		side: THREE.DoubleSide,
	});

	// these must be defined in the model
	robotData.joints.objects[0] = scene.getObjectByName('Z_Actuator');
	robotData.joints.objects[1] = scene.getObjectByName('Stage');
	robotData.joints.objects[2] = scene.getObjectByName('Carriage');

	scene.traverse(function (object) {
		if (object.isMesh) {
			object.castShadow = true
			object.receiveShadow = true
		}
	});

	for (let i = 0; i < robotData.joints.length; i++) {
		robotData.joints.objects[i].castShadow = true
		robotData.joints.objects[i].receiveShadow = true
	}

	// Define joint setting functions
	robotData.joints.set = [
		(zAxis, thetaAxis, rAxis) => {
			robotData.joints.objects[0].position.setZ(zAxis / 1000);
			robotData.joints.objects[2].position.setX(rAxis / 1000);
			robotData.joints.objects[1].rotation.z = thetaAxis * Math.PI / 180;
		}
	];

	onWindowResize()

}, undefined, function (error) {
	console.error(error);
})

init();

function init() {

	canvasHeight = $('#robot').height() - 5;
	canvasWidth = $('#robot').width() - 5;

	camera = new THREE.PerspectiveCamera(60, canvasWidth / canvasHeight, 0.1, 1000);
	camera.position.set(2, 2, 3);
	camera.up.set(0, 0, 1)

	renderer = new THREE.WebGLRenderer({antialias: true});
	renderer.setClearColor(0xfcfcfc);
	renderer.setSize(canvasWidth*2, canvasHeight*2);

	// Add light to scene
	const light = new THREE.HemisphereLight(0xffffff, 100);
	light.position.set(0, 0, 10);
	scene.add(light);

	renderer.shadowMap.enabled = true;
	renderer.shadowMap.type = THREE.PCFSoftShadowMap;

	renderer.domElement.id = 'robotcanvas';
	renderer.domElement.style.width = canvasWidth + 'px';
	renderer.domElement.style.height = canvasHeight + 'px';

	// Update camera and renderer settings if the window is resized
	window.addEventListener('resize', onWindowResize, false);
	
	orbit = new OrbitControls(
		camera,
		renderer.domElement
	);
	orbit.target.set(0, 0, 0);

	render()

}

function onWindowResize() {
	canvasHeight = $('#robot').height() - 5;
	canvasWidth = $('#robot').width() - 5;

	camera.aspect = canvasWidth / canvasHeight;
	camera.updateProjectionMatrix();

	renderer.setSize(canvasWidth*2, canvasHeight*2);
	renderer.domElement.style.width = canvasWidth + 'px';
	renderer.domElement.style.height = canvasHeight + 'px';
}

function render() {

	setTimeout(function(){requestAnimationFrame(render);}, 5);

	orbit.update();
	let el = $('#robot')

	// Render and update stats
	if(el.is(":visible")){
		if(el[0].children.length == 0){
			el[0].appendChild(renderer.domElement);
		}
		renderer.render(scene, camera);
	}

}

// expose pose function
robotData.pose = (zAxis, thetaAxis, rAxis) => {

	if (robotData.joints.set === undefined) {
		// wait for model to finish loading
		return;
	}
	
	robotData.joints.set[0](zAxis, thetaAxis, rAxis);

};

// Set the color of a all children meshes of an object.
function setMeshColor(object, color) {
	object.traverse(function (child) {
		if (child.isMesh) {
			child.material.color.setHex(color);
		}
	});
}

export { robotData }

setTimeout(onWindowResize, 1000);
