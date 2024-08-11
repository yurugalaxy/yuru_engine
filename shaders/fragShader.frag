#version 460 core

out vec4 fragColour;

in vec2 texCoord;

uniform sampler2D playerTex;
//uniform sampler2D bgTexture;

void main()
{
//  fragColour = mix(texture(playerTex, texCoord), texture(bgTexture, texCoord), 0.5);
  fragColour = texture(playerTex, texCoord);
};
