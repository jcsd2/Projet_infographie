#include "renderer.h"

void Renderer::setup()
{
    ofSetFrameRate(60);
    clear_mode = ClearMode::none;
    // couleur fond arriere
    ofSetBackgroundColor(20);

    //Initisialisation des variable (Comme dans les exemples du cours)
    // nombre maximal de primitives vectorielles dans le tableau
    buffer_count = 100;
    // position de la prochaine primitive
    buffer_head = 0;
    // calculer la taille de la structure générique commune à toutes les primitives vectorielles
    buffer_stride = sizeof(VectorPrimitive);
    // calculer le nombre d'octets à allouer en mémoire pour contenir le tableau de primitives vectorielles
    buffer_size = buffer_count * buffer_stride;
    // allocation d'un espace mémoire suffisamment grand pour contenir les données de l'ensemble des primitives vectorielles
    shapes = (VectorPrimitive*)std::malloc(buffer_size);
    // mode de dessin par défaut
    draw_mode = VectorPrimitiveType::none;

    //Initisialisation general 1.0

    // couleur de la ligne de contour (color_stroke)
    stroke_color_r = 255;
    stroke_color_g = 255;
    stroke_color_b = 255;
    stroke_color_a = 255;
    // couleur de la zone de remplissage (color_fill())
    fill_color_r = 0;
    fill_color_g = 0;
    fill_color_b = 255;
    fill_color_a = 255;
    //largeur de la ligne de contour
    stroke_width_default = 500;
    radius = 4.0f;

    mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
    is_mouse_button_pressed = false;
}

