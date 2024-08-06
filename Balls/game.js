let balls = [], R = 250, r = 20, holeAngle = randomInteger(0, 360), holeSize = 20, start = false, gamemode = 1, bounces = 0;

const titles = [
    "How many balls can fit in",
    "Will it escape before gets too big",
    "On every bounce it gets smaller"
]

class Ball {
    constructor(x, y, R) {
        this.x = x;
        this.y = y;
        this.velocityX = 0;
        this.velocityY = 0;
        this.inside = true;
        this.R = R;
        this.color = `hsl(${randomInteger(0, 360)}, 100%, 50%)`
    }
    
    Update() {
        this.x += this.velocityX;
        this.y += this.velocityY;
        this.mass = (R / 20) ** 2;
    }
    
    Draw() {
        fillArc(windowSizeX/2 + this.x, windowSizeY/2 + this.y, this.R, this.color);
    }
}

function SpawnBall(size) {
    let newBall;
    let overlapping;
    do {
        overlapping = false;
        let x = R * 3;
        let y = R * 3;
        let tries = 0;
        while(distance(0, 0, x, y) > R - size  && tries < 10) {
            x = randomInteger(0, R*2 - size) - 250;
            y = randomInteger(0, R*2 - size) - 250;
            tries++;
        }
        if(tries >= 10 && gamemode == 3) {
            x = randomInteger(0, 20) - 10; y = randomInteger(0, 20) - 10;
        }
        newBall = {
            x: x,
            y: y,
            R: size
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

function Switch(gamemode) {
    switch(gamemode) {
        case 1:
            balls = [];
            for(let i = 0; i < 10; i++) {
                SpawnBall(r);
            }
        break;
        case 2:
            balls = [];
            SpawnBall(5);
        break;
        case 3:
            balls = [];
            SpawnBall(R - 50);
        default:
        break;
    }
}

Switch(gamemode);

changeBg("black")

let pressed = false;
function update() {
    if(!pressed && !start) {
        if(Input.GetKey(KeyCode.ArrowUp)) {
            gamemode++;
            Switch(gamemode);
            pressed = true;
        } else if(Input.GetKey(KeyCode.ArrowDown)) {
            gamemode--;
            Switch(gamemode);
            pressed = true;
        } else if(Input.GetKey(KeyCode.ArrowRight)) {
            Switch(gamemode);
            pressed = true;
        }
    }

    if(gamemode < 1) {
        gamemode = 3;
        Switch(gamemode);
    } else if(gamemode > 3) {
        gamemode = 1;
        Switch(gamemode);
    }

    if(Input.GetKey(KeyCode.Space)) {
        if(start && !pressed && Input.GetKey(KeyCode.Space)) {
            document.location.reload();
        }

        start = true;
        pressed = true;
    }

    if(!start) {
        return;
    }
    holeAngle += 0.25;

    if(holeAngle > 360) {
        holeAngle -= 360;
    } else if(holeAngle < 0) {
        holeAngle += 360;
    }

    // Convert holeAngle and holeSize from degrees to radians
    let holeAngleRad = holeAngle * Math.PI / 180;
    let holeSizeRad = holeSize * Math.PI / 180;

    for (let i = 0; i < balls.length; i++) {
        // Adds gravity
        balls[i].velocityY += 0.05;

        // Checks for border collision
        let distFromCenter = distance(0, 0, balls[i].x, balls[i].y);
        if (distFromCenter > R - balls[i].R && balls[i].inside) {
            // Calculate the ball's angle
            let ballAngle = Math.atan2(balls[i].y, balls[i].x);
            if (ballAngle < 0) {
                ballAngle += 2 * Math.PI; // Normalize angle to [0, 2π]
            }

            // Check if the ball is in the hole range
            let holeStart = holeAngleRad - holeSizeRad;
            let holeEnd = holeAngleRad;
            console.log(Math.cos(holeSizeRad) * R, Math.sin(holeSizeRad) * R);

            if (start && balls[i].R * 2 < distance(250, 0, Math.cos(holeSizeRad) * R, Math.sin(holeSizeRad) * R) && (holeStart <= ballAngle && ballAngle <= holeEnd) ||
                (holeStart < 0 && (ballAngle <= holeEnd || ballAngle >= holeStart + 2 * Math.PI)) ||
                (holeEnd > 2 * Math.PI && (ballAngle >= holeStart || ballAngle <= holeEnd - 2 * Math.PI))) {
                balls[i].inside = false;
                BallGoOut({ size: balls[i].R });
            }
            
            if(balls[i].inside) {
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

                Bounce(gamemode);
            }
        }

        // Checks for ball collision
        for (let j = i + 1; j < balls.length; j++) {
            let dist = distance(balls[i].x, balls[i].y, balls[j].x, balls[j].y);
            if (dist < balls[i].R + balls[j].R && balls[j].inside && balls[i].inside) {
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

        // Updates the velocity
        balls[i].Update();
    }
}

function draw() {
    for(let i = 0; i < balls.length; i++) {
        balls[i].Draw();
    }

    // strokeArc(windowSizeX/2, windowSizeY/2, R, 2, "white");
    context.save();
    context.translate(windowSizeX/2, windowSizeY/2);
    context.rotate(holeAngle*Math.PI/180);
    context.translate(-windowSizeX/2, -windowSizeY/2);

    context.beginPath();
    context.arc(windowSizeX/2, windowSizeY/2, R, 0, 2 * (1 - holeSize / 360) * Math.PI);
    context.strokeStyle = "white";
    context.lineWidth = 2;
    context.stroke();

    context.restore();

    if(!start) {
        fillText("Gamemode: " + gamemode, windowSizeX/2 - 130, 10, 40, "arial", "white");
        transparent(70);
        fillRect(windowSizeX/2 - 386, windowSizeY/2 - 56, 774, 102, "black");
        transparent(100);
        fillText("Press SPACE to START", windowSizeX/2 - 370, windowSizeY/2 - 35, 70, "arial", "white");
    } else {
        fillText(titles[gamemode - 1], 0, 10, 50, "arial", "white");
        switch(gamemode) {
            case 1:
                fillText("Balls: " + balls.length, 0, 60, 40, "arial", "white");
            break;
            case 2:
                fillText("Bounces: " + bounces, 0, 60, 40, "arial", "white");
            break;
            case 3:
                fillText("Bounces: " + bounces, 0, 60, 40, "arial", "white");
            break;
            default:
            break;
        }
    }
}

function Bounce(gamemode) {
    switch(gamemode) {
        case 1:
        break;
        case 2:
            balls[0].R += 1;
        break;
        case 3:
            balls[0].R -= 1;
        break;
        default:
        break;
    }

    bounces++;
}

function BallGoOut(options) {
    switch(gamemode) {
        case 1:
            SpawnBall(options.size*0.90);
            SpawnBall(options.size*0.90);
        break;
        case 2:
        break;
        case 3:
        break;
        default:
        break;
    }
}

function angle2points(x1, y1, x2, y2) {
    return Math.atan2(y1 - y2, x1 - x2);
}

function keyup(key) {
    pressed = false;
}