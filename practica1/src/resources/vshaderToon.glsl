#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 vPosition;
IN vec4 vNormal;

OUT vec4 pos;
OUT vec4 norm;

void main()
{
  gl_Position = vPosition;
  norm = vNormal;
  pos = vPosition;
}
