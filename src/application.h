#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofSystemUtils.h"
#include "renderer.h"
class Application : public ofBaseApp
{
public:

  Renderer renderer;

  ofxPanel gui;
  ofxGuiGroup group_image;
  ofxGuiGroup group_dessin_vectoriel;

  ofParameter<bool> checkbox; //Parametre pour voir le Gui
  ofEvent<bool> screenshotButtonEvent;  // Declare an ofEvent
  
  //Cursor selection points/ Position du curseur pour la selection
  ofPoint selection_start;
  ofPoint selection_end;

  ofxToggle screenshot_button;


  void setup();
  void update();
  void draw();
  void exit();
  void keyReleased(int key);
  void screenshot_button_pressed(bool& value);


  // Nouvelles fonctions pour la gestion de la sélection de zone (utile pour screenshot)
  void mousePressed(int x, int y, int button) override;
  void mouseDragged(int x, int y, int button) override;
  void mouseReleased(int x, int y, int button) override;

  private:
    bool is_selecting;  // Indicateur de sélection en cours
    bool captureMode; //Indicateur de mode CaptureDecran

};