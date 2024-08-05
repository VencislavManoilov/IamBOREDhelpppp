let balls = [], R = 250, r = 20;

class Ball {
    constructor(x, y, R) {
        this.x = x;
        this.y = y;
        this.velocityX = 0;
        this.velocityY = 0;
        this.R = R;
        this.mass = (R / 20) ** 2;
        this.color = `hsl(${randomInteger(0, 360)}, 100%, 50%)`
    }

    Update() {
        this.x += this.velocityX;
        this.y += this.velocityY;
    }

    Draw() {
        fillArc(windowSizeX/2 + this.x, windowSizeY/2 + this.y, this.R, this.color);
    }
}

for(let i = 0; i < 10; i++) {
    let newBall;
    let overlapping;
    do {
        overlapping = false;
        let x = (Math.random() * 2 - 1) * (R - 20);
        let y = (Math.random() * 2 - 1) * (R - 20);
        newBall = {
            x: x,
            y: y,
            R: 20
        };

        // Check for overlap with existing balls
        for(let j = 0; j < balls.length; j++) {
            if(distance(newBall.x, newBall.y, balls[j].x, balls[j].y) < newBall.R + balls[j].R) {
                overlapping = true;
                break;
            }
        }
    } while(overlapping);
    balls.push(new Ball(newBall.x, newBall.y, newBall.R));
}

changeBg("black")

function update() {
    for (let i = 0; i < balls.length; i++) {
        // Adds gravity
        balls[i].velocityY += 0.05;

        // Checks for border collision
        let distFromCenter = distance(0, 0, balls[i].x, balls[i].y);
        if(distFromCenter > R - balls[i].R) {
            // Calculate the normal vector at the collision point
            let normalX = balls[i].x / distFromCenter;
            let normalY = balls[i].y / distFromCenter;

            // Calculate the dot product of velocity and normal
            let dotProduct = balls[i].velocityX * normalX + balls[i].velocityY * normalY;

            // Reflect the velocity vector
            balls[i].velocityX -= 2 * dotProduct * normalX;
            balls[i].velocityY -= 2 * dotProduct * normalY;
            
            // Move the ball back to the boundary to prevent it from getting stuck
            balls[i].x = normalX * (R - balls[i].R);
            balls[i].y = normalY * (R - balls[i].R);
        }

        // Checks for ball collision
        for(let j = i + 1; j < balls.length; j++) {
            let dist = distance(balls[i].x, balls[i].y, balls[j].x, balls[j].y);
            if(dist < balls[i].R + balls[j].R) {
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
                let newDotProductNormal1 = (dotProductNormal1 * (balls[i].mass - balls[j].mass) + 2 * balls[j].mass * dotProductNormal2) / (balls[i].mass + balls[j].mass);
                let newDotProductNormal2 = (dotProductNormal2 * (balls[j].mass - balls[i].mass) + 2 * balls[i].mass * dotProductNormal1) / (balls[i].mass + balls[j].mass);

                // Update the velocities
                balls[i].velocityX = tangentX * dotProductTangent1 + normalX * newDotProductNormal1;
                balls[i].velocityY = tangentY * dotProductTangent1 + normalY * newDotProductNormal1;
                balls[j].velocityX = tangentX * dotProductTangent2 + normalX * newDotProductNormal2;
                balls[j].velocityY = tangentY * dotProductTangent2 + normalY * newDotProductNormal2;

                // Separate the balls to prevent them from sticking together
                let overlap = 0.5 * (balls[i].R + balls[j].R - dist + 0.01);  // Slightly increase overlap to avoid precision issues
                balls[i].x -= overlap * normalX;
                balls[i].y -= overlap * normalY;
                balls[j].x += overlap * normalX;
                balls[j].y += overlap * normalY;
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