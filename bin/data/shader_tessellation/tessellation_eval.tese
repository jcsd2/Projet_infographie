#version 330 core

layout(triangles, equal_spacing, ccw) in;

// Attributs en entrée
in vec3 tcs_surface_position[];
in vec3 tcs_surface_normal[];

// Attributs en sortie
out vec3 surface_position;
out vec3 surface_normal;

void main() {
    vec3 p0 = tcs_surface_position[0];
    vec3 p1 = tcs_surface_position[1];
    vec3 p2 = tcs_surface_position[2];

    // Interpolation des positions des sommets
    surface_position = gl_TessCoord.x * p0 + gl_TessCoord.y * p1 + gl_TessCoord.z * p2;

    // Interpolation des normales des sommets
    vec3 n0 = tcs_surface_normal[0];
    vec3 n1 = tcs_surface_normal[1];
    vec3 n2 = tcs_surface_normal[2];
    surface_normal = normalize(gl_TessCoord.x * n0 + gl_TessCoord.y * n1 + gl_TessCoord.z * n2);

    gl_Position = vec4(surface_position, 1.0);
}
