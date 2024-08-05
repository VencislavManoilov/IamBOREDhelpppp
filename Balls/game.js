let balls = [], R = 250;

class Ball {
    constructor(x, y, R) {
        this.x = x;
        this.y = y;
        this.velocityX = 0;
        this.velocityY = 0;
        this.R = R;
        this.color = `hsl(${randomInteger(360)}, 100%, 50%)`
    }

    Update() {
        this.x += this.velocityX;
        this.y += this.velocityY;
    }

    Draw() {
        fillArc(windowSizeX/2 + this.x, windowSizeY/2 + this.y, this.R, this.color);
    }
}

balls.push(new Ball(220, 0, 25));

changeBg("black")

function update() {
    for(let i = 0; i < balls.length; i++) {
        // Adds gravity
        balls[i].velocityY += 0.05;

        // Checks for border colition
        if(distance(0, 0, balls[i].x, balls[i].y) > R - balls[i].R) {
            let normal = Math.atan2(-balls[i].y, -balls[i].x);
            let angleGoing = Math.atan2(-balls[i].velocityY, -balls[i].velocityX);
            let difference = normal - angleGoing;
            let angle = normal + difference;
            let speed = distance(0, 0, balls[i].velocityX, balls[i].velocityY);
            
            // Bounces the balls
            balls[i].velocityX = Math.cos(angle) * speed;
            balls[i].velocityY = Math.sin(angle) * speed;
        }

        // Updates the velocity
        balls[i].Update();
    }
}

function draw() {
    for(let i = 0; i < balls.length; i++) {
        balls[i].Draw();
    }

    strokeArc(windowSizeX/2, windowSizeY/2, R, 2, "white");
}

function angle2points(x1, y1, x2, y2) {
    return Math.atan2(y1 - y2, x1 - x2);
}