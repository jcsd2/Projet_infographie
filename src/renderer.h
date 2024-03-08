#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include <algorithm>
#include "ofxSvg.h"


// Énumération Occlusion
enum class Occlusion {none, wireframe, meshfiled};

// Énumération Couleur Background
enum class BackgroundColorType {none, rgb, hsb};

// Énumération Primitives Vectorielles
enum class VectorPrimitiveType { none, importedImage, pixel, point, line, square, rectangle, circle, ellipse, triangle, face, maison, cube, sphere };

// Énumération Algorithmes
enum class LineRenderer { none, dda, bresenham };

// Énumération Modele
enum class VectorModelType {none, predef1, predef2, predef3, import};

// Énumération Caméra
enum class Camera {devant, derriere, gauche, droite, dessus, dessous};

// Énumération Mode de Vue
enum class Mode_Vue {dessin, camera_3d, double_cam};

// Structure pour primitive
struct VectorPrimitive {
    int id; 
    VectorPrimitiveType type;
    float position1[3];
    float position2[3];
    float position3[3];
    float stroke_width;
    unsigned char stroke_color[4];
    unsigned char fill_color[4];
};

// Structure pour Modele 3D importer
struct VectorModel {
    VectorModelType type;
    float position1[3];
    float rotation[3];
    float proportion[3];
    float normal1[3];
    float texcoord1[2];
    unsigned char stroke_color[4];
    unsigned char fill_color[4];
};


class Renderer
{
public:


    void remove_vector_shap(int index);
    void remove_vector_model(int index);
    void select_vector_shap(int index);
    void select_vector_model(int index);
    int getBufferHead();
    int getBufferModelHead();
    int selectedShapeIndex = -1;
    int selectedModelIndex = -1;
    void zoomIn();
    void zoomOut();
    void rotateAround(float angle, ofVec3f axis);
    void changeView(Camera newView);
    bool is_camera_interactive;
    bool is_moving_forward;
    bool is_rotating_left;
    bool is_moving_backward;
    bool is_rotating_right;
   

    
    // Dimension du framebuffer
    float frame_buffer_width;
    float frame_buffer_heigth;
    ofColor background_color1;
    ofColor background_color2;
    BackgroundColorType color_mode;

    // Images
    ofImage importedImage;
    void import_image();

    //Declaration pours les primitives vectorieles
    VectorPrimitiveType draw_mode;
    VectorPrimitive* shapes;
    LineRenderer lineRenderer;
    int index;
    int buffer_count;
    int buffer_stride;
    int buffer_size;
    int buffer_head;
    float stroke_width_default;
    float radius;
    unsigned char stroke_color_r;
    unsigned char stroke_color_g;
    unsigned char stroke_color_b;
    unsigned char stroke_color_a;
    unsigned char fill_color_r;
    unsigned char fill_color_g;
    unsigned char fill_color_b;
    unsigned char fill_color_a;
    void add_vector_shape(VectorPrimitiveType type); // Fonction d'ajout de forme au vecteur(Il faut lajouter absolument)
    void draw_pixel(float x, float y) const;
    void draw_point(float x, float y, float radius) const;
    void draw_line(float x1, float y1, float x2, float y2) const;
    void draw_line_dda(float x1, float y1, float x2, float y2) const;
    void draw_line_bresenham(float x1, float y1, float x2, float y2) const;
    void draw_square(float x1, float y1, float x2, float y2) const;
    void draw_rectangle(float x1, float y1, float x2, float y2) const;
    void draw_circle(float x1, float y1, float x2, float y2) const;
    void draw_ellipse(float x1, float y1, float x2, float y2) const;
    void draw_triangle(float x1, float y1, float x2, float y2) const;
    void draw_face(float x1, float y1, float x2, float y2) const;
    void draw_maison(float x1, float y1, float x2, float y2) const;
    void draw_primitives();

    void setLineRenderer(LineRenderer renderer);

    // Histogramme
    bool is_active_histogram;
    ofImage histogram_im;
    void histogram();

    // Sélection Multiples
    std::vector<int> selectedIds; 
    void selectShape(int id); 
    void deselectShape(int id); 
    void clearSelection(); 
    void updateSelectedShapesAttribute(float newStrokeWidth, const ofColor& newStrokeColor, const ofColor& newFillColor);

    // Ajouts pour les transformations
    void rotateSelectedShapes(float angle);
    void scaleSelectedShapes(float scaleFactor);


