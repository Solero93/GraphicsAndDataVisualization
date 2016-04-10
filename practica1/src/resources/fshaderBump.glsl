#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 pos;
IN vec4 norm;
IN vec2 v_texcoord;

uniform sampler2D texMapImg;
uniform sampler2D texMapNorm;
uniform vec3 llumAmbient;
uniform int numLlums;

struct MaterialBuffer {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform MaterialBuffer bufferMat;

struct LightsBuffer {
    vec4 position;
    vec4 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float angle;
    vec3 atenuate;
};

uniform LightsBuffer bufferLights[20];

vec4 calculateL(int);
vec4 calculateH(vec4);
float atenuateFactor(int,vec3);

void main()
{
    vec3 c = vec3(0.0, 0.0, 0.0);
    vec4 L, H, N=0.2*norm + 0.8*texture2D(texMapNorm, v_texcoord);
    vec3 diffuseTmp;
    float atenuation;
    for (int j=0; j<numLlums; j++){
        L = normalize(calculateL(j));
        H = normalize(calculateH(L));

        diffuseTmp = bufferMat.diffuse * bufferLights[j].diffuse * max(dot(L,N),0.0);

        atenuation = atenuateFactor(j, bufferLights[j].atenuate);

        c += diffuseTmp * atenuation + llumAmbient * bufferMat.diffuse;
    }
    gl_FragColor = 0.2*vec4(c[0],c[1],c[2],1.0) + 0.8*texture2D(texMapImg, v_texcoord);
  }

vec4 calculateL(int j){
    if (bufferLights[j].position == vec4(0.0, 0.0, 0.0, 0.0)) {
        return -(bufferLights[j].direction);
    } else if (bufferLights[j].angle == 0.0) {
        return bufferLights[j].position - pos;
    } else {
        vec4 rayDirection = normalize(pos - bufferLights[j].position);
        vec4 coneDirection = normalize(bufferLights[j].direction);

        float lightToSurfaceAngle = acos(dot(rayDirection, coneDirection));
        if (lightToSurfaceAngle > bufferLights[j].angle) {
            return vec4(0.0, 0.0, 0.0, 0.0);
        } else {
            return -rayDirection;
        }
    }
}

vec4 calculateH(vec4 L){
    vec4 F = vec4(0.0, 0.0, 10.0, 1.0); // Focus de l'observador
    vec4 V = normalize(F - pos);
    return L + V;
}

float atenuateFactor(int j, vec3 atenuate){
    vec4 rayDirection = bufferLights[j].position - pos;
    float a = atenuate[0], b = atenuate[1], c = atenuate[2];
    float d = length(rayDirection);
    return 1.0/(a + b*d + c*d*d);
}
