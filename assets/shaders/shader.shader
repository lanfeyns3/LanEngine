{
  "UUID": 15482739461592837462,
  "vertexShaderCode": "#version 330 core\nlayout(location=0) in vec3 position;\nlayout(location=1) in vec3 normal;\n\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 projection;\n\nout vec3 fragNormal;\n\nvoid main()\n{\n    fragNormal = mat3(transpose(inverse(model))) * normal;\n\n    gl_Position = projection * view * model * vec4(position, 1.0);\n}",
  "fragmentShaderCode": "#version 330 core\n\nlayout(location=0) out vec4 outColor;\n\nin vec3 fragNormal;\n\nvoid main()\n{\n    outColor = vec4(1.0, 0.0, 0.0, 1.0);\n}"
}
