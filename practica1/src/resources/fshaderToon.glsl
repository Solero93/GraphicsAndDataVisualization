#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

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

void main(){
    float intensity;
    vec4 colorv2;
    intensity = dot(-bufferLights[0].direction,normalize(norm));

    if (intensity > 0.95){
        colorv2 = vec4(1.0,0.5,0.5,1.0);
    }else if (intensity > 0.65){
        colorv2 = vec4(0.6,0.3,0.3,1.0);
    }else if (intensity > 0.25){
        colorv2 = vec4(0.4,0.2,0.2,1.0);
    }else{
    colorv2 = vec4(0.2,0.1,0.1,1.0);
    }
    gl_FragColor = colorv2;
}
