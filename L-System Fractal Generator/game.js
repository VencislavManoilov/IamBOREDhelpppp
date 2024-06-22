let angleChange = 45, stepsStart = 1;

function update() {
    angleChange = parseInt(document.getElementById("angle").value, 10);
    stepsStart = parseInt(document.getElementById("steps").value, 10);

    console.log(angleChange, stepsStart);
}

function draw() {
    tree(400, 300, -90, 50, stepsStart);
}

function tree(x, y, angle, size, steps) {
    if(steps <= 0) {
        return;
    }
    
    let NewX = x + Math.cos(angle * Math.PI / 180) * size;
    let NewY = y + Math.sin(angle * Math.PI / 180) * size;
    drawLine(x, y, NewX, NewY, 2, "black");

    tree(NewX, NewY, angle - angleChange, size / 2, steps - 1);
    tree(NewX, NewY, angle + angleChange, size / 2, steps - 1);
}