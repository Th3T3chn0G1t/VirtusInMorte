#version 410 core

#pragma vertex

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in vec2 v_UV;
layout(location = 3) in vec3 v_Normal;

layout(location = 4) in mat4 i_Transform;
// layout(location = 5) ...;
// layout(location = 6) ...;
// layout(location = 7) ...;
layout(location = 8) in mat3 i_Normal;
// layout(location = 9) ...;
// layout(location = 10) ...;
layout(location = 11) in uint i_SpecularStrength;
layout(location = 12) in float i_Shininess;
layout(location = 13) in uint i_DoSample;

uniform mat4 u_VP;
uniform float u_Time;
uniform vec3 u_AmbientLight;
uniform vec3 u_ViewPosition;

out vec4 o_Color;
out vec2 o_UV;
flat out uint o_DoSample;

void main() {
    vec3 test_light_color = vec3(1.0, 0.0, 0.0);
    vec3 test_light_position = vec3(cos(u_Time), 1.0, sin(u_Time));

    vec4 model = i_Transform * vec4(v_Position, 1.0);

    vec3 light_direction = normalize(test_light_position - model.xyz);
    vec3 normal = normalize(i_Normal * v_Normal);

    vec3 view_direction = normalize(u_ViewPosition - model.xyz);
    vec3 reflect_direction = reflect(-light_direction, normal);  

    float specular_component = pow(max(dot(view_direction, reflect_direction), 0.0), i_SpecularStrength);
    vec3 specular = i_Shininess * specular_component * test_light_color;  

    vec3 diffuse = test_light_color * max(dot(normal, light_direction), 0.0);

    gl_Position = u_VP * model;
  
    o_Color = v_Color * vec4(u_AmbientLight + diffuse + specular, 1.0);
    o_UV = v_UV;
    o_DoSample = i_DoSample;
}
