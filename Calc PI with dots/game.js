let dots = [], time = 0, numInside = 0, numOutside = 0;

changeBg("#333");

function update() {
    time++;

    if(time%1 == 0) {
        for(let i = 0; i < 1; i++) {
            let x = randomInteger(0, 500);
            let y = randomInteger(0, 500);
            let inside = true;
    
            if(distance(250, 250, x, y) > 250) {
                inside = false;
                numInside++;
            } else {
                numOutside++;
            }
            
            dots.push({x: x, y: y, inside: inside});
        }
    }
}

function draw() {
    for(let i = 0; i < dots.length; i++) {
        fillArc(dots[i].x + 200, dots[i].y + 200, 2, dots[i].inside ? "blue" : "red");
    }

    fillText("Pi: " + Math.floor(numInside/numOutside*100000)/10000, 200, 150, 25, "Arial", "black");

    strokeRect(200, 200, 500, 500, 2, "white");
    strokeArc(450, 450, 250, 2, "white");
}