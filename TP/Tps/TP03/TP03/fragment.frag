#version 330 core

in vec2 vertexTexCoord;
out vec4 color;

uniform sampler2D material;
uniform sampler2D material_old;
uniform float fAge;
uniform bool bTexture;

void main()
{
  vec4 materialColor = texture(material, vertexTexCoord);
  vec4 materialColor_old = texture(material_old, vertexTexCoord);
  float alpha = materialColor_old.a * fAge;
  color = (1.0f - alpha) * materialColor + alpha * materialColor_old;
  color = mix(color, vec4(1), float(bTexture));
}
