#version 330

in vec2 surface_texture_coord;

out vec4 fragemnt_color;

uniform sampler2D image;
uniform float tone_mapping_exposure;
uniform float tone_mapping_gamma;
uniform bool tone_mapping_toggle;

//Fonction de mappage reinhard (plus x eleve(pixel lumineux), plus la valeur finale est redutie)
vec3 tone_mapping_reinhard(vec3 x)
{
	return x / (x + vec3(1.0));
}
//Fonction de mappage avec algorithme de ACES (a,b,c,d... sont des approximations de la courbe de ACES)
vec3 tone_mapping_aces(vec3 x)
{
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}
void main()
{
	vec3 color;
	vec3 texture_sample = texture(image, surface_texture_coord).rgb;
	color = texture_sample;
	//Espace gamma vers lineaire (eleve rgb a la puissance de tone_mapping)
	color = pow(color, vec3(tone_mapping_gamma));
	//Exponentielle de loppose de couleur et inversement de celle-ci
	color = vec3(1.0) - exp(-color * tone_mapping_exposure);
	if(tone_mapping_toggle)
	{
		color = tone_mapping_reinhard(color);
	} else {
		color = tone_mapping_aces(color);
	}
	//lineaire vers gamme
	color = pow(color, vec3(1.0 / tone_mapping_gamma));
	fragemnt_color = vec4(color, 1.0);
}
