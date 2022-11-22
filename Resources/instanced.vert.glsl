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
uniform vec3 u_LightPositions[128];
uniform vec3 u_LightColors[128];
uniform uint u_LightCount;

out vec4 o_Color;
out vec2 o_UV;
flat out uint o_DoSample;

void main() {
    vec4 model = i_Transform * vec4(v_Position, 1.0);

    vec3 calculated = vec3(0.0);
    for(uint i = 0; i < u_LightCount; ++i) {
        vec3 light_direction = normalize(u_LightPositions[i] - model.xyz);
        vec3 normal = normalize(i_Normal * v_Normal);

        vec3 view_direction = normalize(u_ViewPosition - model.xyz);
        vec3 reflect_direction = reflect(-light_direction, normal);  

        float specular_component = pow(max(dot(view_direction, reflect_direction), 0.0), i_SpecularStrength);
        vec3 specular = i_Shininess * specular_component * u_LightColors[i];  

        vec3 diffuse = u_LightColors[i] * max(dot(normal, light_direction), 0.0);

        calculated += diffuse + specular;
    }

    gl_Position = u_VP * model;
  
    o_Color = v_Color * vec4(u_AmbientLight + calculated, 1.0);
    o_UV = v_UV;
    o_DoSample = i_DoSample;
}
