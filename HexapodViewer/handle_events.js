/*
**	handle_events.js
**	functions to handle the mouse and the keyboard
*/

/*
** HANDLE MOUSE PART
*/

var mouseDown = false;
var lastMouseX;
var lastMouseY;
var ry = 0;
var rx = 30;

function handleMouseDown(event)
{
    mouseDown = true;
    lastMouseX =  event.clientX;
    lastMouseY =  event.clientY;
}

function handleMouseUp(event)
{
    mouseDown = false;
}

function handleMouseMove(event) 
{
    if (!mouseDown) {
        return;
    }

    var dx = event.clientX - lastMouseX;
    var dy = event.clientY - lastMouseY;

    ry += dx;
    rx += dy;

    lastMouseX = event.clientX
    lastMouseY = event.clientY;
}
