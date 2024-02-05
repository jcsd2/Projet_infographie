#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "renderer.h"
class Application : public ofBaseApp
{
public:

  Renderer renderer;

  ofxPanel gui;

  ofxGuiGroup group_image;

  ofxFloatSlider slider_position_x;
  ofxFloatSlider slider_position_y;
  ofParameter<bool> checkbox;

  ofxButton screenshot_button;
  void setup();
  void update();
  void draw();
  void exit();
  void keyReleased(int key);
  void screenshot_button_pressed();

};