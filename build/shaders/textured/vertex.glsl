varying vec2 vTexCoord;
uniform mat4 MVP;

void main(void)
{
   vTexCoord = gl_MultiTexCoord0.xy;
   gl_Position = MVP * gl_Vertex;
}
