#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUndoSimple.h"

// énumération de différents modes pour changer la couleur d'arrière-plan
enum class ClearMode { none, gray };
// énumération des différents types de primitives vectorielles
enum class VectorPrimitiveType { none, pixel, point, line, square, rectangle, circle, ellipse, triangle, face, maison };

// énumération de différents algorithmes de rastérisation de ligne
enum class LineRenderer { none, dda, bresenham };

// structure de primitive vectorielle générique (Comme dans les exemples du cours)
/*struct VectorPrimitive
{
    VectorPrimitiveType type;            // 1 * 4 = 4  octets
    float               position1[2];    // 2 * 4 = 8  octets
    float               position2[2];    // 2 * 4 = 8  octets
    float               position3[3];    // 2 * 4 = 8  octets
    float               stroke_width;    // 1 * 4 = 4  octets
    unsigned char       stroke_color[4]; // 4 * 1 = 4  octets
    unsigned char       fill_color[4];   // 4 * 1 = 4  octets
};*/                                     //       = 34 octets

struct VectorPrimitive {
    int id; 
    VectorPrimitiveType type;
    float position1[2];
    float position2[2];
    float position3[3];
    float stroke_width;
    unsigned char stroke_color[4];
    unsigned char fill_color[4];
};

class Renderer
{
public:


    
    //Dimension du framebuffer
    float frame_buffer_width;
    float frame_buffer_heigth;
    //ofColor background_color1;
    ofColor background_color2;

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
    void add_vector_shape(VectorPrimitiveType type); //Fonction d'ajout de forme au vecteur(Il faut lajouter absolument)
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

    void setLineRenderer(LineRenderer renderer);

    // Fonction et déclaration pour Sélection Multiples

    std::vector<int> selectedIds; // IDs des formes sélectionnées
    void selectShape(int id); // Sélectionner une forme par son ID
    void deselectShape(int id); // Désélectionner une forme par son ID
    void clearSelection(); // Désélectionner toutes les formes
    void updateSelectedShapesAttribute(float newStrokeWidth, const ofColor& newStrokeColor, const ofColor& newFillColor); // Mettre à jour les attributs des formes sélectionnées

    // Ajouts pour les transformations
    void rotateSelectedShapes(float angle); // Tourner les formes sélectionnées
    void scaleSelectedShapes(float scaleFactor); // Mettre à l'échelle les formes sélectionnées


    // Fonctions et déclarations pour les transformations
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
    float speed;
    float angle;




    //Temporary declaration
    ClearMode clear_mode;
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

    // Paramètres de dessin
    float lineThickness; // Epaisseur de la ligne
    ofColor lineColor; // Couleur de lignes 
    ofColor fillColor; // Couleur de remplissage

    //Section 4
    //Fonction de dessin des formes 3d
    void drawModels(const std::vector<ofxAssimpModelLoader*>& models);


    void setup();
    void update();
    void draw();
    void reset();

    void draw_zone(float x1, float y1, float x2, float y2) const;

    int generate_unique_id();

  
    void remove_vector_shape(int id);
    void select_vector_shape(int id);
    void translateSelectedShapes(float offsetX, float offsetY);

    ~Renderer();

    private:
        // Data structure to store position data for undo
        struct UndoData {
            float x1, y1, x2, y2;
        };

        // Undo mechanism for position data
        ofxUndoSimple<UndoData> undoData_;
};
