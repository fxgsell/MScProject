/*
**	create_parts.js
**	functions to create each part of the robot
*/

//create axes
function axes(length) {
    var axes = new THREE.Line();
    axes.geometry = new THREE.Geometry();
    axes.material = new THREE.LineBasicMaterial();
    axes.geometry.vertices.push(new THREE.Vector3(0, 0, 0));
    axes.geometry.vertices.push(new THREE.Vector3(length, 0, 0));
    axes.geometry.vertices.push(new THREE.Vector3(0, 0, 0));
    axes.geometry.vertices.push(new THREE.Vector3(0, length, 0));
    axes.geometry.vertices.push(new THREE.Vector3(0, 0, 0));
    axes.geometry.vertices.push(new THREE.Vector3(0, 0, length));
    axes.geometry.colors.push(new THREE.Color(0xff0000));
    axes.geometry.colors.push(new THREE.Color(0xff0000));
    axes.geometry.colors.push(new THREE.Color(0x00ff00));
    axes.geometry.colors.push(new THREE.Color(0x00ff00));
    axes.geometry.colors.push(new THREE.Color(0x0000ff));
    axes.geometry.colors.push(new THREE.Color(0x0000ff));
    axes.material.vertexColors = true;
    axes.type = THREE.LinePieces;
    return axes;
}

//create a cube part
function createCube(width, height, depth, material) {
    var geometry = new THREE.CubeGeometry(width, height, depth);
    var cube = new THREE.Mesh(geometry, material);
    return cube;
}

//create the leg part assign to the motor 2
function createLegEnd() {
    var realO = new THREE.Object3D();
    var object = createCube(0.3, 0.18, 0.2);
    var object2 = createCube(0.4, 0.1, 0.1);
    object2.position.x += 0.35;
    object.add(object2);
    object.position.x += 0.1;
    realO.add(object);
    realO.position.x += 0.2;
    return realO;
}

//create the leg part assign to the motor 1
function createLegMid() {
    var realO = new THREE.Object3D();
    var object = createCube(0.4, 0.18, 0.2);
    object.position.x += 0.15;
    object.add(createLegEnd());
    realO.add(object);
    realO.position.x += 0.17;
    return realO;
}

//create the leg part assign to the motor 0
function createLegBase() {
    var realO = new THREE.Object3D();
    var object = createCube(0.3, 0.18, 0.2);
    object.position.x += 0.10;
    object.add(createLegMid());
    realO.add(object);
    return realO;
}

//create the spider
function createBody() {
    var geometry = new THREE.Geometry();

    //create the body
    for (var i = 0; i < 6; ++i) {
        geometry.vertices.push(new THREE.Vector3(Math.cos(i * Math.PI / 3), 0.1, Math.sin(i * Math.PI / 3)));
        geometry.vertices.push(new THREE.Vector3(Math.cos(i * Math.PI / 3), -0.1, Math.sin(i * Math.PI / 3)));
    }
    geometry.faces.push(new THREE.Face4(0, 6, 4, 2));
    geometry.faces.push(new THREE.Face4(6, 0, 10, 8));
    geometry.faces.push(new THREE.Face4(1, 3, 5, 7));
    geometry.faces.push(new THREE.Face4(7, 9, 11, 1));
    for (var i = 0; i < 6; ++i)
        geometry.faces.push(new THREE.Face4(1 + (i + 1) * 2 % 12, 1 + i * 2 % 12, i * 2 % 12, (i + 1) * 2 % 12));
    //create the 3D model of the body
    var object = new THREE.Mesh(geometry);
    //create the 6 legs
    for (var i = 0; i < 6; ++i) {
        var base = createLegBase();
        base.rotation.y = i * Math.PI / 3;
        base.translateX(1);
        object.add(base);
    }
    return object;
}