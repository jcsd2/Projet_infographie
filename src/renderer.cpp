#include "renderer.h"

void Renderer::setup()
{
  ofSetFrameRate(60);
  clear_mode = ClearMode::none;
  // couleur de la zone de remplissage
  ofSetBackgroundColor(191);
  ofBackground(clear_color_r, clear_color_g, clear_color_b);

  //Initisialisation des variable (Comme dans les exemples du cours)
  // nombre maximal de primitives vectorielles dans le tableau
  buffer_count = 100;
  // position de la prochaine primitive
  buffer_head = 0;
  // calculer la taille de la structure générique commune à toutes les primitives vectorielles
  buffer_stride = sizeof(VectorPrimitive);
  // calculer le nombre d'octets à allouer en mémoire pour contenir le tableau de primitives vectorielles
  buffer_size = buffer_count * buffer_stride;
  // allocation d'un espace mémoire suffisamment grand pour contenir les données de l'ensemble des primitives vectorielles
  shapes = (VectorPrimitive*) std::malloc(buffer_size);
  // mode de dessin par défaut
  draw_mode = VectorPrimitiveType::none;
  
  //Inisialistion general

  // couleur de la ligne de contour (color_stroke)

  // couleur de la zone de remplissage (color_fill())

  //largeur de la ligne de contour
  stroke_width_default = 2;
  radius = 4.0f;

  mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
  is_mouse_button_pressed = false;


}

