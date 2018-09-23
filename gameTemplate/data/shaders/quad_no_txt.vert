#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 viewProj;
    vec4 color;
    vec4 additive;
} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 fragColor;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    gl_Position = ubo.viewProj * ubo.model * vec4(inPosition, 1.0);
    fragColor = ubo.color * ubo.additive;
}
