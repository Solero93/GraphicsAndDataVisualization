#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#define MAXLLUM 1
#endif

IN vec4 color;

void main()
{
    gl_FragColor = color;
}

