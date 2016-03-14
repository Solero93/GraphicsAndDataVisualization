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

OUT vec4 color;

struct MaterialBuffer {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform MaterialBuffer bufferMat;

struct LightsBuffer {
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float angle;
};

uniform LightsBuffer bufferLights[5];

void main()
{
  gl_Position = vPosition;
  int c[3];
  for (int i=0; i<3; i++){
      // TODO fix formula
    c[i] = bufferMat.diffuse[i] * bufferLights[0].diffuse[i] * max(dot(L,N),0.0) +
            bufferMat.specular[i] * bufferLights[0].specular[i] * (max(dot(N,H),0.0)**bufferMat.shininess) +
            bufferMat.ambient[i] * bufferLights[0].ambient[i];
  }
  color = vec4(c[0],c[1],c[2],1.0);
  //color = vec4(bufferLights[2].ambient[0],bufferLights[2].ambient[1],bufferLights[2].ambient[2],1.0);
  //color = vec4(bufferMat.ambient[0],bufferMat.ambient[1],bufferMat.ambient[2],1.0);
  //color = vColor;
}
