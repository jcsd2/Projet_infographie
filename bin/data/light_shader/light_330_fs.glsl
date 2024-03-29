#version 330

// attributs interpolés à partir des valeurs en sortie du shader de sommet
in vec3 surface_position;
in vec3 surface_normal;

// attribut en sortie
out vec4 fragment_color;

// Structure de lumiere (remplacée par uniform)
struct Light {
    int type;
    vec3 color;
    vec3 position;
    vec3 target;
    float attenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float spotExponent;        // Ajout du membre spotExponent
    float spotCutoffAngle;     // Ajout du membre spotCutoffAngle
};

#define MAX_LIGHTS 4
uniform Light lumiere[MAX_LIGHTS]; // Uniform au lieu de struct
uniform vec3 material_ambient;

void main()
{
    // Re-normaliser la normale après interpolation
    vec3 n = normalize(surface_normal);

    // Initialiser la couleur du fragment à noir
    vec3 color = vec3(0.0);

    // Boucler sur toutes les lumières
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        // Calculer la couleur en fonction du type de lumière
        if (lumiere[i].type == 0) {
            // Lumière ambiante
            color += lumiere[i].color * material_ambient;
        } else if (lumiere[i].type == 1) {
            // Lumière directionnelle
            vec3 lightDirection = normalize(lumiere[i].position - surface_position);
            float diffuseFactor = max(dot(n, lightDirection), 0.0);
            color += lumiere[i].color * diffuseFactor;
        } else if (lumiere[i].type == 2) {
            // Lumière ponctuelle
            vec3 lightDirection = normalize(lumiere[i].position - surface_position);
            float distance = length(lumiere[i].position - surface_position);
            float attenuation = 1.0 / (lumiere[i].attenuation + lumiere[i].linearAttenuation * distance + lumiere[i].quadraticAttenuation * distance * distance);
            float diffuseFactor = max(dot(n, lightDirection), 0.0);
            color += lumiere[i].color * diffuseFactor * attenuation;
        } else if (lumiere[i].type == 3) {
            // Lumière de type projecteur
            vec3 lightDirection = normalize(lumiere[i].position - surface_position);
            vec3 directionToTarget = normalize(lumiere[i].target - lumiere[i].position); 
            float diffuseFactor = max(dot(n, directionToTarget), 0.0); 
            float distance = length(lumiere[i].position - surface_position);
            float attenuation = 1.0 / (lumiere[i].attenuation + lumiere[i].linearAttenuation * distance + lumiere[i].quadraticAttenuation * distance * distance);
            float spotFactor = pow(max(dot(lightDirection, -directionToTarget), 0.0), lumiere[i].spotExponent);
            float spotCutoff = dot(lightDirection, directionToTarget) > cos(lumiere[i].spotCutoffAngle) ? 1.0 : 0.0;
            color += lumiere[i].color * diffuseFactor * attenuation * spotFactor * spotCutoff;
        }
    }

    // Définir la couleur du fragment
    fragment_color = vec4(color, 1.0);
}