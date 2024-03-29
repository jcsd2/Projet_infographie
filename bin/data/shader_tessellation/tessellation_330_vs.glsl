//shader de tessellation Ce shader gère les transformations géométriques des sommets du modèle.

#version 330


// Attributs de sommet
in vec3 inVertexPosition; // Utilisé pour la tessellation

// Attributs en sortie
out vec3 surface_position;
out vec3 surface_normal;

// Attributs uniformes
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main() {
    // Transformation de la position du sommet dans l'espace de vue
    surface_position = vec3(modelViewMatrix * vec4(inVertexPosition, 1.0));

    // Normalisation de la normale (pas de transformation nécessaire)
    surface_normal = normalize(vec3(modelViewMatrix * vec4(gl_Normal, 0.0)));

    // Transformation de la position du sommet par les matrices de modèle, vue et projection
    gl_Position = projectionMatrix * modelViewMatrix * vec4(inVertexPosition, 1.0);
}
