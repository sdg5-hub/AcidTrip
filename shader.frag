#version 330 core
in vec2 v_uv;
out vec4 FragColor;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_warp;
uniform float u_speed;
uniform int u_segments;
uniform float u_paletteSeed;

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 345.45));
    p += dot(p, p + 34.345);
    return fract(p.x * p.y);
}

float valueNoise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);

    float a = hash(i + vec2(0.0, 0.0));
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

float fbm(vec2 p) {
    float sum = 0.0;
    float amp = 0.5;
    for (int i = 0; i < 5; ++i) {
        sum += amp * valueNoise(p);
        p = p * 2.02 + vec2(17.12, 9.53);
        amp *= 0.5;
    }
    return sum;
}

vec2 kaleido(vec2 p, float segments) {
    float r = length(p);
    float a = atan(p.y, p.x);
    float sector = 6.28318530718 / segments;
    a = mod(a, sector);
    a = abs(a - sector * 0.5);
    return vec2(cos(a), sin(a)) * r;
}

vec3 palette(float t, float seed) {
    vec3 a = vec3(0.52, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.0, 0.33, 0.67) + vec3(seed * 0.013, seed * 0.021, seed * 0.034);
    return a + b * cos(6.28318 * (c * t + d));
}

float sparkle(vec2 p, float t) {
    vec2 cell = floor(p * 80.0);
    float rnd = hash(cell + floor(t * 0.8));
    float starOn = step(0.992, rnd);
    vec2 local = fract(p * 80.0) - 0.5;
    float glint = 0.006 / (dot(local, local) + 0.0004);
    float twinkle = 0.5 + 0.5 * sin(t * 12.0 + rnd * 18.0);
    return starOn * glint * twinkle;
}

float scene(vec2 p, float t, float warp) {
    vec2 flow = vec2(
        fbm(p * 2.2 + vec2(t * 0.21, -t * 0.13)),
        fbm(p * 2.1 + vec2(-t * 0.16, t * 0.18))
    );

    p += (flow - 0.5) * 1.3 * warp;

    float rings = sin(length(p) * 12.5 - t * 4.2 + fbm(p * 3.4) * 6.0);
    float swirls = sin((p.x + p.y) * 8.0 + t * 3.1 + fbm(p * 4.8) * 7.0);
    float density = fbm(p * 5.5 - t * 0.5);

    return 0.4 * rings + 0.35 * swirls + 0.7 * density;
}

void main() {
    vec2 uv = (gl_FragCoord.xy / u_resolution.xy) * 2.0 - 1.0;
    uv.x *= u_resolution.x / u_resolution.y;

    float mouseInfluence = (u_mouse.x / max(u_resolution.x, 1.0) - 0.5) * 0.3;
    float t = u_time * u_speed + mouseInfluence;

    vec2 k = kaleido(uv, float(u_segments));

    float shift = 0.013 * u_warp;
    float rScene = scene(k + vec2(shift, 0.0), t, u_warp);
    float gScene = scene(k, t, u_warp);
    float bScene = scene(k - vec2(shift, 0.0), t, u_warp);

    vec3 col = vec3(rScene, gScene, bScene);
    col = palette(col.x * 0.22 + col.y * 0.14 + t * 0.08, u_paletteSeed);

    float spark = sparkle(k * 0.5 + 0.5, t);
    col += vec3(0.9, 1.0, 1.2) * spark;

    float scan = 0.96 + 0.04 * sin(gl_FragCoord.y * 1.4 + t * 22.0);
    float flicker = 0.985 + 0.015 * sin(t * 57.0 + fbm(uv * 4.0) * 6.0);

    col *= scan * flicker;
    col = pow(max(col, 0.0), vec3(0.9));

    FragColor = vec4(col, 1.0);
}
