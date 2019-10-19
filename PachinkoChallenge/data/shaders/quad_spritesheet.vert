#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 viewProj;
    vec4 color;
    vec4 additive;
    vec4 glyphRect;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 fragColor;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    gl_Position = ubo.viewProj * ubo.model * vec4(inPosition, 1.0);

    // Init the UV to the sprite sheet coordinates for this glyph
    fragTexCoord.x = ubo.glyphRect.x + (inUV.x * ubo.glyphRect.z);
    fragTexCoord.y = ubo.glyphRect.y + (inUV.y * ubo.glyphRect.w);

    fragColor = ubo.color * ubo.additive;
}
