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
    ofParameter<bool> checkbox; // Parametre GUI 

    // Importation d'images 1.1
    ofxButton importImageButton;

    // Exportation d'images 2.2
    ofxButton exportation_button;
    void exportation_button_pressed();
    bool isExporting;
    float lastExportTime;
    int exportCount;

    // Échantillonnage d'images 1.3
    ofxGuiGroup group_image;
    ofxToggle screenshot_button;
    ofxToggle screenshot_button_funny;

    // Espace de couleur RGB HSB 1.4
    ofxButton background_rgb_button;
    ofxButton background_hsb_button;
    ofParameter<ofColor> color_picker_background; 
    ofParameter<ofColor> color_picker_background_HSB;
    void background_rgb_button_pressed();
    void background_hsb_button_pressed();

    // Histogramme 1.5
    ofxButton histogramme_button;
    void histogramme_button_pressed();

    ofxGuiGroup group_dessin_vectoriel;
    ofxGuiGroup group_dessin_vectoriel_formes;
    ofxGuiGroup group_dessin_algo_ligne;

    // Curseur dynamique 2.1
    ofxButton cursorDefaultButton;
    ofxButton cursorDrawLineButton;
    ofxButton cursorDrawCircleButton;
    ofxButton cursorSelectButton;
    ofxButton cursorTranslateButton;
    ofxButton cursorRotateButton;

    // Outils de dessin 2.2
    ofxGuiGroup group_outils_dessin; 
    ofxFloatSlider lineThickness;
    ofxColorSlider lineColor, fillColor;

    // Primitives Vectorielles 2.3
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

    // Formes Vectorielles 2.4
    ofxToggle maison_shape_button;
    ofxToggle face_shape_button;

    // Ajout/suppression formes
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

    // Section Transformation 3.0
    ofxGuiGroup group_transformation;
    ofxGuiGroup groupe_transforamtion_interactive;

    // Graphe de scene 3.1
  
    std::vector<VectorPrimitive> elements;
    int selectedElementIndex = -1;
    void addElementPressed();
    void removeElementPressed();
    int selectedShapeIndex = -1;
    int selectedModelIndex = -1;
    ofxGuiGroup group_scene_control;
    ofxButton addElementButton;
    ofxButton removeElementButton;

    // Selection multiple 3.2
    ofxGuiGroup groupe_selection_multiple;
    ofxToggle selectionButton;
    void selection_multiple(bool& pressed);
    bool isSelectionModeActive;
    bool isInside(int x, int y, const VectorPrimitive& shape);
    bool shapeSelected;

    // Transformation interactives 3.3
    ofxButton noneTransformationButton;
    ofxButton translateButton;
    ofxButton rotateButton;
    ofxButton scaleButton;

    // Historique de transformation Undo/Redo 3.4
    ofxGuiGroup historique_group;
    ofxButton undo_button;
    ofxButton redo_button;
    void undo_button_pressed();
    void redo_button_pressed();

    // Section 4 Geometrie
    ofxGuiGroup groupe_geometrie;


    // Boîte de délimitation 4.1
    ofxToggle drawBoundingBoxButton;

    // Primitives geometriques 4.2
    ofxGuiGroup groupe_primitive_geometrie;
    ofxToggle cubeButton;
    ofxToggle sphereButton;
    void cubeButtonPressed(bool& pressed);
    void sphereButtonPressed(bool& pressed);

    // Modele 3D 4.3
    ofxButton none_model_button;
    ofxButton import_model_button;
    ofxButton predef1_model_button;
    ofxButton predef2_model_button;
    ofxButton predef3_model_button;
    ofxButton remove_last_model_button;
    bool bShowModel;
    void non_model_button_pressed();
    void import_model_button_pressed();
    void predef1_model_button_pressed();
    void predef2_model_button_pressed();
    void predef3_model_button_pressed();
    void remove_last_model_button_pressed();

    // Animation 4.4
    ofxGuiGroup geometrie_animation;
    ofxButton animation_button;
    bool animation_svg_object_active;
    void animation_button_pressed();

    // Instanciation 4.5
    ofxButton instanciation_button;
    void instanciation_button_pressed();

    // Section 5 Caméra
    ofxGuiGroup groupe_camera;

    // Modes de projection
    ofVec3f camera_position;
    ofVec3f camera_target;
    ofxGuiGroup mode_projection;

    // Camera interactive 5.1
    ofxButton camera_interactive;
    void toggleCameraInteractive();

    // Modes de projection 5.2
    ofxButton perspectiveButton;
    ofxButton orthogonaleButton;
    void perspectiveButtonPressed();
    void orthogonaleButtonPressed();

    // Modes POV multiple 5.3
    ofxButton mode_dessincam_button;
    ofxButton mode_1cam_button;
    ofxButton mode_2cam_button;
    void mode_dessin_pressed();
    void mode_1cam_pressed();
    void mode_2cam_pressed();

    // Occlusion 5.4
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

    // Position start/end du curseur
    ofPoint selection_start;
    ofPoint selection_end;

    // Temps
    float time_current;
    float time_last;
    float time_elapsed;

    // Touche fleche du clavier
    bool is_key_press_up;
    bool is_key_press_down;
    bool is_key_press_left;
    bool is_key_press_right;
    bool is_key_home;
    bool is_key_end;
    bool is_key_ins;
    bool is_key_del;
    bool is_key_pgup;
    bool is_key_pgdown;

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
    bool captureMode; 
    bool captureMode_funny;
    
};
