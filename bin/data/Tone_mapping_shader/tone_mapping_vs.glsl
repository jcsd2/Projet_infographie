#version 330

//Attribut uniforme pour la pos en 3D donc 4 para
uniform mat4 modelViewProjectionMatrix;

//Attribut de position 3D
in vec4 position;
//Attribut de pos de texture 2D
in vec2 texcoord;

//Sortie passer au shader de fragment
out vec2 surface_texture_coord;

void main()
{
	//Transform matrice de pos 3D par ModelViewProjection
	gl_Position = modelViewProjectionMatrix * position;
	surface_texture_coord = texcoord;
}
