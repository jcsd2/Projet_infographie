#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofSystemUtils.h"
#include "renderer.h"
#include "ofxAssimpModelLoader.h"
#include <iostream>

enum CursorState {
        CURSOR_DEFAULT,
        CURSOR_DRAW_LINE,
        CURSOR_DRAW_CIRCLE,
        CURSOR_SELECT,
        CURSOR_TRANSLATE,
        CURSOR_ROTATE
    };

class Application : public ofBaseApp
{
public:

    Renderer renderer;
    

    

    //Section 1
    ofxGuiGroup group_image;

    // Importation d'images 1.1
    ofxButton importImageButton;
    void importImage();

    // Exportation d'images 2.2
    ofxButton exportation_button;
    void exportation_button_pressed();
    bool isExporting;
    int exportCount;
    float lastExportTime;

    // Échantillonnage d'images 1.3
    ofxToggle screenshot_button;
    ofxToggle screenshot_button_funny;
    void screenshot_button_pressed(bool& value);
    void screenshot_funny_button_pressed(bool& value);
    void screenshot(int x, int y, bool z);

    // Espace de couleur RGB HSB 1.4
    ofxButton background_rgb_button;
    ofxButton background_hsb_button;
    void background_rgb_button_pressed();
    void background_hsb_button_pressed();
    ofParameter<ofColor> color_picker_background; 
    ofParameter<ofColor> color_picker_background_HSB;

    // Histogramme 1.5
    ofxButton histogramme_button;
    void histogramme_button_pressed();

    //Section 2
    ofxGuiGroup group_dessin_vectoriel;
    
    // Curseur dynamique 2.1
    ofxGuiGroup group_curseurs;
    CursorState cursor;
    ofxButton cursorDefaultButton;
    ofxButton cursorDrawLineButton;
    ofxButton cursorDrawCircleButton;
    ofxButton cursorSelectButton;
    ofxButton cursorTranslateButton;
    ofxButton cursorRotateButton;
    ofPoint selection_start;// Position start/end du curseur
    ofPoint selection_end;
    void cursorDefaultButtonPressed();
    void cursorDrawLineButtonPressed();
    void cursorDrawCircleButtonPressed();
    void cursorSelectButtonPressed();
    void cursorTranslateButtonPressed();
    void cursorRotateButtonPressed();
    void drawCursor();

    // Outils de dessin 2.2
    ofxGuiGroup group_outils_dessin; 
    ofxFloatSlider lineThickness;
    ofxColorSlider lineColor, fillColor;

    // Primitives Vectorielles 2.3
    ofxGuiGroup group_dessin_vectoriel_formes;
    ofxGuiGroup group_dessin_algo_ligne;
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
    // Formes Vectorielles 2.4
    ofxToggle maison_shape_button;
    ofxToggle face_shape_button;
    void button_maison_pressed(bool& pressed);
    void button_face_pressed(bool& pressed);
    void ajout_boutons_formes(); //Ajoute boutons 2.3 et 2.4
    
    // Interface 2.5
    ofxPanel gui;
    ofParameter<bool> checkbox; // Parametre GUI


    // Section Transformation 3.0
    ofxGuiGroup group_transformation;

    // Graphe de scene 3.1
    ofxGuiGroup group_scene_control;
    ofxButton addElementButton;
    ofxButton removeElementButton;
    int selectedShapeIndex;
    int selectedModelIndex;
    void addElementPressed();
    void removeElementPressed();
    
    // Selection multiple 3.2
    ofxGuiGroup groupe_selection_multiple;
    ofxToggle selectionButton;
    void selection_multiple(bool& pressed);
    bool isSelectionModeActive;
    bool isInside(int x, int y, const VectorPrimitive& shape);
    bool shapeSelected;

    // Transformation interactives 3.3
    ofxGuiGroup groupe_transforamtion_interactive;
    ofxButton noneTransformationButton;
    ofxButton translateButton;
    ofxButton rotateButton;
    ofxButton scaleButton;
    void noneTransformationButtonPressed();
    void translateButtonPressed();
    void rotateButtonPressed();
    void scaleButtonPressed();
    bool isTranslationActive;
    bool isRotatingActive;
    bool isScalingActive;

