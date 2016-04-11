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
    vec4 normalAlt = texture2D(texMapNorm, v_texcoord); // Last coordinate is 1, must change to 0
    vec4 L, H, N=0.2*normalize(norm) + 0.8*vec4(normalAlt[0],normalAlt[1],normalAlt[2],0.0);
    vec3 diffuseTmp, specularTmp, ambientTmp;
    float atenuation;
    vec4 textura = texture2D(texMapImg, v_texcoord);
    for (int j=0; j<numLlums; j++){
        L = normalize(calculateL(j));
        H = normalize(calculateH(L));

        diffuseTmp = (0.2*bufferMat.diffuse + 0.8*textura.xyz)
                * bufferLights[j].diffuse * max(dot(L,N),0.0);
        specularTmp = bufferMat.specular * bufferLights[j].specular * pow(max(dot(N,H),0.0), bufferMat.shininess);
        ambientTmp = bufferMat.ambient * bufferLights[j].ambient;

        atenuation = atenuateFactor(j, bufferLights[j].atenuate);

        c += (diffuseTmp + specularTmp + ambientTmp) * atenuation + llumAmbient * bufferMat.ambient;
    }
    gl_FragColor = vec4(c[0],c[1],c[2],1.0);
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
