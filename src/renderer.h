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
    bool has_changed;


    void setup();
    void update();
    void draw();
    void reset();

    void clear_to_gray(int g);
    void clear() const;

    ~Renderer();
};

