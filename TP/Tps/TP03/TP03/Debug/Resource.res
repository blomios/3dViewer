        ��  ��                  3  @   S H A D E R S   V E R T E X . V E R T       0         #version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
out vec2 vertexTexCoord;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
    vertexTexCoord = texCoord;
} �  D   S H A D E R S   F R A G M E N T . F R A G       0         #version 330 core

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
