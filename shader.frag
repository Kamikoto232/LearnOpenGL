#version 330 core
in vec3 Color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 LightPosition;
in vec3 WorldVertPos;
in vec3 _WorldSpaceCameraPos;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPosition);
    vec3 viewDir = normalize(_WorldSpaceCameraPos - WorldVertPos);

    float lambert  = max(0.5*dot(norm, lightDir)+0.5,0.0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, norm), 0.0), 32) * lambert;
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2) * (lambert + spec);
}