void Renderer::draw()
{
    // couleur d'arrière-plan
    //ofClear(background_color1);
    ofClear(background_color2);

    for (index = 0; index < buffer_count; ++index)
    {
        switch (shapes[index].type)
        {
        case VectorPrimitiveType::none:
            break;

        case VectorPrimitiveType::pixel:

            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_pixel(
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::point:

            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_point(
                shapes[index].position2[0],
                shapes[index].position2[1],
                shapes[index].stroke_width);
            break;

        case VectorPrimitiveType::line:

            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_line(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::square:

            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_square(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_square(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::rectangle:

            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_rectangle(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_rectangle(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::circle:

            //Remplissage
            ofFill();
            ofSetLineWidth(0);
            ofSetCircleResolution(48);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_circle(shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);

            //Contour
            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_circle(shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;


        case VectorPrimitiveType::ellipse:

            ofFill();
            ofSetLineWidth(0);
            ofSetCircleResolution(48);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_ellipse(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_ellipse(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::triangle:
            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_triangle(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);

            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_triangle(
                shapes[index].position1[0], shapes[index].position1[1],
                shapes[index].position2[0], shapes[index].position2[1]);

            break;

        case VectorPrimitiveType::face:
            //Remplissage
            ofFill();
            ofSetLineWidth(0);
            ofSetCircleResolution(48);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_face(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::maison:
            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_maison(
                shapes[index].position1[0], shapes[index].position1[1],
                shapes[index].position2[0], shapes[index].position2[1]);
            break;

        default:
            break;
        }
    }

    // afficher la zone de sélection
    if (is_mouse_button_pressed)
    {
        draw_zone(
            mouse_press_x,
            mouse_press_y,
            mouse_current_x,
            mouse_current_y);
    }

}

// fonction qui vide le tableau de primitives vectorielles (Comme dans les exemples du cours)
void Renderer::reset()
{
    for (index = 0; index < buffer_count; ++index)
        shapes[index].type = VectorPrimitiveType::none;

    buffer_head = 0;

    ofLog() << "<reset>";
}

// fonction qui ajoute une primitive vectorielle au tableau (Comme dans les exemples du cours)
void Renderer::add_vector_shape(VectorPrimitiveType type)
{
    shapes[buffer_head].type = type;

    shapes[buffer_head].position1[0] = mouse_press_x;
    shapes[buffer_head].position1[1] = mouse_press_y;

    shapes[buffer_head].position2[0] = mouse_current_x;
    shapes[buffer_head].position2[1] = mouse_current_y;

    shapes[buffer_head].stroke_color[0] = stroke_color_r;
    shapes[buffer_head].stroke_color[1] = stroke_color_g;
    shapes[buffer_head].stroke_color[2] = stroke_color_b;
    shapes[buffer_head].stroke_color[3] = stroke_color_a;

    shapes[buffer_head].fill_color[0] = fill_color_r;
    shapes[buffer_head].fill_color[1] = fill_color_g;
    shapes[buffer_head].fill_color[2] = fill_color_b;
    shapes[buffer_head].fill_color[3] = fill_color_a;

    shapes[buffer_head].stroke_width = stroke_width_default;

    ofLog() << "<new primitive at index: " << buffer_head << ">";

    buffer_head = ++buffer_head >= buffer_count ? 0 : buffer_head; // boucler sur le tableau si plein
}

// fonction qui dessine un pixel (Comme dans les exemples du cours)
void Renderer::draw_pixel(float x, float y) const
{
    //Floorf : fonction qui arrondit un nombre flottant vers le bas pour obtenir le nombre entier immédiatement inférieur
    int pixel_x = floorf(x);
    int pixel_y = floorf(y);

    ofDrawRectangle(pixel_x, pixel_y, 1, 1);
}

// fonction qui dessine un point
void Renderer::draw_point(float x, float y, float radius) const
{
    ofDrawEllipse(x, y, radius, radius);
}

// fonction qui dessine une ligne
void Renderer::draw_line(float x1, float y1, float x2, float y2) const
{
    switch (lineRenderer) {
    case LineRenderer::none:
        // Utilisez l'algorithme par défaut ou choisissez-en un
        ofDrawLine(x1, y1, x2, y2);
        ofLog() << "<Ligne algo of>";
        break;
    case LineRenderer::dda:
        // Utilisez l'algorithme DDA pour rasteriser la ligne
        draw_line_dda(x1, y1, x2, y2);
        ofLog() << "<Ligne algo dda>";
        break;
    case LineRenderer::bresenham:
        // Utilisez l'algorithme Bresenham pour rasteriser la ligne
        draw_line_bresenham(x1, y1, x2, y2);
        ofLog() << "<Ligne algo bresenham>";
        break;
    default:
        break;
    }
}

// Fonction de dessin de ligne avec l'algorithme DDA
void Renderer::draw_line_dda(float x1, float y1, float x2, float y2) const
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float x_increment = dx / steps;
    float y_increment = dy / steps;
    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; ++i)
    {
        draw_pixel(x, y);
        x += x_increment;
        y += y_increment;
    }
}

// Fonction de dessin de ligne avec l'algorithme Bresenham
void Renderer::draw_line_bresenham(float x1, float y1, float x2, float y2) const
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x_sign = x1 < x2 ? 1 : -1;
    int y_sign = y1 < y2 ? 1 : -1;

    int error = dx - dy;

    int x = static_cast<int>(x1);
    int y = static_cast<int>(y1);

    draw_pixel(x, y);

    while (x != static_cast<int>(x2) || y != static_cast<int>(y2))
    {
        int error2 = error * 2;

        if (error2 > -dy)
        {
            error -= dy;
            x += x_sign;
        }

        if (error2 < dx)
        {
            error += dx;
            y += y_sign;
        }

        draw_pixel(x, y);
    }
}

// Fonction de dessin de rectangle
void Renderer::draw_square(float x1, float y1, float x2, float y2) const
{
    float squareX = min(x1, x2);
    float squareY = min(y1, y2);
    float squareSize = min(abs(x2 - x1), abs(y2 - y1));
    ofDrawRectangle(squareX, squareY, squareSize, squareSize);
}

// fonction qui dessine un rectangle (Comme dans les exemples du cours)
void Renderer::draw_rectangle(float x1, float y1, float x2, float y2) const
{
    ofDrawRectangle(x1, y1, x2 - x1, y2 - y1);
}

//Fonction qui dessine un cercle
void Renderer::draw_circle(float x1, float y1, float x2, float y2) const
{
    // Calculer la distance euclidienne entre les deux points
    float distance = ofDist(x1, y1, x2, y2);

    // Calculer le centre du cercle
    float centerX = (x1 + x2) / 2;
    float centerY = (y1 + y2) / 2;

    // Utiliser la moitié de la distance comme rayon (ou toute autre formule selon vos besoins)
    float radius_circle = distance / 2;
    // Utiliser le centre et le rayon pour dessiner le cercle
    ofDrawCircle(centerX, centerY, radius_circle);
}

// fonction qui dessine une ellipse (Comme dans les exemples du cours)
void Renderer::draw_ellipse(float x1, float y1, float x2, float y2) const
{
    float diameter_x = x2 - x1;
    float diameter_y = y2 - y1;

    ofDrawEllipse(x1 + diameter_x / 2.0f, y1 + diameter_y / 2.0f, diameter_x, diameter_y);
}

//Fonction qui dessine un triangle
void Renderer::draw_triangle(float x1, float y1, float x2, float y2) const
{
    // Calculer les points du triangle
    float baseLength = x2 - x1;
    float xCenter = (x1 + x2) / 2;

    float x1_triangle = xCenter - baseLength / 2;
    float y1_triangle = y2;
    float x2_triangle = xCenter + baseLength / 2;
    float y2_triangle = y2;
    float x3_triangle = xCenter;
    float y3_triangle = y1;

    // Dessiner le triangle
    ofDrawTriangle(x1_triangle, y1_triangle, x2_triangle, y2_triangle, x3_triangle, y3_triangle);
}


void Renderer::draw_face(float x1, float y1, float x2, float y2) const
{
    // Calculer la distance euclidienne entre les deux points
    float distance = ofDist(x1, y1, x2, y2);

    // Calculer le centre du cercle
    float centerX = (x1 + x2) / 2;
    float centerY = (y1 + y2) / 2;
    float largeur = (x2 - x1);
    float hauteur = (y2 - y1);

    // Utiliser la moitié de la distance comme rayon (ou toute autre formule selon vos besoins)
    float rayon = distance / 2;

    ofDrawCircle(centerX, centerY, rayon);
    ofSetColor(0, 0, 0);
    ofDrawCircle(centerX - largeur / 3, centerY - hauteur / 3, rayon / 10);
    ofDrawCircle(centerX + largeur / 3, centerY - hauteur / 3, rayon / 10);
    ofDrawLine(x1 + largeur / 4, y2 - hauteur / 4, x2 - largeur / 4, y2 - hauteur / 4);
}

void Renderer::draw_maison(float x1, float y1, float x2, float y2) const
{
    float hauteur = (y2 - y1);
    float largeur = (x2 - x1);

    float y_t = hauteur / 3;
    float x_t = largeur / 2;

    ofDrawTriangle(x1, y1 + y_t, x1 + x_t, y1, x2, y1 + y_t);
    ofSetColor(66);
    ofDrawRectangle(x1, y2 - hauteur * 2 / 3, largeur, hauteur * 2 / 3);
    ofSetColor(255, 0, 0);
    ofDrawRectangle(x1 + 0.2 * largeur, y2 - hauteur * 1 / 4, largeur * 0.1, hauteur * 0.25);
}





//Fonction de cahngement de algo pour les lignes
void Renderer::setLineRenderer(LineRenderer renderer)
{
    lineRenderer = renderer;
}





void Renderer::update()
{


}

//Fonction pour dessiner la zone de selection (Commed ans les exemples du cours)
void Renderer::draw_zone(float x1, float y1, float x2, float y2) const
{
    float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
    float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());

    ofSetLineWidth(radius);
    ofSetColor(255, 0, 0, 100);
    ofNoFill();
    ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
    ofFill();
    //Ellipse pour arrondir les coins de la selection
    ofDrawEllipse(x1, y1, radius, radius);
    ofDrawEllipse(x1, y2_clamp, radius, radius);
    ofDrawEllipse(x2_clamp, y1, radius, radius);
    ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
}

Renderer::~Renderer()
{
    std::free(shapes);
}
