#version 330

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_normal;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    FragPos = vec3(model_mat * vec4(vertex_position, 1.0));
    Normal = mat3(transpose(inverse(model_mat))) * vertex_normal;
    TexCoord = vertex_texcoord;
    
    gl_Position = proj_mat * view_mat * vec4(FragPos, 1.0);
}