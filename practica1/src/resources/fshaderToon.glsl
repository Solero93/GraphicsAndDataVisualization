#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 pos;
IN vec4 norm;

uniform vec3 llumAmbient;
uniform int numLlums;

struct LightsBuffer {
    vec4 position;
    vec4 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float angle;
};

uniform LightsBuffer bufferLights[20];

struct MaterialBuffer {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform MaterialBuffer bufferMat;

vec4 calculateL(int);

void main(){
    vec4 focus = vec4(0.0, 0.0, 10.0, 1.0);
    vec3 colorv2;
    float normalVisionAngleCos = dot(normalize(focus - pos), norm);
    float intensity = dot(normalize(calculateL(0)), norm);

    if (intensity > 0.95) {
        colorv2 = vec3(1.0,0.5,0.5);
    } else if (intensity > 0.65) {
        colorv2 = vec3(0.6,0.3,0.3);
    } else if (intensity > 0.25) {
        colorv2 = vec3(0.4,0.2,0.2);
    } else {
        colorv2 = vec3(0.2,0.1,0.1);
    }
    vec3 colorFinal = colorv2 * (normalVisionAngleCos);
    gl_FragColor = vec4(colorFinal[0], colorFinal[1], colorFinal[2], 1.0);
}

vec4 calculateL(int i){
    vec3 noColor = vec3(0.0, 0.0, 0.0);
    vec4 noVector = vec4(0.0, 0.0, 0.0, 0.0);
    if (bufferLights[i].position == noVector) {
        return -bufferLights[i].direction;
    } else if(bufferLights[i].angle == 0.0) {
        return bufferLights[i].position - pos;
    } else {
        vec4 rayDirection = normalize(pos - bufferLights[i].position);
        vec4 coneDirection = normalize(bufferLights[i].direction);

        float lightToSurfaceAngle = acos(dot(rayDirection, coneDirection));
        if (lightToSurfaceAngle > bufferLights[i].angle) {
            return noVector;
        } else {
            return -rayDirection;
        }
    }
    return noVector;
}
