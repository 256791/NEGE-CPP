#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTextureCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 normal;
out vec3 position;
out vec3 textureCoords;

void main() {
    mat4 MV = uView * uModel;
    mat4 MVP = uProjection * MV;
    normal = vec3(uModel * vec4(aNormal, 0));
    position = vec3(uModel * aPosition);
    textureCoords = aTextureCoords;

    gl_Position = MVP * aPosition;
}