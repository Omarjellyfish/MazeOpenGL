#version 330

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

// Material
uniform sampler2D texture1;
uniform bool useTexture;
uniform vec4 solidColor;

// Camera
uniform vec3 viewPos;

// Lighting (increased to support more lights)
#define MAX_LIGHTS 100  // Changed from 10 to 100
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform int numLights;
uniform PointLight lights[MAX_LIGHTS];

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Combine
    vec3 ambient = light.ambient * baseColor;
    vec3 diffuse = light.diffuse * diff * baseColor;
    vec3 specular = light.specular * spec * vec3(0.5); // Specular highlight
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 baseColor;
    if (useTexture) {
        baseColor = texture(texture1, TexCoord).rgb;
    } else {
        baseColor = solidColor.rgb;
    }
    
    vec3 result = vec3(0.0);
    
    // Calculate lighting from all lights
    for(int i = 0; i < numLights; i++) {
    if (i >= MAX_LIGHTS) break;
    result += CalculatePointLight(lights[i], norm, FragPos, viewDir, baseColor);
}
    
    // Add minimal ambient if no lights
    if (numLights == 0) {
        result = baseColor * 1.0;
    }
    
    FragColor = vec4(result, 1.0);
}