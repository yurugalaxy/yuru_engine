#version 460

in vec3 fragPos;
in vec3 normal;

uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 fragColour;

void main() {
  float ambientStrength = 0.1;
  float specularStrength = 0.5;
  vec3 ambient = ambientStrength * lightColour;
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColour;
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColour;

  vec3 result = (ambient + diffuse + specular) * objectColour;
  fragColour = vec4(result, 1.0);
}