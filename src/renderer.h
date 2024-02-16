#pragma once
#include "ofMain.h"

// énumération de différents modes pour changer la couleur d'arrière-plan
enum class ClearMode { none, gray };
// énumération des différents types de primitives vectorielles
enum class VectorPrimitiveType { none, pixel, point, line, square, rectangle, circle, ellipse, triangle, face, maison };

// énumération de différents algorithmes de rastérisation de ligne
enum class LineRenderer { none, dda, bresenham };

// structure de primitive vectorielle générique (Comme dans les exemples du cours)
struct VectorPrimitive
{
    VectorPrimitiveType type;            // 1 * 4 = 4  octets
    float               position1[2];    // 2 * 4 = 8  octets
    float               position2[2];    // 2 * 4 = 8  octets
    float               position3[3];    // 2 * 4 = 8  octets
    float               stroke_width;    // 1 * 4 = 4  octets
    unsigned char       stroke_color[4]; // 4 * 1 = 4  octets
    unsigned char       fill_color[4];   // 4 * 1 = 4  octets
};                                     //       = 34 octets

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

    // Fonctions et déclarations pour les transformations
    int get_last_primitive() const;
    void translateLastShape(float offsetX, float offsetY);
    void rotatePrimitive(VectorPrimitive& primitive, float angleDegrees);
    void scalePrimitive(VectorPrimitive& primitive, float scaleFactor);
    ofVec3f vector_origin;
    ofVec3f vector_position1;
    ofVec3f vector_position2;
    ofVec3f vector_position3;
    ofColor vector_color;
    float offset_x;
    float offset_y;
    float offset_z;
    float delta_x;
    float delta_y;
    float delta_z;
    float radious;
    float proportion;
    float speed;


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

    void setup();
    void update();
    void draw();
    void reset();

    void draw_zone(float x1, float y1, float x2, float y2) const;


    ~Renderer();
};
