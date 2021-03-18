#shader vertex
#version 100
attribute vec4 position;
void main()
{
   gl_Position = position;
}

#shader fragment
#version 100

uniform vec4 u_Color;

void main()
{
   gl_FragColor = u_Color;
}