// Get the canvas element and WebGL context
const canvas = document.getElementById('webgl-canvas');
const gl = canvas.getContext('webgl');

let mouseX = 0;
let mouseY = 0;
canvas.addEventListener('mousemove', (event) => {
    mouseX = event.clientX;
    mouseY = canvas.height - event.clientY; // Invert Y to match WebGL coordinate system
});

// Vertex shader source code
const vertexShaderSource = `
    attribute vec4 a_Position;
    void main() {
        gl_Position = a_Position;
    }
`;

// Fragment shader source code (GLSL code to compute each pixel color)
const fragmentShaderSource = `
    precision mediump float;
    uniform vec2 iResolution;
    uniform float iTime;
    uniform vec2 iMouse;
    void main() {
        vec2 uv = gl_FragCoord.xy / iResolution;
        vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
        gl_FragColor = vec4(col, 1.0);

        float a = 2., b = 0.5;
        float U = iTime * 5.;
        mat3 mRotate;
        mRotate[0] = vec3(cos(U),      sin(U),          0.);
        mRotate[1] = vec3(-sin(U),     cos(U),          0.);
        mRotate[2] = vec3(0.,          0.,              1.);
        
        mat3 mTranslate;
        mTranslate[0] = vec3(1., 0., 0.);
        mTranslate[1] = vec3(0., 1., 0.);
        mTranslate[2] = vec3(-iResolution.x + cos(iTime * 5.) * 200., -iResolution.y + sin(iTime * 5.) * 200., 1.);
        
        vec3 p_rotate = mRotate*mTranslate*vec3(gl_FragCoord);
        if(abs(p_rotate.x) + abs(p_rotate.y) < 200.) {
            col = 0.5 - 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
            gl_FragColor = vec4(col, 1.);
        }
    }
`;

// Function to create a shader
function createShader(gl, type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        console.error('Error compiling shader:', gl.getShaderInfoLog(shader));
        gl.deleteShader(shader);
        return null;
    }
    return shader;
}

// Create vertex and fragment shaders
const vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
const fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);

// Create the shader program
const program = gl.createProgram();
gl.attachShader(program, vertexShader);
gl.attachShader(program, fragmentShader);
gl.linkProgram(program);
if(!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    console.error('Error linking program:', gl.getProgramInfoLog(program));
} else {
    gl.useProgram(program);
}

// Define the geometry and store it in a buffer
const vertices = new Float32Array([
    -1.0, -1.0,
     1.0, -1.0,
    -1.0,  1.0,
     1.0,  1.0
]);
const buffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

// Bind the buffer and set the attribute
const positionLocation = gl.getAttribLocation(program, 'a_Position');
gl.enableVertexAttribArray(positionLocation);
gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);

// Get the location of the uniforms
const resolutionLocation = gl.getUniformLocation(program, 'iResolution');
const timeLocation = gl.getUniformLocation(program, 'iTime');
const mouseLocation = gl.getUniformLocation(program, 'iMouse');

// Set the resolution uniform
gl.uniform2f(resolutionLocation, canvas.width, canvas.height);

// Render the scene
function render(time) {
    // Convert time to seconds
    const timeInSeconds = time * 0.001;
    // Set the time uniform
    gl.uniform1f(timeLocation, timeInSeconds);
    gl.uniform2f(mouseLocation, mouseX*2, mouseY*2);

    // Clear the canvas
    gl.clear(gl.COLOR_BUFFER_BIT);

    // Draw the rectangle
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

    // Request the next frame
    requestAnimationFrame(render);
}

// Start the rendering loop
requestAnimationFrame(render);
