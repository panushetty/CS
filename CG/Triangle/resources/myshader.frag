// Each shader begins with a declaration of its version gl3.3 means 330 core gl4.2 means 420 core
#version 330 core
//vec3 means vector datatype 
layout(location=0) out vec4 out_color;
in vec3 v_color;

void main(){
	//create a color
	out_color=vec4(v_color,1); 
	
}