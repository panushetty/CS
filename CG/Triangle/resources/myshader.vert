// Each shader begins with a declaration of its version gl3.3 means 330 core gl4.2 means 420 core
#version 330 core
//vec3 means vector datatype 
layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;

out vec3 v_color;

void main(){
	//create a position
	gl_Position=vec4(in_position,1);
	v_color=in_color;
}