uniform mat4 MVP;

void main(void){
   gl_Position = MVP * gl_Vertex;
}
