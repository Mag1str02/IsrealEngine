#version 410 core

struct VP{
    mat4 view;
    mat4 projection;
};

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec3 i_Tangent;
layout(location = 3) in vec2 i_TexCoords;
layout(location = 4) in mat4 i_Transform;

uniform VP u_Camera;

out VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPos;
    vec2 TexCoords;
    mat3 TBNMatrix;
}
vs_out;

void main()
{  
    mat4 view = -inverse(u_Camera.view) ;
    mat3 normal_matrix = mat3(transpose(inverse(i_Transform))); 
    vec3 tangent = normalize(normal_matrix * i_Tangent);
    vec3 normal = normalize(normal_matrix * i_Normal);
    vec3 bitangent = cross(normal, tangent);

    gl_Position      = u_Camera.projection * u_Camera.view * i_Transform * vec4(i_Pos, 1.0);
    vs_out.FragPos   = vec3(i_Transform * vec4(i_Pos, 1.0));
    vs_out.Normal    = normal;
    vs_out.TexCoords = i_TexCoords;
    vs_out.CameraPos = -vec3(view[3][0],view[3][1],view[3][2]);
    vs_out.TBNMatrix = mat3(tangent, bitangent, normal);
}