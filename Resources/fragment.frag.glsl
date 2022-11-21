#version 410 core

#pragma fragment

in vec4 o_Color;
in vec2 o_UV;
flat in uint o_DoSample;

uniform sampler2D u_Texture;
uniform float u_Time;
uniform vec2 u_Viewport;

out vec4 f_Color;

void main() {
    vec2 UV = gl_FragCoord.xy / u_Viewport;

    vec4 sampled[] = vec4[](
        vec4(1.0),
        texture(u_Texture, o_UV)
    );

    f_Color = sampled[o_DoSample] * o_Color;
    // f_Color = texture(u_Texture, o_UV) * o_Color;
    // f_Color = vec4(o_Normal * 0.5 + 0.5, 1.0);
    // f_Color = vec4(1.0);
}
