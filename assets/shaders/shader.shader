{
  "UUID": 15482739461592837462,
  "vertexShaderCode": "#version 330 core\nlayout(location=0) in vec2 position;\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 projection;\nvoid main(){gl_Position=projection*view*model*vec4(position,0.0,1.0);}",
  "fragmentShaderCode": "#version 330 core\nlayout(location=0) out vec4 outColor;void main(){outColor=vec4(1.0,0.0,0.0,1.0);}"
}