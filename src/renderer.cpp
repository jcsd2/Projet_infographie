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

Renderer::~Renderer()
{
}