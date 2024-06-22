#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#define N 150
#define SIZE (N+2)*(N+2)
#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}

void add_source(float* x, float* s, float dt) {
    int size = (N + 2) * (N + 2);
    for (int i = 0; i < size; i++) x[i] += dt * s[i];
}

void set_bnd(int b, float* x) {
    for (int i = 1; i <= N; i++) {
        x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
        x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
    }
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
    x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
    x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void diffuse(int b, float* x, float* x0, float diff, float dt) {
    float a = dt * diff * N * N;
    for (int k = 0; k < 20; k++) {
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                    x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
            }
        }
        set_bnd(b, x);
    }
}

void advect(int b, float* d, float* d0, float* u, float* v, float dt) {
    int i, j, i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;
    dt0 = dt * N;
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            x = i - dt0 * u[IX(i, j)];
            y = j - dt0 * v[IX(i, j)];
            if (x < 0.5f) x = 0.5f;
            if (x > N + 0.5f) x = N + 0.5f;
            i0 = (int)x;
            i1 = i0 + 1;
            if (y < 0.5f) y = 0.5f;
            if (y > N + 0.5f) y = N + 0.5f;
            j0 = (int)y;
            j1 = j0 + 1;
            s1 = x - i0;
            s0 = 1 - s1;
            t1 = y - j0;
            t0 = 1 - t1;
            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }
    set_bnd(b, d);
}

static void project(float* u, float* v, float* p, float* div) {
    int i, j, k;
    float h = 1.0f / N;
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
                v[IX(i, j + 1)] - v[IX(i, j - 1)]);
            p[IX(i, j)] = 0;
        }
    }
    set_bnd(0, div);
    set_bnd(0, p);
    for (k = 0; k < 20; k++) {
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
                    p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
            }
        }
        set_bnd(0, p);
    }
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            u[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
            v[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
        }
    }
    set_bnd(1, u);
    set_bnd(2, v);
}

void dens_step(float* x, float* x0, float* u, float* v, float diff, float dt) {
    add_source(x, x0, dt);
    SWAP(x0, x);
    diffuse(0, x, x0, diff, dt);
    SWAP(x0, x);
    advect(0, x, x0, u, v, dt);
}

void vel_step(float* u, float* v, float* u0, float* v0, float visc, float dt) {
    add_source(u, u0, dt);
    add_source(v, v0, dt);
    SWAP(u0, u);
    diffuse(1, u, u0, visc, dt);
    SWAP(v0, v);
    diffuse(2, v, v0, visc, dt);
    project(u, v, u0, v0);
    SWAP(u0, u);
    SWAP(v0, v);
    advect(1, u, u0, u0, v0, dt);
    advect(2, v, v0, u0, v0, dt);
    project(u, v, u0, v0);
}

void draw_dens(sf::RenderWindow& window, float* dens) {
    sf::RectangleShape rect(sf::Vector2f(window.getSize().x / (N + 2), window.getSize().y / (N + 2)));
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            float d = dens[IX(i, j)];
            rect.setPosition(i * rect.getSize().x, j * rect.getSize().y);
            rect.setFillColor(sf::Color(255, 255, 255, d * 5));
            window.draw(rect);
        }
    }
}

static bool isMousePressed = false;

void get_from_UI(sf::RenderWindow& window, float* dens_prev, float* u_prev, float* v_prev) {
    static sf::Vector2i lastMousePosition;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!isMousePressed) {
            isMousePressed = true;
            lastMousePosition = sf::Mouse::getPosition(window);
        }

        sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f center(window.getSize().x / 2, window.getSize().y / 2);
        sf::Vector2f direction(currentMousePosition.x - center.x, currentMousePosition.y - center.y);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }

        int cx = N / 2;
        int cy = N / 2;
        float strength = 10.0f;

        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                float distance = std::sqrt((i - cx) * (i - cx) + (j - cy) * (j - cy));
                if (distance < 5) {
                    dens_prev[IX(i, j)] = strength;
                    u_prev[IX(i, j)] = direction.x * strength;
                    v_prev[IX(i, j)] = direction.y * strength;
                }
            }
        }
    }
    else {
        isMousePressed = false;
    }
}

int main() {
    float dt = 0.1f;
    float diff = 0.0001f;
    float visc = 0.0001f;

    float u[SIZE] = { 0 }, v[SIZE] = { 0 }, u_prev[SIZE] = { 0 }, v_prev[SIZE] = { 0 };
    float dens[SIZE] = { 0 }, dens_prev[SIZE] = { 0 };

    sf::RenderWindow window(sf::VideoMode(800, 800), "Fluid Simulation");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        get_from_UI(window, dens_prev, u_prev, v_prev);

        vel_step(u, v, u_prev, v_prev, visc, dt);
        dens_step(dens, dens_prev, u, v, diff, dt);

        window.clear();
        draw_dens(window, dens);
        window.display();
    }

    return 0;
}
