#version 460

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 lightColour;

out vec4 fragColour;

void main() {
  //Ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

  //Diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = lightColour * light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

  //Specular
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = lightColour * light.specular * spec * vec3(texture(material.specular, texCoord));

  fragColour = vec4(ambient + diffuse + specular, 1.0);
}