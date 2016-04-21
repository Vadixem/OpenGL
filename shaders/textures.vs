#version 330 core
layout (location = 0) in vec3 position;
  
uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{   
    // We read multiplication from right to left.
    gl_Position = projection * view * model * vec4(position, 1.0f);
} 