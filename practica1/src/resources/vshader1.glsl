#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 vPosition;
IN vec4 vColor;

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
  color = vec4(bufferLights[2].ambient[0],bufferLights[2].ambient[1],bufferLights[2].ambient[2],1.0);
  //color = vec4(bufferMat.ambient[0],bufferMat.ambient[1],bufferMat.ambient[2],1.0);
  //color = vColor;
}
