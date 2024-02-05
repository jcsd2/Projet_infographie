#include "application.h"


void Application::setup()
{
  ofSetWindowTitle("Projet_Session");

  ofLog() << "<app::setup>";

  renderer.setup();
  gui.setup("Interface");
  checkbox.setName("Gui visible");
  checkbox = true;
  gui.add(checkbox);
  group_image.setup("Image");
  //Bouton pour echantillonnement dimage
  screenshot_button.setup("Capture d'ecran");
  screenshot_button.addListener(this, &Application::screenshot_button_pressed);
  group_image.add(&screenshot_button);
  slider_position_x.setup("Position X", 0, 0, ofGetWidth());
  slider_position_y.setup("Position Y", 0, 0, ofGetHeight());
  group_image.add(&slider_position_x);
  group_image.add(&slider_position_y);
  gui.add(&group_image);
}

void Application::draw()
{
  renderer.draw();
  if (checkbox)
    gui.draw();
}


void Application::update()
{
  renderer.update();
}

void Application::keyReleased(int key)
{
    switch (key)
  {
    case 117: //key u
      checkbox = !checkbox;
      ofLog() << "<toggle ui: " << checkbox << ">";
      break;
  
    default:
      break;
  }
}

void Application::screenshot_button_pressed()
{
  // Faire screenshot
  
  ofLog() << "<Screenshot>";
}

void Application::exit()
{
  screenshot_button.removeListener(this, &Application::screenshot_button_pressed);
  ofLog() << "<app::exit>";
}