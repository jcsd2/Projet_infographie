#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofSystemUtils.h"
#include "renderer.h"
#include "ofxAssimpModelLoader.h"
#include <iostream>



class Application : public ofBaseApp
{
public:


    enum CursorState {
        CURSOR_DEFAULT,
        CURSOR_DRAW_LINE,
        CURSOR_DRAW_CIRCLE,
        CURSOR_SELECT,
        CURSOR_TRANSLATE,
        CURSOR_ROTATE
    };

    CursorState currentCursorState = CURSOR_DEFAULT;

    Renderer renderer;
  
    
    ofxPanel gui;
    ofParameter<bool> checkbox; //Parametre pour voir le Gui

    
    ofxGuiGroup group_scene_control;
    ofxButton addElementButton;
    ofxButton removeElementButton;
    ofxButton selectElementButton;

   
    ofxToggle drawBoundingBoxButton;
    std::vector<VectorPrimitive> elements;
    int selectedElementIndex = -1;
    void addElementPressed();
    void removeElementPressed();
    void selectElementPressed();
    int selectedShapeIndex = -1;
    int selectedModelIndex = -1;
   
    //1.3
    ofxGuiGroup group_image;
    ofxToggle screenshot_button;
    ofxToggle screenshot_button_funny;
    //1.4
    ofxButton background_rgb_button;
    ofxButton background_hsb_button;
    ofParameter<ofColor> color_picker_background; //changer couleur fond
    ofParameter<ofColor> color_picker_background_HSB; //changer  HSB
    void background_rgb_button_pressed();
    void background_hsb_button_pressed();

    //1.5
    ofxButton histogramme_button;
    void histogramme_button_pressed();

    ofxGuiGroup group_dessin_vectoriel;
    ofxGuiGroup group_dessin_vectoriel_formes;
    ofxGuiGroup group_dessin_algo_ligne;

    ofxButton cursorDefaultButton;
    ofxButton cursorDrawLineButton;
    ofxButton cursorDrawCircleButton;
    ofxButton cursorSelectButton;
    ofxButton cursorTranslateButton;
    ofxButton cursorRotateButton;
     
    

    //ofImage importedImage; // stocker l'image charge
    ofxButton importImageButton; //bouton pour importer l'images
    ofxButton exportation_button; // Parametre Exportation
    void exportation_button_pressed();
    bool isExporting;
    float lastExportTime;
    int exportCount;

    ofxGuiGroup group_outils_dessin; // Parametre Outil de dessin
    ofxFloatSlider lineThickness;
    ofxColorSlider lineColor, fillColor;

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
    ofxToggle maison_shape_button;
    ofxToggle face_shape_button;

    void ajout_boutons_formes();
    void retirer_boutons_formes();
    void button_none_pressed(bool& pressed);
    void button_pixel_pressed(bool& pressed);
    void button_point_pressed(bool& pressed);
    void button_line_pressed(bool& pressed);
    void button_algo_of_pressed(bool& pressed);
    void button_algo_dda_pressed(bool& pressed);
    void button_algo_bressenham_pressed(bool& pressed);
    void button_square_pressed(bool& pressed);
    void button_rectangle_pressed(bool& pressed);
    void button_circle_pressed(bool& pressed);
    void button_ellipse_pressed(bool& pressed);
    void button_triangle_pressed(bool& pressed);
    void button_maison_pressed(bool& pressed);
    void button_face_pressed(bool& pressed);
    void importImage();

    //Section transformation 3.0
    ofxGuiGroup group_transformation;
    ofxGuiGroup groupe_transforamtion_interactive;

    // Selection multiple 3.2 ... déclaration
    ofxGuiGroup groupe_selection_multiple;
    ofxToggle selectionButton;
    void selection_multiple(bool& pressed);
    bool isSelectionModeActive;
    bool isInside(int x, int y, const VectorPrimitive& shape);
    bool shapeSelected;

    // Boutons pour les options de modification
    ofxButton noneTransformationButton;
    ofxButton translateButton;
    ofxButton rotateButton;
    ofxButton scaleButton;

    //Section 4 Geometrie
    ofxGuiGroup groupe_geometrie;
    ofxButton none_model_button;
    ofxButton import_model_button;
    ofxButton predef1_model_button;
    ofxButton predef2_model_button;
    ofxButton predef3_model_button;
    ofxButton predef4_model_button;
    ofxButton remove_last_model_button;
    bool bShowModel;
    void non_model_button_pressed();
    void import_model_button_pressed();
    void predef1_model_button_pressed();
    void predef2_model_button_pressed();
    void predef3_model_button_pressed();
    void predef4_model_button_pressed();
    void remove_last_model_button_pressed();
 

   // Primitives géométriques 4.2    


    ofxGuiGroup groupe_primitive_geometrie;
    ofxToggle cubeButton;
    ofxToggle sphereButton;


    void cubeButtonPressed(bool& pressed);
    void sphereButtonPressed(bool& pressed);


    // Section 5 Caméra
    ofxGuiGroup groupe_camera;


    // Modes de projection 5.2

    ofVec3f camera_position;
    ofVec3f camera_target;

    ofxGuiGroup mode_projection;
    ofxButton perspectiveButton;
    ofxButton orthogonaleButton;

    void perspectiveButtonPressed();
    void orthogonaleButtonPressed();

    //Modes pov multiple
    ofxButton mode_dessincam_button;
    ofxButton mode_1cam_button;
    ofxButton mode_2cam_button;
    void mode_dessin_pressed();
    void mode_1cam_pressed();
    void mode_2cam_pressed();

    //Occlusion 
    ofxButton meshfilled_button;
    ofxButton wireframe_button;
    void meshfilled_button_pressed();
    void wireframe_button_pressed();

    // Méthodes associées aux boutons
    void noneTransformationButtonPressed();
    void translateButtonPressed();
    void rotateButtonPressed();
    void scaleButtonPressed();
    void drawCursor();

    void cursorDefaultButtonPressed();
    void cursorDrawLineButtonPressed();
    void cursorDrawCircleButtonPressed();
    void cursorSelectButtonPressed();
    void cursorTranslateButtonPressed();
    void cursorRotateButtonPressed();


    bool isTranslationActive;
    bool isRotatingActive;
    bool isScalingActive;

    //Cursor selection points/ Position du curseur pour la selections
    ofPoint selection_start;
    ofPoint selection_end;

    //Temps
    float time_current;
    float time_last;
    float time_elapsed;

    //Touche felche clavier
    bool is_key_press_up;
    bool is_key_press_down;
    bool is_key_press_left;
    bool is_key_press_right;

    void setup();
    void update();
    void draw();

    


    void exit();

    void screenshot_button_pressed(bool& value);
    void screenshot_funny_button_pressed(bool& value);
    void screenshot(int x, int y, bool z);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button) override;
    void mouseDragged(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;

private:
    bool captureMode; //Indicateur de mode CaptureDecran
    bool captureMode_funny;
    //ofxUndoSimple<VectorPrimitive> position_;
};
