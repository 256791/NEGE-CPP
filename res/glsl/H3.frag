#version 330 core

uniform vec3 uCameraPosition;

struct Material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Ni;
    float Ns;
    float d;
    int illum;
};

in vec3 normal;
in vec3 position;
in vec3 textureCoords;
uniform Material uCurrentMaterial;

struct PointLight {
    vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 color;
    vec3 direction;
};

struct Spotlight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float focus;
    float angle;
};

float PoincareDistance(vec3 u, vec3 v) {
    vec3 uv = u - v;
    float uvn = dot(uv, uv);
    return acosh(1 + ((2 * uvn) / ((1 - dot(u, u)) * (1 - dot(v, v)))));
}

// vec3 EinsteinAddition(vec3 v, vec3 u) {
vec3 EinsteinAddition(vec3 u, vec3 v) {
    float gammaI = sqrt(1.f - dot(u, u));
    float gamma = 1.f / gammaI;
    return 1.f / (1.f + dot(u, v)) * ((gammaI) * v + u * (1 + (gamma / (1.f + gamma)) * dot(u, v)));
}

vec3 Specular(vec3 normal, vec3 viewDirection, vec3 lightDirection, vec3 lightColor, float shininess) {
    vec3 reflectionDirection = normalize(reflect(-lightDirection, normal));
    float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0), shininess);
    return lightColor * specular;
}

vec3 Diffuse(vec3 normal, vec3 lightDirection, vec3 lightColor) {
    return max(dot(normal, lightDirection), 0.0) * lightColor;
}

#define MAX_POINT_LIGHT_COUNT 8
#define MAX_DIRECTIONAL_LIGHT_COUNT 4
#define MAX_SPOT_LIGHT_COUNT 4

uniform int light_spot_arrsize;
uniform Spotlight[MAX_SPOT_LIGHT_COUNT] light_spot;
uniform int light_directional_arrsize;
uniform DirectionalLight[MAX_DIRECTIONAL_LIGHT_COUNT] light_directional;
uniform int light_point_arrsize;
uniform PointLight[MAX_POINT_LIGHT_COUNT] light_point;

const vec3 ambientLight = vec3(0.1);
// const vec3 ambientLight = vec3(0.0);

void main() {
    vec3 nnormal = normalize(normal);
    vec3 camPos = uCameraPosition;
    if(uCurrentMaterial.illum == 0) {
        gl_FragColor = vec4(uCurrentMaterial.Ka, 1.0f);
    }
    else if(uCurrentMaterial.illum == 1) 
    // {
    //     vec3 diff = vec3(0);
    //     for(int i = 0; i < light_point_arrsize; i++) {
    //         vec3 dir = normalize(EinsteinAddition(light_point[i].position, -position));
    //         diff += Diffuse(nnormal, dir, light_point[i].color);
    //     }
    //     for(int i = 0; i < light_directional_arrsize; i++) {
    //         diff += Diffuse(nnormal, light_directional[i].direction, light_directional[i].color);
    //     }
    //     gl_FragColor = vec4(ambientLight * uCurrentMaterial.Ka + uCurrentMaterial.Kd * diff, uCurrentMaterial.d);
    // } 
    // else if(uCurrentMaterial.illum == 2) {
    //     vec3 diff = vec3(0);
    //     vec3 spec = vec3(0);
    //     vec3 viewDir = normalize(EinsteinAddition(camPos, -position));
    //     for(int i = 0; i < light_point_arrsize; i++) {
    //         vec3 dir = normalize(EinsteinAddition(light_point[i].position, -position));
    //         diff += Diffuse(nnormal, dir, light_point[i].color);
    //         spec += Specular(nnormal, viewDir, dir, light_point[i].color, uCurrentMaterial.Ns);
    //     }
    //     for(int i = 0; i < light_directional_arrsize; i++) {
    //         diff += Diffuse(nnormal, light_directional[i].direction, light_directional[i].color);
    //         spec += Specular(nnormal, viewDir, light_directional[i].direction, light_directional[i].color, uCurrentMaterial.Ns);
    //     }
    //     gl_FragColor = vec4(ambientLight * uCurrentMaterial.Ka + uCurrentMaterial.Kd * diff + uCurrentMaterial.Ks * spec, uCurrentMaterial.d);
    // } else  {
    //     vec3 diff = vec3(0);
    //     vec3 spec = vec3(0);
    //     vec3 viewDir = normalize(EinsteinAddition(camPos, -position));
    //     for(int i = 0; i < light_point_arrsize; i++) {
    //         vec3 dir = normalize(EinsteinAddition(light_point[i].position, -position));
    //         diff += Diffuse(nnormal, dir, light_point[i].color);
    //         spec += Specular(nnormal, viewDir, dir, light_point[i].color, uCurrentMaterial.Ns);
    //     }
    //     for(int i = 0; i < light_directional_arrsize; i++) {
    //         diff += Diffuse(nnormal, light_directional[i].direction, light_directional[i].color);
    //         spec += Specular(nnormal, viewDir, light_directional[i].direction, light_directional[i].color, uCurrentMaterial.Ns);
    //     }
    //     gl_FragColor = vec4(ambientLight * uCurrentMaterial.Ka + uCurrentMaterial.Kd * diff + uCurrentMaterial.Ks * spec, uCurrentMaterial.d);
    // }

    {
        vec3 diff = vec3(0);
        for(int i = 0; i < light_point_arrsize; i++) {
            vec3 dir = normalize(EinsteinAddition(light_point[i].position, -position));
            float dist = length(EinsteinAddition(light_point[i].position, -position));
            float strength = 1.0 / (light_point[i].constant + dist * light_point[i].linear + (dist * dist) * light_point[i].quadratic);
            diff += strength * Diffuse(nnormal, dir, light_point[i].color);
        }
        for(int i = 0; i < light_directional_arrsize; i++) {
            diff += Diffuse(nnormal, light_directional[i].direction, light_directional[i].color);
        }
        gl_FragColor = vec4(ambientLight * uCurrentMaterial.Ka + uCurrentMaterial.Kd * diff, uCurrentMaterial.d);
    } else {
        vec3 diff = vec3(0);
        vec3 spec = vec3(0);
        vec3 viewDir = normalize(EinsteinAddition(camPos, -position));
        for(int i = 0; i < light_point_arrsize; i++) {
            vec3 dir = normalize(EinsteinAddition(light_point[i].position, -position));
            float dist = length(EinsteinAddition(light_point[i].position, -position));
            float strength = 1.0 / (light_point[i].constant + dist * light_point[i].linear + (dist * dist) * light_point[i].quadratic);

            diff += strength * Diffuse(nnormal, dir, light_point[i].color);
            spec += strength * Specular(nnormal, viewDir, light_directional[i].direction, light_directional[i].color, uCurrentMaterial.Ns);
        }
        for(int i = 0; i < light_directional_arrsize; i++) {
            diff += Diffuse(nnormal, light_directional[i].direction, light_directional[i].color);
            spec += Specular(nnormal, viewDir, light_directional[i].direction, light_directional[i].color, uCurrentMaterial.Ns);
        }
        gl_FragColor = vec4(ambientLight * uCurrentMaterial.Ka + uCurrentMaterial.Kd * diff + uCurrentMaterial.Ks * spec, uCurrentMaterial.d);
    }
}