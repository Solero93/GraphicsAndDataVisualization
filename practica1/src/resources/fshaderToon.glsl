#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 pos;
IN vec4 norm;

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

vec4 calculateV(void);

void main(){
    vec4 colorv2, focus = vec4(0.0, 0.0, 10.0, 1.0);
    vec4 silhuette = vec4(bufferMat.diffuse[0],bufferMat.diffuse[1],bufferMat.diffuse[2],1.0);
    float intensity = dot(normalize(calculateV()),normalize(norm));
    float atenuation = 1.0 - dot(normalize(focus - pos), normalize(norm));

    if (intensity > 0.95) {
        colorv2 = vec4(1.0,0.5,0.5,1.0);
    } else if (intensity > 0.65) {
        colorv2 = vec4(0.6,0.3,0.3,1.0);
    } else if (intensity > 0.25) {
        colorv2 = vec4(0.4,0.2,0.2,1.0);
    } else {
        colorv2 = vec4(0.2,0.1,0.1,1.0);
    }
    gl_FragColor =  silhuette * atenuation + colorv2;
}

vec4 calculateV(){
    vec3 noColor = vec3(0.0, 0.0, 0.0);
    vec4 noVector = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i=0; i<numLlums; i++){
        // Check which light is turnt on
        if (bufferLights[i].ambient != noColor ||
                bufferLights[i].diffuse != noColor ||
                bufferLights[i].specular != noColor){
            if (bufferLights[i].direction == noVector) {
                return bufferLights[i].position - pos;
            } else {
                return -bufferLights[i].direction;
            }
        }
    }
    return noVector;
}
