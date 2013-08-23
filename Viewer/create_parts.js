/*
 * **      create_parts.js
 * **      functions to create each part of the robot
 * */

//create arrow
function createArrow(material) {
    var geometry = new THREE.Geometry();
    geometry.vertices.push(new THREE.Vector3(-0.5, 0.101, 0.1));
    geometry.vertices.push(new THREE.Vector3(-0.5, 0.101, -0.1));
    geometry.vertices.push(new THREE.Vector3(0.25, 0.101, -0.1));
    geometry.vertices.push(new THREE.Vector3(0.25, 0.101, 0.1));
    geometry.vertices.push(new THREE.Vector3(0.25, 0.101, 0.2));
    geometry.vertices.push(new THREE.Vector3(0.5, 0.101, 0));
    geometry.vertices.push(new THREE.Vector3(0.25, 0.101, -0.2));
    geometry.faces.push(new THREE.Face4(2, 1, 0, 3));
    geometry.faces.push(new THREE.Face3(4, 5, 6));
    geometry.computeFaceNormals();
    var object = new THREE.Mesh(geometry, material);
    return object;
}


//create a cube part
function createCube(width, height, depth, material) {
	var geometry = new THREE.CubeGeometry(width, height, depth);
	var cube = new THREE.Mesh(geometry, material);
	return cube;
}

//create the leg part assign to the motor 2
function createLegEnd(material) {
	var realO = new THREE.Object3D();
	var object = createCube(0.3, 0.18, 0.2, material);
	var object2 = createCube(0.4, 0.1, 0.1, material);
	object2.position.x += 0.35;
	object.add(object2);
	object.position.x += 0.1;
	realO.add(object);
	realO.position.x += 0.2;
	return realO;
}

//create the leg part assign to the motor 1
function createLegMid(material) {
	var realO = new THREE.Object3D();
	var object = createCube(0.4, 0.18, 0.2, material);
	object.position.x += 0.15;
	object.add(createLegEnd(material));
	realO.add(object);
	realO.position.x += 0.17;
	return realO;
}

//create the leg part assign to the motor 0
function createLegBase(material) {
	var realO = new THREE.Object3D();
	var object = createCube(0.3, 0.18, 0.2, material);
	object.position.x += 0.10;
	object.add(createLegMid(material));
	realO.add(object);
	return realO;
}

//create the spider
function createBody(material) {
	var geometry = new THREE.Geometry();

	//create the body
	for (var i = 0; i < 6; ++i) {
		geometry.vertices.push(new THREE.Vector3(Math.cos((i + 0.5) * Math.PI / 3), 0.1, Math.sin((i + 0.5) * Math.PI / 3)));
		geometry.vertices.push(new THREE.Vector3(Math.cos((i + 0.5) * Math.PI / 3), -0.1, Math.sin((i + 0.5) * Math.PI / 3)));
	}
	geometry.faces.push(new THREE.Face4(0, 6, 4, 2));
	geometry.faces.push(new THREE.Face4(6, 0, 10, 8));
	geometry.faces.push(new THREE.Face4(1, 3, 5, 7));
	geometry.faces.push(new THREE.Face4(7, 9, 11, 1));
	for (var i = 0; i < 6; ++i)
		geometry.faces.push(new THREE.Face4(1 + (i + 1) * 2 % 12, 1 + i * 2 % 12, i * 2 % 12, (i + 1) * 2 % 12));
	//create the 3D model of the body
	geometry.computeFaceNormals();
	var object = new THREE.Mesh(geometry, material);
	object.geometry.dynamic = true
		object.geometry.__dirtyVertices = true;
	object.geometry.__dirtyNormals = true;
	object.geometry.computeVertexNormals();
	object.geometry.computeFaceNormals();

	//create the 6 legs
	for (var i = 0; i < 6; ++i) {
		var base = createLegBase(material);
		base.rotation.y = (i + 0.5) * Math.PI / 3;
		base.translateX(1);
		object.add(base);
	}
	return object;
}
