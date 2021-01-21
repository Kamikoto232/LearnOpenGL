#version 330 core
layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

//out vec3 Position_worldspace;
out vec2 TexCoord;
out vec3 Normal;
out vec3 LightPosition;
out vec3 WorldVertPos;  
out vec3 _WorldSpaceCameraPos;  

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;
uniform vec3 WorldSpaceLightPos0;
uniform vec3 ambient;
uniform vec3 WorldSpaceCameraPos;

void main()
{
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
    // Позиция вершины в пространстве камеры :МВП * положение
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    LightPosition = WorldSpaceLightPos0;
    Normal = mat3(transpose(inverse(M))) * normal; // Будет работать лишь в том случае, когда матрица модели не изменяет её размер.
    WorldVertPos = vec3(M * vec4(vertexPosition_modelspace,1));
    _WorldSpaceCameraPos = WorldSpaceCameraPos;
}