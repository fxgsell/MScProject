/*
**      scene.js
**      functions to init and run the scene
*/

var scene;
var camera;
var renderer;
var spider;

//move a leg, position can be 0-6, motor 0-2, angle -90-90.
//motor 0 of position 0,1,2 got inversed angle than motor 0 position 3,4,5
function moveLeg(position, motor, angle) {
	switch (motor){
		case 0:
			if (position > 2)
				angle = -angle;
			(spider.children[position]).children[0].rotation.y = angle * Math.PI / 180;
			break;
		case 1:
			((spider.children[position]).children[0]).children[0].rotation.z = angle * Math.PI / 180;
			break;
		case 2:
			((((spider.children[position]).children[0]).children[0]).children[0]).children[0].rotation.z = (angle - 90) * Math.PI / 180;
			break;
	}
}

//Init the scene
function init() {
	//init socket
	var socket = io.connect('http://localhost:33334');
	socket.on('moveLeg', function (position, motor, angle) {
			moveLeg(position, motor, angle);
			console.log("new data : {position:" + position + ", motor:" + motor + ", angle:" + angle + "}");
			});

	//create scene
	scene = new THREE.Scene(); 
	camera = new THREE.PerspectiveCamera(75, window.innerWidth/window.innerHeight, 0.1, 1000); 
	renderer = new THREE.WebGLRenderer({antialias:true});
	renderer.setClearColorHex(0x000000, 1);
	renderer.setSize(window.innerWidth, window.innerHeight);
	document.body.appendChild(renderer.domElement);
	var arena = new THREE.Object3D();
	//display axes xyz
	spider = createBody(new THREE.MeshLambertMaterial({ color: 0x444444}));
	arena.add(spider);
	arena.add(createArrow(new THREE.MeshLambertMaterial({ color: 0x444444})));
	scene.add(arena);
	//light
	var light = new THREE.SpotLight(0xffffff, 2);
	light.position.set(0, 10, 10);
	arena.add(light);
	light = new THREE.AmbientLight(0x303030);
	arena.add(light);
	//mouse event
	document.onmousedown = handleMouseDown;
	document.onmouseup = handleMouseUp;
	document.onmousemove = handleMouseMove;
	run();
}

function run()
{
	renderer.render(scene, camera);
	camera.position.x = Math.sin(ry / ( 2 * Math.PI)) * 3;
	camera.position.z = Math.cos(ry / ( 2 * Math.PI)) * 3;
	camera.position.y = rx / 10.0;
	camera.lookAt(new THREE.Vector3(0, 0, 0));
	requestAnimationFrame(run);
}

init();