    // Transformation
    int get_last_primitive() const;
    void translateLastShape(float offsetX, float offsetY);
    void rotatePrimitive(float angle);
    void rotatePoint(float x, float y, float centerX, float centerY, float angle, float& rotatedX, float& rotatedY);
    void scalePrimitive(float scaleFactor);
    float offset_x;
    float offset_y;
    float offset_z;
    float delta_x;
    float delta_y;
    float delta_z;
    float radious;
    float proportion;
    float angle;

    VectorPrimitive primitive_state;
    VectorPrimitive new_state;
    std::stack<VectorPrimitive> undoStack;
    std::stack<VectorPrimitive> redoStack;
    void undo();
    void redo();
    void execute();
    

    // Declaration temporaire
    int clear_color_gray;
    int clear_color_r;
    int clear_color_g;
    int clear_color_b;

    bool is_mouse_button_pressed;
    bool has_changed;

    int mouse_press_x;
    int mouse_press_y;

    int mouse_current_x;
    int mouse_current_y;

    // Outils de dessin
    float lineThickness;
    ofColor lineColor;
    ofColor fillColor;

    // Formes 3D
    ofxAssimpModelLoader model;
    int modelIndex;
    int animationIndex;
    float animationPosition;
    float mTimeModelLoaded;
    ofMesh mesh;
    bool bAnimate;
    void loadModels();
    void loadModel(string filename);
    
    // Modeles VBO
    int buffer_model_count;
    int buffer_model_head;
    int buffer_model_stride;
    int buffer_models_size;
    VectorModel *models;
    VectorModelType draw_mode_models;
    void init_buffer_model();
    void add_vector_models(VectorModelType type);
    ofxAssimpModelLoader model1;
    ofxAssimpModelLoader model2;
    ofxAssimpModelLoader model3;
    int get_last_model() const;
    void translateLastModel(float offsetX, float offsetY, float offsetZ);
    void drawModels();

    // Primitives geometrique 3D
    void drawCube(float x, float y, float z, float width, float height, float depth) const;
    void drawSphere(float x, float y, float z, float radius) const;
    float sphereRayon;

    // Instanciation
    void dispatch_random_models(int count, float range);
    ofVec3f vector_position;
    ofVec3f vector_rotation;
    ofVec3f vector_proportion;
    bool instanciation_active;

    // Modeles 3D
    void drawCubeSVG();
    ofxSvg svg;
    float deg;
    float step;
    vector<ofPolyline> outlines;
    bool animation_svg_object_active;

    // Camera
    Camera camera_active;
    ofCamera camera_devant;
    ofCamera camera_derriere;
    ofCamera camera_gauche;
    ofCamera camera_droite;
    ofCamera camera_dessus;
    ofCamera camera_dessous;
    ofCamera *camera;
    ofQuaternion camera_orientation;
    ofVec3f camera_position;
    ofVec3f camera_target;
    string camera_nom;
    string camera_name;
    string camera_projection;
    float camera_near_clipping;
    float camera_far_clipping;
    float camera_near;
    float camera_far;
    bool is_camera_fov_narrow;
    bool is_camera_fov_wide;
    bool is_camera_perspective;
    float camera_fov;
    float camera_fov_delta;
    float speed_delta;
    float speed_translation;
    float speed_rotation;
    float offset_camera;
    float offset_color;
    float offset_scene;
    bool is_visible_axes;
    bool is_visible_camera;
    bool is_camera_move_left;
    bool is_camera_move_right;
    bool is_camera_move_up;
    bool is_camera_move_down;
    bool is_camera_move_forward;
    bool is_camera_move_backward;
    bool is_camera_tilt_up;
    bool is_camera_tilt_down;
    bool is_camera_pan_left;
    bool is_camera_pan_right;
    bool is_camera_roll_left;
    bool is_camera_roll_right;
    bool mode_cam;
    bool is_flip_axis_y;
    void setup_camera();


    // Mode affichage actuel
    Mode_Vue mode_vue;

    // Occlusion
    Occlusion occlusion;

    void setup();

    // Temps
    float time_current;
    float time_last;
    float time_elapsed;

    void update();
    void draw();
    void reset();

    void draw_zone(float x1, float y1, float x2, float y2) const;

    int generate_unique_id();

  // Remove/Select/Translate
    void remove_vector_shape(int id);
    void select_vector_shape(int id);
    void translateSelectedShapes(float offsetX, float offsetY);


    ofNode node;

    float center_x;
    float center_y;

    ofPoint minBounds;
    ofPoint maxBounds;
    bool bBoundingBoxCalculated = false;

    void calculateBoundingBox();
    void drawBoundingBox();

    


   

    

    ~Renderer();

    private:
        
};
