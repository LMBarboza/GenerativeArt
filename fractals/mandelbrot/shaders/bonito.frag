#version 450 core
layout(location = 0) uniform float rect_width;
layout(location = 1) uniform float rect_height;
layout(location = 2) uniform vec2 area_w;
layout(location = 3) uniform vec2 area_h;
layout(location = 4) uniform uint max_iterations;
layout(location = 5) uniform vec2 inC;

out vec4 outColor;

vec3 getColor(float t) {
    vec3 darkPurple = vec3(0.1, 0.0, 0.2);  // Dark Purple
    vec3 pink = vec3(1.0, 0.2, 0.6);        // Pink
    vec3 white = vec3(1.0, 1.0, 1.0);       // White

    if (t < 0.5) {
        return mix(darkPurple, pink, t * 2.0);  // Interpolate between Dark Purple and Pink
    } else {
        return mix(pink, white, (t - 0.5) * 2.0);  // Interpolate between Pink and White
    }
}

void main() {

    const vec2 C = inC;
    vec2 Z = vec2(gl_FragCoord.x * (area_w.y - area_w.x) / rect_width  + area_w.x,
                        gl_FragCoord.y * (area_h.y - area_h.x) / rect_height + area_h.x);
    uint iteration = 0;

    while (iteration < max_iterations) {
        const float x = Z.x * Z.x - Z.y * Z.y + C.x;
        const float y = 2.0 * Z.x * Z.y       + C.y;

        if (x * x + y * y > 4.0)
            break;

        Z.x = x;
        Z.y = y;

        ++iteration;
    }

    float t = float(iteration) / float(max_iterations);
    vec3 color = getColor(t);
    outColor = vec4(color, 1.0);
}
