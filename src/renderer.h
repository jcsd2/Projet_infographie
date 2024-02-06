#pragma once
#include "ofMain.h"

// énumération de différents modes pour changer la couleur d'arrière-plan
enum class ClearMode {none, gray};

class Renderer
{
    public:

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


    void setup();
    void update();
    void draw();
    void reset();

    void clear_to_gray(int g);
    void clear() const;
    void draw_zone(float x1, float y1, float x2, float y2) const;

    ~Renderer();
};

