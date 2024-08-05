let balls = [], R = 250, r = 10;

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

for(let i = 0; i < 20; i++) {
    let x = 0;
    let y = 0;
    while(distance(250, 250, x, y) > R - r) {
        x = randomInteger(0, 500);
        y = randomInteger(0, 500);
        for(let j = 0; j < balls.length; j++) {
            if(distance(x, y, balls[j].x, balls[j].y) < r*2) {
                x = 0; y = 0;
            }
        }
    }
    balls.push(new Ball(x - 250, y - 250, r))
}

changeBg("black")

function update() {
    for (let i = 0; i < balls.length; i++) {
        // Adds gravity
        balls[i].velocityY += 0.05;

        // Checks for border collision
        if (distance(0, 0, balls[i].x, balls[i].y) > R - balls[i].R) {
            let normal = Math.atan2(balls[i].y, balls[i].x);
            let angleGoing = Math.atan2(balls[i].velocityY, balls[i].velocityX);
            let difference = normal - angleGoing;
            let angle = normal + difference;
            let speed = distance(0, 0, balls[i].velocityX, balls[i].velocityY);

            // Bounces the balls
            balls[i].velocityX = -Math.cos(angle) * speed;
            balls[i].velocityY = -Math.sin(angle) * speed;
        }

        // Checks for ball collision
        for (let j = i + 1; j < balls.length; j++) {
            let dist = distance(balls[i].x, balls[i].y, balls[j].x, balls[j].y);
            if (dist < balls[i].R + balls[j].R) {
                // Calculate the normal and tangent vectors
                let normalX = (balls[j].x - balls[i].x) / dist;
                let normalY = (balls[j].y - balls[i].y) / dist;
                let tangentX = -normalY;
                let tangentY = normalX;

                // Calculate the dot product of the velocities with the normal and tangent vectors
                let dotProductNormal1 = balls[i].velocityX * normalX + balls[i].velocityY * normalY;
                let dotProductNormal2 = balls[j].velocityX * normalX + balls[j].velocityY * normalY;
                let dotProductTangent1 = balls[i].velocityX * tangentX + balls[i].velocityY * tangentY;
                let dotProductTangent2 = balls[j].velocityX * tangentX + balls[j].velocityY * tangentY;

                // Calculate the new normal velocities
                let newDotProductNormal1 = (2 * dotProductNormal2) / 2;
                let newDotProductNormal2 = (2 * dotProductNormal1) / 2;

                // Update the velocities
                balls[i].velocityX = tangentX * dotProductTangent1 + normalX * newDotProductNormal1;
                balls[i].velocityY = tangentY * dotProductTangent1 + normalY * newDotProductNormal1;
                balls[j].velocityX = tangentX * dotProductTangent2 + normalX * newDotProductNormal2;
                balls[j].velocityY = tangentY * dotProductTangent2 + normalY * newDotProductNormal2;

                // console.log(dist, normalX, normalY, tangentX, tangentY, dotProductNormal1, dotProductTangent2, newDotProductNormal1, newDotProductNormal2)
            }
        }

        // Update the position based on the velocity
        balls[i].x += balls[i].velocityX;
        balls[i].y += balls[i].velocityY;

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