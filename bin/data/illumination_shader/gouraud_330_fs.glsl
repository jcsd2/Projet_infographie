#version 330

in vec3 surface_color;

// attribut en sortie
out vec4 fragment_color;

void main()
{
  // calculer la couleur du fragment
  fragment_color = vec4(surface_color, 1.0);
}
