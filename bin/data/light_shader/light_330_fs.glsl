#version 330



// attributs interpolés à partir des valeurs en sortie du shader de sommet
in vec3 surface_position;
in vec3 surface_normal;


// attribut en sortie
out vec4 fragment_color;

// Structure de lumiere

struct VectorLumiere {
    int type;
    vec3 color;
    vec3 position;
    vec3 target;
    float attenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

#define MAX_LIGHTS 4
uniform VectorLumiere lumiere[MAX_LIGHTS];

//attribut pour le type de lumiere 1 a 4
uniform int light_type;

//attributs couleur constante
uniform vec3 color_ambiant_light;

//attribut position
uniform vec3 light_position;
//attribut vecteur de direction

//attribut constante de attenuation
uniform float attenuation;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

//calcul vecteur de direction
vec3 direction(vec3 light_position, vec3 surface_position)
{
    return normalize(light_position - surface_position);
}

//Calcul pour la couleur directionnelle
vec3 lumiere_directionnelle(vec3 surface_normal, vec3 lightDirection, vec3 lightColor) 
{
    float diffuseFactor = max(dot(surface_normal, lightDirection), 0.0);
    return lightColor * diffuseFactor;
}

// Fonction pour calculer la couleur de la lumière ponctuelle
vec3 lumiere_ponctuelle(vec3 normalSurface, vec3 lightPosition, vec3 surfacePosition, vec3 lightColor, float constantAttenuation, float linearAttenuation, float quadraticAttenuation) {
    vec3 lightDirection = direction(lightPosition, surfacePosition);
    float diffuseFactor = max(dot(normalSurface, lightDirection), 0.0);
    float distance = length(lightPosition - surfacePosition);
    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);
    return lightColor * diffuseFactor * attenuation;
}

// Fonction pour calculer la couleur de la lumière de type projecteur
vec3 lumiere_projecteur(vec3 normalSurface, vec3 lightPosition, vec3 surfacePosition, vec3 lightColor, vec3 target, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, float cutoff, float power) {
    vec3 lightToSurfaceDirection = normalize(surfacePosition - lightPosition);
    vec3 lightDirection = direction(lightPosition, surfacePosition);
    vec3 directionToTarget = normalize(target - lightPosition); // Calcul de la direction vers la cible
    float diffuseFactor = max(dot(normalSurface, directionToTarget), 0.0); // Utilisation de la direction vers la cible pour le facteur de diffusion
    float distance = length(lightPosition - surfacePosition);
    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);
    float spotFactor = pow(max(dot(lightDirection, -lightToSurfaceDirection), 0.0), power);
    float spotCutoff = dot(lightToSurfaceDirection, lightDirection) > cos(cutoff) ? 1.0 : 0.0;
    return lightColor * diffuseFactor * attenuation * spotFactor * spotCutoff;
}


void main()
{
  // re-normaliser la normale après interpolation
    vec3 n = normalize(surface_normal);

    // initialiser la couleur du fragment à noir
    vec3 color = vec3(0.0);

    // boucler sur toutes les lumières
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        // calculer la couleur en fonction du type de lumière
        if (lumiere[i].type == 0) {
            // lumière ambiante
            color += lumiere[i].color;
        } else if (lumiere[i].type == 1) {
            // lumière directionnelle
            vec3 lightDirection = direction(lumiere[i].position, surface_position);
            color += lumiere_directionnelle(n, lightDirection, lumiere[i].color);
        } else if (lumiere[i].type == 2) {
            // lumière ponctuelle
            color += lumiere_ponctuelle(n, lumiere[i].position, surface_position, lumiere[i].color, lumiere[i].attenuation, lumiere[i].linearAttenuation, lumiere[i].quadraticAttenuation);
        } else if (lumiere[i].type == 3) {
            // lumière de type projecteur
            vec3 lightDirection = direction(lumiere[i].position, surface_position);
            color += lumiere_projecteur(n, lumiere[i].position, surface_position, lumiere[i].color, lumiere[i].target, lumiere[i].attenuation, lumiere[i].linearAttenuation, lumiere[i].quadraticAttenuation, 0.5, 1.0); // vous devrez ajuster les valeurs de cutoff(angle de coupure) et power en fonction de vos besoins
        }
    }

    // définir la couleur du fragment
    fragment_color = vec4(color, 1.0);

}