    // Historique de transformation 3.4
    ofxGuiGroup historique_group;
    ofxButton undo_button;
    ofxButton redo_button;
    void undo_button_pressed();
    void redo_button_pressed();

    // Section 4 Geometrie
    ofxGuiGroup groupe_geometrie;

    // Boîte de délimitation 4.1

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

    // Camera interactive 5.1
    ofxButton camera_interactive_button;
    void toggleCameraInteractive();

    // Modes de projection 5.2
    ofxGuiGroup mode_projection;
    ofxButton perspectiveButton;
    ofxButton orthogonaleButton;
    void perspectiveButtonPressed();
    void orthogonaleButtonPressed();

    // Modes POV multiple 5.3
    ofxButton mode_dessincam_button;
    ofxButton mode_1cam_button;
    void mode_dessin_pressed();
    void mode_1cam_pressed();

    // Occlusion 5.4
    ofxButton meshfilled_button;
    ofxButton wireframe_button;
    void meshfilled_button_pressed();
    void wireframe_button_pressed();

    // Portail 5.5
    ofxButton mode_2cam_button;
    void mode_2cam_pressed();

    //Section 6 commence ici
    ofxGuiGroup groupe_texture;
    //Coordonnees de texture 6.1 ici vv

    //Filtrage 6.2 ici vv
    ofxGuiGroup groupe_filtrage;
    ofxButton filtrage_bilineaire_button;
    ofxButton filtrage_trilineaire_button;
    ofxButton filtrage_anisotropique_button;

    void bilineaireButtonPressed();
    void trilineaireButtonPressed();
    void anisotropiqueButtonPressed();


    //Mappage tonal 6.3 vv
    ofxGuiGroup groupe_mappage_tonal;
    ofxButton reset_slider_button;
    ofParameter<float> slider_exposure;
    ofParameter<float> slider_gamma;
    ofxButton default_mapping_button;
    ofParameter<bool> toggle_tone_mapping;
    bool mapping_Active;
    void reset_mapping_slidder();
    void default_mapping();

    //Cubemap 6.4 ici vv

    // Section 7
    ofxGuiGroup  groupe_illumination_cl;

    //Modele illumination 7.1 ici vv

    //Materiaux 7.2 ici vv

    //Types de lumieres 7.3 ici vv
    ofxGuiGroup groupe_lumiere;
    ofParameter<ofColor> color_picker_constante;
    ofxButton lumiere_ambiante_button;
    ofxButton lumiere_directionnelle_button;
    ofxButton lumiere_ponctuelle_button;
    ofxButton lumiere_spot_button;
    void lumiere_ambiante_button_pressed();
    void lumiere_directionnelle_button_pressed();
    void lumiere_ponctuelle_button_pressed();
    void lumiere_spot_button_pressed();
    //Lumiere multiples ici vv


    // Section 8
    ofxGuiGroup  groupe_topologie;

    //Courbe parametrique 8.1 ici vv

    //Surface parametrique 8.2 ici vv

    //Shader de tesselation 8.3 ici vv

    //Triangulation 8.4 ici vv

    // Section 9
    ofxGuiGroup  groupe_rayon;

    //Intersection 9.1 ici vv

    //Reflexion 9.2 ici vv

    //Refraction 9.3 ici vv

    //Ombrage 9.4 ici vv

    // Section 10
    ofxGuiGroup  groupe_illumination_moderne;

    //HDR 10.1 ici vv

    //Eclairage environnemental 10.2 ici vv

    //Metallicite 10.3 ici vv

    //Microfacettes 10.4 ici vv

    //Section declaration global
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
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button) override;
    void mouseDragged(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;

    void setup();
    void update();
    void draw();
    void exit();

private:
    bool captureMode; 
    bool captureMode_funny;
    
};
