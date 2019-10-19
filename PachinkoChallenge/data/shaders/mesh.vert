#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 rotate;
    mat4 viewProj;
    vec4 color;
    vec4 additive;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 fragColor;
layout(location = 2) out vec4 fragNormal;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    vec4 transNorm = normalize(ubo.rotate * vec4(inNormal, 1.0));
    float nDot = abs(dot(vec3(0.5, 0.5, -1.0), transNorm.xyz));
    fragNormal = vec4(nDot, nDot, nDot, 1.0);

    gl_Position = ubo.viewProj * ubo.model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
    fragColor = ubo.color * ubo.additive;
}
