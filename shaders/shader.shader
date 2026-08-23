{
    "UUID": 15482739461592837462,
    "vertexShaderCode": "#version 450\nvec2 positions[3]={vec2(0.0,-0.5),vec2(0.5,0.5),vec2(-0.5,0.5)};void main(){gl_Position=vec4(positions[gl_VertexIndex],0.0,1.0);}",
    "fragmentShaderCode": "#version 450\nlayout(location=0) out vec4 outColor;void main(){outColor=vec4(1.0,0.0,0.0,1.0);}"
}