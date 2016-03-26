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



void main(){
  gl_Position = vPosition;
  color = vNormal;
  //color = vColor;
}
