#version 410 core

in vec4 o_Color;
in vec2 o_UV;
in vec3 o_Normal;

uniform sampler2D u_Texture;
uniform float u_Time;
uniform vec2 u_Viewport;

out vec4 f_Color;

void main() {
    vec2 UV = gl_FragCoord.xy / u_Viewport;

    // f_Color = texture(u_Texture, o_UV) * o_Color;
    f_Color = vec4(o_Normal * 0.5 + 0.5, 1.0);
}
