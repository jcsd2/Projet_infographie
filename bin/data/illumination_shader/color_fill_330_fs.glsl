#version 330

// couleur de remplissage du matériau
uniform vec3 color;

out vec4 fragment_color;

void main()
{
  // déterminer la couleur du fragment
  fragment_color = vec4(color, 1.0);
}
