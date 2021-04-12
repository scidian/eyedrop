//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
//####################################################################################
//##    Basic Shader
//####################################################################################
@ctype mat4 hmm_mat4
@ctype vec3 hmm_vec3
@ctype vec4 hmm_vec4


//########## Vertex Shader ##########
@vs vs
uniform vs_params {
    mat4 mvp;
    mat4 m;
};

in vec4 pos;                // Vertex Position
in vec3 norm;               // Vertex Normal
in vec2 texcoord0;          // Vertex Texture Coordinate
in vec3 bary;               // Vertex Barycentric Coordinate

in vec4 instance_uv;        // Sprite Instance Atlas Coordinates
                            //  .x = uv0.x
                            //  .y = uv1.x
                            //  .z = uv0.y
                            //  .w = uv1.y

out vec2 uv;
out vec3 vert;
out vec3 vert_normal;
out vec3 vert_bary;

void main() {
    gl_Position = mvp * pos;
    uv = texcoord0;
    uv.x = instance_uv.x + (texcoord0.x * (instance_uv.y - instance_uv.x));
    uv.y = instance_uv.z + (texcoord0.y * (instance_uv.w - instance_uv.z));
    vert =          (m * vec4(pos.xyz, 1.0)).xyz;
    vert_normal =   (m * vec4(norm, 0.0)).xyz;
    vert_bary = bary;
}
@end


//########## Fragment Shader ##########
@fs fs
uniform sampler2D tex;

uniform fs_params {
    vec3  u_eye;
    float u_wireframe;
};

in vec2 uv;
in vec3 vert;
in vec3 vert_normal;
in vec3 vert_bary;

out vec4 frag_color;

void main() {

    // ***** Color from texture
    vec4  color_in  = texture(tex, uv);
    vec3  rgb_in    = color_in.xyz;
    float alpha_in  = color_in.a;//1.0;
    vec3  rgb_out   = rgb_in;
    float alpha_out = alpha_in;//1.0;


    // ***** Wireframe
    if (u_wireframe == 1.0) {
        float width = 1.0;

        vec3  d = fwidth(vert_bary);
        vec3  a3 = smoothstep(vec3(0.0), d * width, vert_bary);
        float wire = min(min(a3.x, a3.y), a3.z);
        rgb_out = rgb_in * (1.0 - wire);

        // If not on edge, draw texture faded
        if (rgb_out.x < 0.02 && rgb_out.y < 0.02 && rgb_out.z < 0.02) {
            // Texture is slightly there
            rgb_out = rgb_in * 0.8;
            alpha_out = alpha_in * 0.8;
        }
    }


    // ***** Shade Away
    // Calculate angle between camera vector and vertex normal for triangle shading
    float shade_away = 1.0;
    if (shade_away == 1.0) {
        vec3 eye = u_eye;
        float dp = dot(normalize(vert_normal), normalize(vert - eye)) + 0.15;
              dp = clamp(dp, 0.0, 1.0);
        rgb_out = mix(vec3(0.0), rgb_out, dp);
    }
    

    // ***** Set Final Color
    frag_color = vec4(rgb_out, alpha_out);
}
@end


//########## Shader Name ##########
@program basic_shader vs fs


