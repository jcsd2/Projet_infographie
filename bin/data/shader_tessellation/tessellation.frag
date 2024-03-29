#version 330 core

in vec3 surface_position;
in vec3 surface_normal;

out vec4 FragColor;

void main() {
    // Exemple : utiliser la normale comme couleur
    FragColor = vec4(surface_normal, 1.0);
}
