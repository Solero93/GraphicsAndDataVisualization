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

uniform MaterialBuffer buffer;


void main()
{
  gl_Position = vPosition;
  color = vec4(buffer.ambient[0],buffer.ambient[1],buffer.ambient[2],1.0);
  //color = vColor;
}
