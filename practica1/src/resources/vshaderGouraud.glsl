#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 vPosition;
IN vec4 vColor;
IN vec4 vNormal;
IN vec2 vCoordTextura;

OUT vec2 v_texcoord;
OUT vec4 color;

uniform vec3 llumAmbient;
uniform int numLlums;
uniform sampler2D texMap;

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
float atenuateFactor(int, vec3);

void main()
{
  color = vec4(1.0,0.0,0.0,1.0);
  gl_Position = vPosition;
  vec3 c = vec3(0.0, 0.0, 0.0);
  vec4 L, H, N=vNormal;
  vec3 diffuseTmp, specularTmp, ambientTmp;
  float atenuation;
  for (int j=0; j<numLlums; j++){
      L = normalize(calculateL(j));
      H = normalize(calculateH(L));

      diffuseTmp = bufferMat.diffuse * bufferLights[j].diffuse * max(dot(L,N),0.0);
      specularTmp = bufferMat.specular * bufferLights[j].specular * pow(max(dot(N,H),0.0), bufferMat.shininess);
      ambientTmp = bufferMat.ambient * bufferLights[j].ambient;

      atenuation = atenuateFactor(j, bufferLights[j].atenuate);

      c += (diffuseTmp + specularTmp + ambientTmp) * atenuation + llumAmbient * bufferMat.diffuse;
  }
  v_texcoord = vCoordTextura;
  color = vec4(c[0],c[1],c[2],1.0);
}


vec4 calculateL(int j) {
    if (bufferLights[j].position == vec4(0.0, 0.0, 0.0, 0.0)) {
        return -(bufferLights[j].direction);
    } else if (bufferLights[j].angle == 0.0) {
        return bufferLights[j].position - gl_Position;
    } else {
        vec4 rayDirection = normalize(gl_Position - bufferLights[j].position);
        vec4 coneDirection = normalize(bufferLights[j].direction);

        float lightToSurfaceAngle = acos(dot(rayDirection, coneDirection));
        if (lightToSurfaceAngle > bufferLights[j].angle) {
            return vec4(0.0, 0.0, 0.0, 0.0);
        } else {
            return -rayDirection;
        }
    }
}

vec4 calculateH(vec4 L) {
    vec4 F = vec4(0.0, 0.0, 10.0, 1.0); // Focus de l'observador
    vec4 V = normalize(F - gl_Position);
    return L + V;
}

float atenuateFactor(int j, vec3 atenuate) {
    vec4 rayDirection = bufferLights[j].position - gl_Position;
    float a = atenuate[0], b = atenuate[1], c = atenuate[2];
    float d = length(rayDirection);
    return 1.0/(a + b*d + c*d*d);
}
