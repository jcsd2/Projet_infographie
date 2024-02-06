#include "renderer.h"

void Renderer::setup()
{
  ofSetFrameRate(60);
  clear_mode = ClearMode::none;
  // couleur de la zone de remplissage
  ofSetBackgroundColor(clear_color_r, clear_color_g, clear_color_b);
  ofBackground(clear_color_r, clear_color_g, clear_color_b);
  // changement d'état
  has_changed = true;
  mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
  is_mouse_button_pressed = false;


}
void Renderer::draw()
{
  // 3. appliquer la couleur d'arrière-plan selon le mode courant
  switch (clear_mode)
  {
    case ClearMode::none:
      clear();
      break;

    case ClearMode::gray:
      clear_to_gray(223);
      break;

    default:
      ofBackground(255, 0, 0);
      break;
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

// fonction qui efface le contenu du framebuffer actif et le remplace par une couleur par défaut
void Renderer::clear() const
{
  ofBackground(clear_color_r, clear_color_g, clear_color_b);
}

void Renderer::clear_to_gray(int g)
{
    if (has_changed)
  {
    has_changed = false;
    clear_color_gray = g;
  }

  ofBackground(clear_color_gray);
}


void Renderer::update()
{
  

}

void Renderer::draw_zone(float x1, float y1, float x2, float y2) const
{
  float x2_clamp = min(max(0.0f, x2), (float) ofGetWidth());
  float y2_clamp = min(max(0.0f, y2), (float) ofGetHeight());

  
  ofSetColor(255, 0, 0, 100);
  ofNoFill();
  ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
  ofFill();
}
Renderer::~Renderer()
{

}