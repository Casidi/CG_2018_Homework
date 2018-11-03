#version 400 core
layout (location = 0) in vec3 aPos;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}