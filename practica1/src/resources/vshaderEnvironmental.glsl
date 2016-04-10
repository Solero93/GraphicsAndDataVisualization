#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 vPosition;
IN vec4 vNormal;
IN vec2 vCoordTextura;

OUT vec4 pos;
OUT vec4 norm;
OUT vec2 v_texcoord;

void main()
{
    gl_Position = vPosition;
    pos = vPosition;
    norm = vNormal;
    v_texcoord = vCoordTextura;
}
