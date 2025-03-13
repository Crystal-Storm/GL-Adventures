#version 330 core
out vec4 FragColor;

void main() {
    // Normalize screen coordinates to [0, 1]
    vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0); // Replace 800x600 with your window size

    // Create a gradient based on the pixel's position
    FragColor = vec4(uv.x, uv.y, 0.5, 1.0); // Red and green vary with position, blue is fixed
}