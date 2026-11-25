/*#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoord;
layout (location = 3) in vec3 atangent;
layout (location = 4) in vec3 abitangent;

layout (location = 5) in vec3 instanceoffset;

out vec3 fragpos;
out vec2 texcoord;
out mat3 tbn;

uniform mat4 umodel;
uniform mat4 uview;
uniform mat4 uproj;

void main()
{
    vec4 worldpos = umodel * vec4(apos + instanceoffset, 1.0);
    fragpos  = worldpos.xyz;
    texcoord = atexcoord;

    mat3 normalmat = mat3(transpose(inverse(umodel)));
    vec3 n = normalize(normalmat * anormal);
    vec3 t = normalize(normalmat * atangent);
    t = normalize(t - dot(t, n) * n);          // re-orthogonalize
    vec3 b = normalize(normalmat * abitangent);
    tbn = mat3(t, b, n);

    gl_position = uproj * uview * worldpos;
}*/

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (location = 5) in vec3 InstanceOffset;

out vec3 FragPos;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

// character facing: rotation about the Z (up) axis around uPivot (model space).
// Both default to 0, which is the identity, so the dungeon is unaffected.
uniform float uYaw;
uniform vec3  uPivot;

vec3 rotZ(vec3 v, float a)
{
    float c = cos(a), s = sin(a);
    return vec3(c * v.x - s * v.y, s * v.x + c * v.y, v.z);
}

void main()
{
    // rotate in model space first, then apply the instance offset
    vec3 localPos = rotZ(aPos - uPivot, uYaw) + uPivot;
    vec3 localN   = rotZ(aNormal,    uYaw);
    vec3 localT   = rotZ(aTangent,   uYaw);
    vec3 localB   = rotZ(aBitangent, uYaw);

    vec4 worldPos = uModel * vec4(localPos + InstanceOffset, 1.0);
    FragPos  = worldPos.xyz;
    TexCoord = aTexCoord;

    mat3 normalMat = mat3(transpose(inverse(uModel)));
    vec3 N = normalize(normalMat * localN);
    vec3 T = normalize(normalMat * localT);
    T = normalize(T - dot(T, N) * N);          // re-orthogonalize
    vec3 B = normalize(normalMat * localB);
    TBN = mat3(T, B, N);

    gl_Position = uProj * uView * worldPos;
}