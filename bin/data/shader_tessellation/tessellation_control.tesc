//Ce shader contrôle la tessellation en définissant les niveaux de tessellation et en passant des données aux shaders d'évaluation de tessellation.

#version 330 core

layout(vertices = 3) out;

// Attributs en entrée
in vec3 surface_position[];
in vec3 surface_normal[];

// Attributs en sortie
out vec3 tcs_surface_position[];
out vec3 tcs_surface_normal[];

void main() {
    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = 5.0; // Inner tessellation level
        gl_TessLevelOuter[0] = 5.0; // Outer tessellation level
        gl_TessLevelOuter[1] = 5.0;
        gl_TessLevelOuter[2] = 5.0;
    }

    // Passer les données nécessaires au shader d'évaluation de tessellation
    for (int i = 0; i < 3; ++i) {
        tcs_surface_position[i] = surface_position[i];
        tcs_surface_normal[i] = surface_normal[i];
    }

    // Passer les positions des sommets
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
