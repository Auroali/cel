#version 450 core
layout(location=0) out vec3 outPos;
layout(location=1) out vec3 outNormal;
layout(location=2) out vec4 outColour;

in vec3 WorldPos;
in vec3 Normal;

void main() {
    // vec3 lightPos = vec3(1.2, 1.0, 2.0);
    // vec3 lightColor = vec3(1,1,1);
    // vec3 objectColor = vec3(1,1,1);
    // vec3 ambient = objectColor * 0.1;
    
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - WorldPos); 
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;
    // vec3 result = (ambient + diffuse) * objectColor;
    outPos = vec3(1.0);
}