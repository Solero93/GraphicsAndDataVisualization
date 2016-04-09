#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 diffuseColor;
IN vec2 v_texcoord;

uniform sampler2D texMapImg;

void main()
{
    gl_FragColor = 0.2*diffuseColor + 0.8*texture2D(texMapImg, v_texcoord);
}
