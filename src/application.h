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
  ofParameter<bool> checkbox; //Parametre pour voir le Gui
  
  ofxGuiGroup group_image;
  ofxToggle screenshot_button;

  ofxGuiGroup group_dessin_vectoriel;
  ofxGuiGroup group_dessin_vectoriel_formes;
  ofxGuiGroup group_dessin_algo_ligne;

  
  void ajout_boutons_formes();
  void retirer_boutons_formes();
  void button_none_pressed(bool &pressed);
  void button_pixel_pressed(bool &pressed);
  void button_point_pressed(bool &pressed);
  void button_line_pressed(bool &pressed);
  void button_algo_of_pressed(bool &pressed);
  void button_algo_dda_pressed(bool &pressed);
  void button_algo_bressenham_pressed(bool &pressed);
  void button_square_pressed(bool &pressed);
  void button_rectangle_pressed(bool &pressed);
  void button_circle_pressed(bool &pressed);
  void button_ellipse_pressed(bool &pressed);
  void button_triangle_pressed(bool &pressed);

  ofxToggle none_shape_button;
  ofxToggle pixel_shape_button;
  ofxToggle point_shape_button;
  ofxToggle line_shape_button;

  ofxToggle algo_of_button;
  ofxToggle algo_dda_button;
  ofxToggle algo_bressenham_button;

  ofxToggle square_shape_button;
  ofxToggle rectangle_shape_button;
  ofxToggle circle_shape_button;
  ofxToggle ellipse_shape_button;
  ofxToggle triangle_shape_button;


  //Cursor selection points/ Position du curseur pour la selection
  ofPoint selection_start;
  ofPoint selection_end;

  void setup();
  void update();
  void draw();
  void exit();
  
  void screenshot_button_pressed(bool &value);
  void screenshot(int x,int y);
  
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mousePressed(int x, int y, int button) override;
  void mouseDragged(int x, int y, int button) override;
  void mouseReleased(int x, int y, int button) override;

  private:
    bool captureMode; //Indicateur de mode CaptureDecran


};