void Renderer::draw()
{
  for (index = 0; index < buffer_count; ++index)
  {
    switch (shapes[index].type)
    {
      case VectorPrimitiveType::none:
        break;

      case VectorPrimitiveType::pixel:

        ofFill();
        ofSetLineWidth(0);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_pixel(
          shapes[index].position2[0],
          shapes[index].position2[1]);
        break;

      case VectorPrimitiveType::point:

        ofFill();
        ofSetLineWidth(0);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_point(
          shapes[index].position2[0],
          shapes[index].position2[1],
          shapes[index].stroke_width);
        break;

      case VectorPrimitiveType::line:

        ofNoFill();
        ofSetLineWidth(shapes[index].stroke_width);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_line(
          shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
      break;

      case VectorPrimitiveType::square:

        ofFill();
        ofSetLineWidth(0);
        ofSetColor(
          shapes[index].fill_color[0],
          shapes[index].fill_color[1],
          shapes[index].fill_color[2]);
        draw_rectangle(
          shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
        ofNoFill();
        ofSetLineWidth(shapes[index].stroke_width);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_square(
          shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
      break;
        
      case VectorPrimitiveType::rectangle:

        ofFill();
        ofSetLineWidth(0);
        ofSetColor(
          shapes[index].fill_color[0],
          shapes[index].fill_color[1],
          shapes[index].fill_color[2]);
        draw_rectangle(
          shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
        ofNoFill();
        ofSetLineWidth(shapes[index].stroke_width);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_rectangle(
          shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
      break;

      case VectorPrimitiveType::circle:
        
        //Remplissage
        ofFill();
        ofSetLineWidth(0);
        ofSetCircleResolution(48);
        ofSetColor(
          shapes[index].fill_color[0],
          shapes[index].fill_color[1],
          shapes[index].fill_color[2]);
        draw_circle(shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);

        //Contour
        ofNoFill();
        ofSetLineWidth(shapes[index].stroke_width);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_circle(shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
        break;


      case VectorPrimitiveType::ellipse:

        ofFill();
        ofSetLineWidth(0);
        ofSetCircleResolution(48);
        ofSetColor(
          shapes[index].fill_color[0],
          shapes[index].fill_color[1],
          shapes[index].fill_color[2]);
        draw_ellipse(
          shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
        ofNoFill();
        ofSetLineWidth(shapes[index].stroke_width);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_ellipse(
          shapes[index].position1[0],
          shapes[index].position1[1],
          shapes[index].position2[0],
          shapes[index].position2[1]);
      break;

      case VectorPrimitiveType::triangle:
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(
          shapes[index].fill_color[0],
          shapes[index].fill_color[1],
          shapes[index].fill_color[2]);
        draw_triangle(
          shapes[index].position1[0], 
          shapes[index].position1[1],
          shapes[index].position2[0], 
          shapes[index].position2[1]);

        ofNoFill();
        ofSetLineWidth(shapes[index].stroke_width);
        ofSetColor(
          shapes[index].stroke_color[0],
          shapes[index].stroke_color[1],
          shapes[index].stroke_color[2]);
        draw_triangle(
          shapes[index].position1[0], shapes[index].position1[1],
          shapes[index].position2[0], shapes[index].position2[1]);

      break;

      default:
        break;
    }
  }

  // afficher la zone de sélection
  if (is_mouse_button_pressed)
  {
    draw_zone(
      mouse_press_x,
      mouse_press_y,
      mouse_current_x,
      mouse_current_y);
  }

}

// fonction qui vide le tableau de primitives vectorielles (Comme dans les exemples du cours)
void Renderer::reset()
{
  for (index = 0; index < buffer_count; ++index)
    shapes[index].type = VectorPrimitiveType::none;

  buffer_head = 0;

  ofLog() << "<reset>";
}

// fonction qui ajoute une primitive vectorielle au tableau (Comme dans les exemples du cours)
void Renderer::add_vector_shape(VectorPrimitiveType type)
{
  shapes[buffer_head].type = type;

  shapes[buffer_head].position1[0] = mouse_press_x;
  shapes[buffer_head].position1[1] = mouse_press_y;

  shapes[buffer_head].position2[0] = mouse_current_x;
  shapes[buffer_head].position2[1] = mouse_current_y;

  shapes[buffer_head].stroke_color[0] = stroke_color_r;
  shapes[buffer_head].stroke_color[1] = stroke_color_g;
  shapes[buffer_head].stroke_color[2] = stroke_color_b;
  shapes[buffer_head].stroke_color[3] = stroke_color_a;

  shapes[buffer_head].fill_color[0] = fill_color_r;
  shapes[buffer_head].fill_color[1] = fill_color_g;
  shapes[buffer_head].fill_color[2] = fill_color_b;
  shapes[buffer_head].fill_color[3] = fill_color_a;

  shapes[buffer_head].stroke_width = stroke_width_default;
  
  ofLog() << "<new primitive at index: " << buffer_head << ">";

  buffer_head = ++buffer_head >= buffer_count ? 0: buffer_head; // boucler sur le tableau si plein
}

// fonction qui dessine un pixel (Comme dans les exemples du cours)
void Renderer::draw_pixel(float x, float y) const
{
  //Floorf : fonction qui arrondit un nombre flottant vers le bas pour obtenir le nombre entier immédiatement inférieur
  int pixel_x = floorf(x);
  int pixel_y = floorf(y);

  ofDrawRectangle(pixel_x, pixel_y, 1, 1);
}

// fonction qui dessine un point
void Renderer::draw_point(float x, float y, float radius) const
{
  ofDrawEllipse(x, y, radius, radius);
}

// fonction qui dessine une ligne
void Renderer::draw_line(float x1, float y1, float x2, float y2) const
{
  //Ajouter choix algo

  ofDrawLine(x1, y1, x2, y2);
}

void Renderer::draw_square(float x1, float y1, float x2, float y2) const
{
  float squareX = min(x1, x2);
  float squareY = min(y1, y2);
  float squareWidth = abs(x2 - x1);
  float squareHeight = abs(y2 - y1);

  ofDrawRectangle(squareX, squareY, squareWidth, squareHeight);
}

// fonction qui dessine un rectangle (Comme dans les exemples du cours)
void Renderer::draw_rectangle(float x1, float y1, float x2, float y2) const
{
  ofDrawRectangle(x1, y1, x2 - x1, y2 - y1);
}

//Fonction qui dessine un cercle
void Renderer::draw_circle(float x1, float y1, float x2, float y2) const
{
  // Calculer le centre et le rayon du cercle
  float centerX = (x1 + x2) / 2;
  float centerY = (y1 + y2) / 2;
  float radiusX = (x2 - x1) / 2;
  float radiusY = (y2 - y1) / 2;

    // Utiliser le centre et le rayon pour dessiner le cercle
    ofDrawEllipse(centerX, centerY, radiusX * 2, radiusY * 2);
}

// fonction qui dessine une ellipse (Comme dans les exemples du cours)
void Renderer::draw_ellipse(float x1, float y1, float x2, float y2) const
{
  float diameter_x = x2 - x1;
  float diameter_y = y2 - y1;

  ofDrawEllipse(x1 + diameter_x / 2.0f, y1 + diameter_y / 2.0f, diameter_x, diameter_y);
}

//Fonction qui dessine un triangle
void Renderer::draw_triangle(float x1, float y1, float x2, float y2) const
{
    // Calculer les points du triangle
    float baseLength = x2 - x1;
    float xCenter = (x1 + x2) / 2;

    float x1_triangle = xCenter - baseLength / 2;
    float y1_triangle = y2;
    float x2_triangle = xCenter + baseLength / 2;
    float y2_triangle = y2;
    float x3_triangle = xCenter;
    float y3_triangle = y1;

    // Dessiner le triangle
    ofDrawTriangle(x1_triangle, y1_triangle, x2_triangle, y2_triangle, x3_triangle, y3_triangle);
}

void Renderer::update()
{
  

}

//Fonction pour dessiner la zone de selection (Commed ans les exemples du cours)
void Renderer::draw_zone(float x1, float y1, float x2, float y2) const 
{
  float x2_clamp = min(max(0.0f, x2), (float) ofGetWidth());
  float y2_clamp = min(max(0.0f, y2), (float) ofGetHeight());

  ofSetLineWidth(radius);
  ofSetColor(255, 0, 0, 100);
  ofNoFill();
  ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
  ofFill();
  //Ellipse pour arrondir les coins de la selection
  ofDrawEllipse(x1, y1, radius, radius);
  ofDrawEllipse(x1, y2_clamp, radius, radius);
  ofDrawEllipse(x2_clamp, y1, radius, radius);
  ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
}
Renderer::~Renderer()
{
  std::free(shapes);
}