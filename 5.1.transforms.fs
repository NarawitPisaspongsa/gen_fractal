#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform float centerX;
uniform float centerY;
uniform float zoom;
uniform float time;
uniform int maxIter;

// Mandelbrot function using standard floats
int mandelbrot(vec2 c) {
    vec2 z = vec2(0.0);
    for (int i = 0; i < maxIter; i++) {
        // dot(z, z) is the same as z.x*z.x + z.y*z.y
        if (dot(z, z) > 4.0) return i;
        z = vec2(z.x * z.x - z.y * z.y + c.x, 2.0 * z.x * z.y + c.y);
    }
    return maxIter;
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    // Map TexCoords to -1.0 to 1.0 range and adjust aspect ratio
    vec2 uv = (TexCoord * 2.0 - 1.0);
    uv.x *= 800.0 / 600.0;
    
    // Use float precision for the coordinate calculation
    vec2 c = vec2(centerX + uv.x / zoom, centerY + uv.y / zoom);
    
    int iter = mandelbrot(c);
    
    // Color mapping
    float smooth_iter = float(iter) / float(maxIter);
    vec3 color = hsv2rgb(vec3(mod(smooth_iter * 5.0 + time * 0.1, 1.0), 0.8, smooth_iter));
    
    FragColor = vec4(color, 1.0);
}