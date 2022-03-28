#version 330 core

in VS_OUT
{
    vec2 near_far;
    vec3 light_pos;
    vec4 frag_pos;
    vec2 lightDistance;
}fs_in;

void main()
{             
    //gl_FragDepth = gl_FragCoord.z;
    float lightDistance = length(fs_in.frag_pos.xyz - fs_in.light_pos);
    lightDistance /= fs_in.near_far.y;

    lightDistance = ( lightDistance + fs_in.lightDistance.x) / 2;

    gl_FragDepth = fs_in.lightDistance.x;
}