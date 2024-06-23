let angleChange = 45, stepsStart = 1, sizeChange = 1.3;

function update() {
    angleChange = parseInt(document.getElementById("angle").value, 10);
    stepsStart = parseInt(document.getElementById("steps").value, 10);
    sizeChange = parseInt(document.getElementById("size").value, 10) / 100;
}

function draw() {
    tree(400, 300, -90, 50, stepsStart);
}

function tree(x, y, angle, size, steps) {
    let NewX = x + Math.cos(angle * Math.PI / 180) * size;
    let NewY = y + Math.sin(angle * Math.PI / 180) * size;

    context.beginPath();
    context.moveTo(x, y);
    context.lineTo(NewX, NewY);
    context.stroke();

    if(steps > 0) {
        tree(NewX, NewY, angle - angleChange, size / sizeChange, steps - 1);
        tree(NewX, NewY, angle + angleChange, size / sizeChange, steps - 1);
    }
}