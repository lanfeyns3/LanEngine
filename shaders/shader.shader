{
  "UUID": 15482739461592837462,
  "vertexShaderCode": "#version 450\nlayout(location=0) in vec2 pos;\nvoid main(){gl_Position=vec4(pos,0.0,1.0);}",
  "fragmentShaderCode": "#version 450\nlayout(location=0) out vec4 outColor;void main(){outColor=vec4(1.0,0.0,0.0,1.0);}"
}