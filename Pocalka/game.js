let clicked = false;

class Player {
    constructor(x, y, size, maxHealth, gun, speed) {
        this.x = x;
        this.y = y;
        this.size = size;
        this.maxHealth = maxHealth;
        this.health = maxHealth;
        this.gun = gun;
        this.speed = speed;
        this.zombie = zombie;

        this.gun.x = this.x + this.size/2;
        this.gun.y = this.y + this.size/2;
    }

    Move(forward, down) {
        this.x += forward * this.speed;
        this.y += down * this.speed;
    }

    draw(color) {
        fillRect(this.x, this.y, this.size, this.size, color);
        
        fillRect(this.x, this.y - 15, this.size, 10, "#910000");
        fillRect(this.x, this.y - 15, this.size * (this.health / this.maxHealth), 10, "#14c400");

        this.gun.x = this.x + this.size/2;
        this.gun.y = this.y + this.size/2;
        this.gun.draw();
    }
}

class Gun {
    constructor(damage, speed, range) {
        this.x = 0;
        this.y = 0;
        this.angle = 0;
        this.speed = speed;
        this.damage = damage;
        this.range = range;
        this.ammos = [];
    }

    Shoot() {
        this.ammos.push(new Ammo(this.x + MoveForwardX(this.angle, 30), this.y + MoveForwardY(this.angle, 30), this.angle));
    }

    update() {
        for(let i = 0; i < this.ammos.length; i++) {
            this.ammos[i].x += MoveForwardX(this.ammos[i].angle, this.speed);
            this.ammos[i].y += MoveForwardY(this.ammos[i].angle, this.speed);

            if(distance(this.ammos[i].x, this.ammos[i].y, this.ammos[i].startX, this.ammos[i].startY) > this.range) {
                this.ammos.splice(i, 1);
                break;
            }
        }
    }

    draw() {
        for(let i = 0; i < this.ammos.length; i++) {
            this.ammos[i].draw();
        }

        rotate2(this.angle, this.x, this.y, 15);
        fillRect(this.x, this.y, 50, 15, "#756034");
        context.restore();
    }
}

class Ammo {
    constructor(x, y, angle) {
        this.x = x;
        this.y = y;
        this.startX = x;
        this.startY = y;
        this.angle = angle;
    }

    draw() {
        rotate2(this.angle, this.x, this.y, 10);
        fillRect(this.x, this.y, 20, 10, "#ede847");
        context.restore();
    }
}

let p = new Player(windowSizeX/2 - 25, windowSizeY/2 - 25, 50, 100, new Gun(10, 5, 2500), 3);
let zombies = [];

for(let i = 0; i < 10; i++) {
    zombies.push(new Player(randomInteger(0, windowSizeX), randomInteger(0, windowSizeY), 50, 20, new Gun(10, 5, 2500), 2));
}

function update() {
    let forward = 0, down = 0;
    if(Input.GetKey(KeyCode.W)) down = -1;
    if(Input.GetKey(KeyCode.A)) forward = -1;
    if(Input.GetKey(KeyCode.S)) down = 1;
    if(Input.GetKey(KeyCode.D)) forward = 1;
    p.Move(forward, down);
    p.gun.update();
}

function draw() {
    p.gun.angle = findAngleBetweenTwoPoints(p.x + p.size/2, p.y + p.size/2, mouseX, mouseY) * 180/Math.PI;

    p.draw("#3160eb");

    for(let i = 0; i < zombies.length; i++) {
        zombies[i].angle = findAngleBetweenTwoPoints(zombies[i].x, zombies[i].y, p.gun.x, p.gun.y) * 180/Math.PI;
        zombies[i].draw("#d43535");
    }
}

function mousedown() {
    if(!clicked) {
        p.gun.Shoot();
    }

    clicked = true;
}

function mouseup() {
    clicked = false;
}

function findAngleBetweenTwoPoints(x, y, x2, y2) {
    return Math.atan2(y2 - y, x2 - x);
}

function rotate2(angle, x, y, ys) {
    context.save();
    context.translate(x, y + ys/2);
    context.rotate(angle*Math.PI/180);
    context.translate(-x, -y - ys/2);
}

function rotate3(angle, x, y, xs, ys) {
    context.save();
    context.translate(x + xs/2, y + ys/2);
    context.rotate(angle*Math.PI/180);
    context.translate(-x - xs/2, -y - ys/2);
}