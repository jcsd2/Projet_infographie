#include "renderer.h"

void Renderer::setup()
{
    ofSetFrameRate(60);
    // couleur fond arriere
    ofSetBackgroundColor(20);
    ofLog() << "<app::BackgroundColor:" << ofGetBackgroundColor();
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
    //mode de couleur
    color_mode = BackgroundColorType::rgb;

    //Speed de deplacement
    speed_delta = 100.0f;
    delta_x = speed_delta;
    delta_y = speed_delta;
    delta_z = speed_delta;
    angle = 0.0f;


    mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
    is_mouse_button_pressed = false;
    is_active_histogram = false;

    //Initialisation des variable pour les modeles 3d
    modelIndex = 1;
    animationIndex = 0;
    mTimeModelLoaded = 0.f;
    animationPosition = 0;
    bAnimate = true;

    is_camera_perspective = false;

    //Init cam
    camera_position = {0.0f, 0.0f, 0.0f};
    camera_target = {0.0f, 0.0f, 0.0f};
    camera_near = 3.0f;
    camera_far = 1750.0f;
    camera_fov = 60.0f;
    camera_fov_delta = 16.0f;
    is_camera_move_left = false;
    is_camera_move_right = false;
    is_camera_move_up = false;
    is_camera_move_down = false;
    is_camera_move_forward = false;
    is_camera_move_backward = false;
    is_camera_tilt_up = false;
    is_camera_tilt_down = false;
    is_camera_pan_left = false;
    is_camera_pan_right = false;
    is_camera_roll_left = false;
    is_camera_roll_right = false;
    is_camera_fov_narrow = false;
    is_camera_fov_wide = false;
    is_camera_interactive = false;
    is_flip_axis_y = false;
    offset_x = 0;
    offset_z = 0;

    //Mode de vue dessin ou 3d
    mode_vue = Mode_Vue::dessin;

    loadModels();
    init_buffer_model();


    setup_camera();
    

    occlusion = Occlusion::meshfiled;

}

void Renderer::draw()
{
    switch (mode_vue)
    {
    case Mode_Vue::dessin:

        switch (color_mode)
        {
        case BackgroundColorType::rgb:
            ofSetBackgroundColor(background_color1);
            break;

        case BackgroundColorType::hsb:
            ofSetBackgroundColor(background_color2);
        default:
            break;
        }
        if (is_mouse_button_pressed)
        {
            draw_zone(
                mouse_press_x,
                mouse_press_y,
                mouse_current_x,
                mouse_current_y);
        }
        if (is_active_histogram){
            histogram();
        }
        if (animation_svg_object_active)
        {
            drawCubeSVG();
        }
        ofPushMatrix();
        draw_primitives();
        drawModels();
        ofPopMatrix();
        break;
    
    case Mode_Vue::camera_3d:
        camera->begin();
        
        ofScale(1.0f, is_flip_axis_y ? -1.0f : 1.0f);
        ofFill();
        //ofTranslate((frame_buffer_width/2) + offset_x, is_flip_axis_y ? - (frame_buffer_heigth/2) : (frame_buffer_heigth/2), offset_z);
        ofTranslate(camera_target);
        ofSetColor(127);
        node.setPosition(0,0,-30);
        node.draw();
        
        if (is_visible_camera)
        {
            if (camera_active != Camera::devant)
                camera_devant.draw();
            if (camera_active != Camera::derriere)
                camera_derriere.draw();
            if (camera_active != Camera::gauche)
                camera_gauche.draw();
            if (camera_active != Camera::droite)
                camera_droite.draw();
            if (camera_active != Camera::dessus)
                camera_dessus.draw();
            if (camera_active != Camera::dessous)
                camera_dessous.draw();
        }
        if (is_mouse_button_pressed)
        {
            draw_zone(
                mouse_press_x,
                mouse_press_y,
                mouse_current_x,
                mouse_current_y);
        }
        if(instanciation_active)
        {
            dispatch_random_models(10, min(ofGetWidth() * 0.6f, ofGetHeight() * 0.6f));
            ofLog() << "<Dispatch done>";
            instanciation_active = false;
        }
        draw_primitives();
        drawModels();

        camera->end();
        break;

    case Mode_Vue::double_cam:
        glViewport(0,0, ofGetWindowWidth()/2.0, ofGetWindowHeight());
        camera = &camera_gauche;
        camera->begin();
        draw_primitives();
        drawModels();
        camera->end();
        glViewport(ofGetWindowWidth() / 2, 0, ofGetWindowWidth() / 2, ofGetWindowHeight());
        camera = &camera_droite;
        camera->begin();
        draw_primitives();
        drawModels();
        if (is_visible_camera)
        {
            if (camera_active != Camera::devant)
                camera_devant.draw();
            if (camera_active != Camera::derriere)
                camera_derriere.draw();
            if (camera_active != Camera::gauche)
                camera_gauche.draw();
            if (camera_active != Camera::droite)
                camera_droite.draw();
            if (camera_active != Camera::dessus)
                camera_dessus.draw();
            if (camera_active != Camera::dessous)
                camera_dessous.draw();
        }
        camera->end();
        break;
    
    default:
        break;

    if (is_visible_axes)
        ofDrawRotationAxes(64);
}
}

// fonction qui vide le tableau de primitives vectorielles (Comme dans les exemples du cours)
void Renderer::reset()
{
    for (index = 0; index < buffer_count; ++index)
    {
        shapes[index].type = VectorPrimitiveType::none;
        buffer_head = 0;
    }
    for (index =0 ; index < buffer_model_count; ++index){
        models[index].type = VectorModelType::none;
        buffer_model_head = 0;
    }
    offset_camera = 5.0f;
    camera_devant.setPosition(0, 0, -offset_camera);
    camera_derriere.setPosition(0, 0, offset_camera);
    camera_gauche.setPosition(-offset_camera, 0, 0);
    camera_droite.setPosition(offset_camera, 0, 0);
    camera_dessus.setPosition(0, offset_camera, 0);
    camera_dessous.setPosition(0, -offset_camera, 0);
    camera_active = Camera::devant;

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


    shapes[buffer_head].stroke_width = (lineThickness > 0) ? lineThickness : stroke_width_default; //outil de dessin
    shapes[buffer_head].stroke_color[0] = (lineColor.a > 0) ? lineColor.r : stroke_color_r;
    shapes[buffer_head].stroke_color[1] = (lineColor.a > 0) ? lineColor.g : stroke_color_g;
    shapes[buffer_head].stroke_color[2] = (lineColor.a > 0) ? lineColor.b : stroke_color_g;
    shapes[buffer_head].stroke_color[3] = (lineColor.a > 0) ? lineColor.a : stroke_color_a;

    shapes[buffer_head].fill_color[0] = (fillColor.a > 0) ? fillColor.r : fill_color_r;
    shapes[buffer_head].fill_color[1] = (fillColor.a > 0) ? fillColor.g : fill_color_g;
    shapes[buffer_head].fill_color[2] = (fillColor.a > 0) ? fillColor.b : fill_color_b;
    shapes[buffer_head].fill_color[3] = (fillColor.a > 0) ? fillColor.a : fill_color_a;

    switch (shapes[buffer_head].type)
    {
    case VectorPrimitiveType::importedImage:
        shapes[buffer_head].position1[0] = 0;
        shapes[buffer_head].position1[1] = 0;
        break;
    case VectorPrimitiveType::rectangle:
    {
        ofColor couleur_hsb = ofColor::fromHsb(background_color2.r, background_color2.g, background_color2.b);
        float hue = couleur_hsb.getHue();
        float sat = couleur_hsb.getSaturation();
        float lumi = couleur_hsb.getBrightness();
        shapes[buffer_head].fill_color[0] = hue;
        shapes[buffer_head].fill_color[1] = sat;
        shapes[buffer_head].fill_color[2] = lumi;
        ofLog() << "<Color to hsb r: " << couleur_hsb <<"hue:" <<hue<<"sat:"<<sat<< "Lumi : "<< lumi;
        break; 
    }
    default:
        break;
    }

    ofLog() << "<new primitive at index: " << buffer_head << ">";
    execute();
    buffer_head = ++buffer_head >= buffer_count ? 0 : buffer_head; // boucler sur le tableau si plein
}

//Fonction qui importe une image et load celle-ci
void Renderer::import_image()
{
    string defaultPath = ofFilePath::getUserHomeDir();
    ofLog() << "Default path" << defaultPath;
    ofFileDialogResult result = ofSystemLoadDialog("Importer une image", false);
    if (result.bSuccess) {
        importedImage.load(result.getPath());
        draw_mode = VectorPrimitiveType::importedImage;
    } else {
        std::string path;
        std::cout << "Veuillez entrer le chemin de l'image à importer : ";
        std::getline(std::cin, path);

        if (!path.empty()) {
            importedImage.load(path);
            draw_mode = VectorPrimitiveType::importedImage;
        } else {
            ofLog() << "Erreur import";
        }
    }

}

// fonction qui dessine un pixel (Comme dans les exemples du cours)
void Renderer::draw_pixel(float x, float y) const
{
    //Floorf : fonction qui arrondit un nombre flottant vers le bas pour obtenir le nombres entier immédiatement inférieur
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
        break;
    case LineRenderer::dda:
        // Utilisez l'algorithme DDA pour rasteriser la ligne
        draw_line_dda(x1, y1, x2, y2);
        break;
    case LineRenderer::bresenham:
        // Utilisez l'algorithme Bresenham pour rasteriser la ligne
        draw_line_bresenham(x1, y1, x2, y2);
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

void Renderer::draw_primitives()
{
    for (index = 0; index < buffer_count; ++index)
    {
        ofPushMatrix();
        ofPushStyle();
        switch (shapes[index].type)
        {
        case VectorPrimitiveType::none:
            break;
        case VectorPrimitiveType::importedImage:
            importedImage.draw(shapes[index].position1[0], shapes[index].position1[1]);
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

        case VectorPrimitiveType::cube: {
            ofFill();
            ofSetLineWidth(0);
            ofSetColor(shapes[index].fill_color[0], shapes[index].fill_color[1], shapes[index].fill_color[2]);

            float cubeWidth = 100;
            float cubeHeight = 100;
            float cubeDepth = 100;

            drawCube(
                shapes[index].position1[0], // x
                shapes[index].position1[1], // y
                0,
                cubeWidth, cubeHeight, cubeDepth);

            break;
        }
        case VectorPrimitiveType::sphere: {
            ofFill();
            ofSetLineWidth(0);
            ofSetColor(shapes[index].fill_color[0], shapes[index].fill_color[1], shapes[index].fill_color[2]);
            float sphereRayon = 50;

            drawSphere(
                shapes[index].position1[0],
                shapes[index].position1[1],
                0,
                sphereRayon);
            break;
        }

        default:
            break;
        }
        ofPopMatrix();
        ofPopStyle();
    }
}

// Section 4 Primitives géometrique sphere et cube

void Renderer::drawCube(float x, float y, float z, float width, float height, float depth) const {

    ofSetColor(255, 0, 0);
    ofFill();
    ofDrawBox(x, y, z, width, height, depth);

}


void Renderer::drawSphere(float x, float y, float z, float radius) const {


    ofSetColor(0, 255, 0); // Couleur verte fixée
    ofFill();
    ofDrawSphere(x, y, z, radius);

}


void Renderer::drawCubeSVG() {

    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    ofScale(0.2);
    ofRotateDeg(deg, 0, 0, deg);
    if (ofGetMousePressed()) {
        ofNoFill();
        for (ofPolyline& line : outlines) {
            int num = step * line.size();

            ofBeginShape();
            for (int j = 0; j < num; j++) {
                ofVertex(line[j]);
            }
            ofEndShape();
        }
    }
    else {
        svg.draw();
    }

    ofPopMatrix();
}

//Fonction pour faire instanciation comme les exemples du cours
void Renderer::dispatch_random_models(int count, float range)
{
  float scale;

  if (count <= 0 || range <= 0 || count > buffer_model_count)
    return;

  float halfRange = range / 2.0f;

  buffer_model_head = count;

  for (int index = 0; index < buffer_model_head; ++index)
  {
    vector_position.x = ofRandom(-halfRange, halfRange);
    vector_position.y = ofRandom(-halfRange, halfRange);
    vector_position.z = ofRandom(-halfRange, halfRange);

    vector_rotation.x = 0.0f;
    vector_rotation.y = ofRandom(0.0f, 360.0f);
    vector_rotation.z = 0.0f;

    scale = ofRandom(0.05f, 0.35f);

    vector_proportion.x = scale;
    vector_proportion.y = scale;
    vector_proportion.z = scale;

    models[index].position1[0] = vector_position.x;
    models[index].position1[1] = vector_position.y;
    models[index].position1[2] = vector_position.z;

    models[index].rotation[0] = vector_rotation.x;
    models[index].rotation[1] = vector_rotation.y;
    models[index].rotation[2] = vector_rotation.z;

    models[index].proportion[0] = vector_proportion.x;
    models[index].proportion[1] = vector_proportion.y;
    models[index].proportion[2] = vector_proportion.z;
  }
}

//Fonction de cahngement de algo pour les lignes
void Renderer::setLineRenderer(LineRenderer renderer)
{
    lineRenderer = renderer;
}

int Renderer::get_last_primitive() const {
    // Récupérer la dernière primitive ajoutée
    int lastIndex = buffer_head - 1;
    if (lastIndex < 0) {
        lastIndex = buffer_count - 1; // Si buffer_head est à 0, le dernier élément est à buffer_count - 1
    }
    return lastIndex;
}

int Renderer::get_last_model() const {
    // Récupérer la dernière primitive ajoutée
    int lastIndex = buffer_model_head - 1;
    if (lastIndex < 0) {
        lastIndex = buffer_model_count - 1; // Si buffer_head est à 0, le dernier élément est à buffer_count - 1
    }
    return lastIndex;
}

// Fonction Selection Multiple a complété (shape / deselectionne/ clearer et updater)
void Renderer::selectShape(int id) {
    // Ajouter l'ID à la liste des formes sélectionnées si ce n'est pas déjà le cas
    if (std::find(selectedIds.begin(), selectedIds.end(), id) == selectedIds.end()) {
        selectedIds.push_back(id);
    }
}

void Renderer::deselectShape(int id) {
    // Retirer l'ID de la liste des formes sélectionnées
    selectedIds.erase(std::remove(selectedIds.begin(), selectedIds.end(), id), selectedIds.end());
}

void Renderer::clearSelection() {
    // Effacer la liste des formes sélectionnées
    selectedIds.clear();
}

void Renderer::updateSelectedShapesAttribute(float newStrokeWidth, const ofColor& newStrokeColor, const ofColor& newFillColor) {
    // Mettre à jour les attributs pour chaque forme sélectionnée
    for (int id : selectedIds) {
        if (id >= 0 && id < buffer_count) {
            shapes[id].stroke_width = newStrokeWidth;
            shapes[id].stroke_color[0] = newStrokeColor.r;
            shapes[id].stroke_color[1] = newStrokeColor.g;
            shapes[id].stroke_color[2] = newStrokeColor.b;
            shapes[id].stroke_color[3] = newStrokeColor.a;
            shapes[id].fill_color[0] = newFillColor.r;
            shapes[id].fill_color[1] = newFillColor.g;
            shapes[id].fill_color[2] = newFillColor.b;
            shapes[id].fill_color[3] = newFillColor.a;
        }
    }
}

void Renderer::translateLastShape(float offsetX, float offsetY) {
    int dernier_primitive = get_last_primitive();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_count) {
        //execute();

        shapes[dernier_primitive].position1[0] += offsetX;
        shapes[dernier_primitive].position1[1] += offsetY;
        shapes[dernier_primitive].position2[0] += offsetX;
        shapes[dernier_primitive].position2[1] += offsetY;
        
    }
}

void Renderer::translateLastModel(float offsetX, float offsetY, float offsetZ) {
    int dernier_primitive = get_last_model();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_model_count) {
        models[dernier_primitive].position1[0] += offsetX;
        models[dernier_primitive].position1[1] += offsetY;
    }
}

void Renderer::rotatePrimitive(float angle){
    int dernier_primitive = get_last_primitive();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_count) {
        //Centre de la forme
        float centerX = (shapes[dernier_primitive].position1[0] + shapes[dernier_primitive].position2[0]) / 2.0f;
        float centerY = (shapes[dernier_primitive].position1[1] + shapes[dernier_primitive].position2[1]) / 2.0f;
        // Translate to the center, rotate, and then translate back
        ofPushMatrix();
        ofTranslate(centerX, centerY);
        ofRotateZDeg(angle); // Rotate around the Z-axis (2D rotation)
        ofTranslate(-centerX, -centerY);

        // Update the shape's positions after rotation
        float rotatedX1, rotatedY1, rotatedX2, rotatedY2;
        rotatePoint(shapes[dernier_primitive].position1[0], shapes[dernier_primitive].position1[1], centerX, centerY, angle, rotatedX1, rotatedY1);
        rotatePoint(shapes[dernier_primitive].position2[0], shapes[dernier_primitive].position2[1], centerX, centerY, angle, rotatedX2, rotatedY2);

        shapes[dernier_primitive].position1[0] = rotatedX1;
        shapes[dernier_primitive].position1[1] = rotatedY1;
        shapes[dernier_primitive].position2[0] = rotatedX2;
        shapes[dernier_primitive].position2[1] = rotatedY2;

        ofPopMatrix();
    }
}

void Renderer::rotatePoint(float x, float y, float centerX, float centerY, float angle, float& rotatedX, float& rotatedY) {
    float radians = ofDegToRad(angle);
    float cosA = cos(radians);
    float sinA = sin(radians);

    // Rotate the point around the center
    rotatedX = cosA * (x - centerX) - sinA * (y - centerY) + centerX;
    rotatedY = sinA * (x - centerX) + cosA * (y - centerY) + centerY;
}

void Renderer::scalePrimitive(float scaleFactor){
    int dernier_primitive = get_last_primitive();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_count) {
        shapes[dernier_primitive].position2[0] *= scaleFactor;
        shapes[dernier_primitive].position2[1] *= scaleFactor;
    }
}

void Renderer::undo(){
    if(!undoStack.empty())
    {
        // On pousse l'état actuel dans la pile de rétablissement
        redoStack.push(shapes[buffer_head]);

        // On récupère l'état précédent depuis la pile d'annulation
        shapes[buffer_head] = undoStack.top();
        undoStack.pop();
    }
}

void Renderer::redo(){
    if(!redoStack.empty())
    {
        // On pousse l'état actuel dans la pile d'annulation
        undoStack.push(shapes[buffer_head]);

        // On récupère l'état suivant depuis la pile de rétablissement
        shapes[buffer_head] = redoStack.top();
        redoStack.pop();
    }
}


void Renderer::execute()
{

    while(!redoStack.empty())
    {
        redoStack.pop();
    }
    VectorPrimitive new_state = primitive_state;

    new_state.type = shapes[buffer_head].type;
    new_state.position1[0] = shapes[buffer_head].position1[0];
    new_state.position1[1] = shapes[buffer_head].position1[1];
    new_state.position2[0] = shapes[buffer_head].position2[0];
    new_state.position2[1] = shapes[buffer_head].position2[1];
    new_state.stroke_width = shapes[buffer_head].stroke_width;
    new_state.stroke_color[0] = shapes[buffer_head].stroke_color[0];
    new_state.stroke_color[1] = shapes[buffer_head].stroke_color[1];
    new_state.stroke_color[2] = shapes[buffer_head].stroke_color[2];
    new_state.stroke_color[3] = shapes[buffer_head].stroke_color[3];

    undoStack.push(new_state);
    if(!undoStack.empty()){
        ofLog() << "Pile non vide";
    }
}


void Renderer::update()
{
    frame_buffer_width = ofGetWidth();
    frame_buffer_heigth = ofGetHeight();
    model.update();
    if( model.hasAnimations() )
    {
        mesh = model.getCurrentAnimatedMesh(0);
    }
    mTimeModelLoaded = ofGetElapsedTimef();

    calculateBoundingBox();

    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    speed_translation = speed_delta * time_elapsed;
    speed_rotation = speed_translation / 8.0f;

    if (is_camera_move_left)
        camera->truck(-speed_translation);
    if (is_camera_move_right)
        camera->truck(speed_translation);
    if (is_camera_move_up)
        camera->boom(speed_translation);
    if (is_camera_move_down)
        camera->boom(-speed_translation);
    if (is_camera_move_forward)
        camera->dolly(-speed_translation);
    if (is_camera_move_backward)
        camera->dolly(speed_translation);
    if (is_camera_tilt_up)
        camera->tiltDeg(-speed_rotation);
    if (is_camera_tilt_down)
        camera->tiltDeg(speed_rotation);
    if (is_camera_pan_left)
        camera->panDeg(speed_rotation);
    if (is_camera_pan_right)
        camera->panDeg(-speed_rotation);
    if (is_camera_roll_left)
        camera->rollDeg(-speed_rotation);
    if (is_camera_roll_right)
        camera->rollDeg(speed_rotation);

    //camera_fov = std::max(camera_fov -= camera_fov_delta * time_elapsed, 0.0f);
    camera->setFov(camera_fov);


    //4.3
    step += 0.001;
    if (step > 1) {
        step -= 1;
    }

    deg += 1;
    if (deg > 360) {
        deg = 0;
    }
}

//Fonction pour dessiner la zone de selection (Commed ans les exemples du cours)
void Renderer::draw_zone(float x1, float y1, float x2, float y2) const
{
    float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
    float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());

    ofPushMatrix();
    ofSetLineWidth(radius);
    //ofSetColor(255, 0, 0, 100);
    ofNoFill();
    ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
    ofFill();
    //Ellipse pour arrondir les coins de la selection
    ofDrawEllipse(x1, y1, radius, radius);
    ofDrawEllipse(x1, y2_clamp, radius, radius);
    ofDrawEllipse(x2_clamp, y1, radius, radius);
    ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
    ofPopMatrix();
}
void Renderer::histogram()
{
    int x = ofGetWidth();
    int y = ofGetHeight();
    histogram_im.allocate(x, y, OF_IMAGE_COLOR);
    histogram_im.grabScreen(0,0,x,y);
    screenGrayscale = histogram_im;
    contourFinders.findContours(screenGrayscale, 10, ofGetWidth() * ofGetHeight(), 1, false);
    for (int i = 0; i < contourFinders.nBlobs; ++i)
    {
        float bar_x = ofMap(i, 0, contourFinders.nBlobs, 0, ofGetWidth());
        float height = ofMap(contourFinders.blobs[i].area, 0, ofGetWidth() * ofGetHeight(), 0, ofGetHeight());

        // Adjust the drawing of rectangles
        ofPushMatrix();
        ofTranslate(0, 0); // Translate to the top-left corner
        ofScale(0.7);
        ofDrawRectangle(bar_x, y, ofGetWidth() * 0.7 / contourFinders.nBlobs, -height);
        ofPopMatrix();
    }
}


int Renderer::generate_unique_id() {
    static int current_id = 0;
    return current_id++;
}



void Renderer::remove_vector_shape(int id) {
    for (int i = 0; i < buffer_count; ++i) {
        if (shapes[i].id == id) {
           
            shapes[i].type = VectorPrimitiveType::none;
            break;
        }
    }
}


std::vector<int> selected_ids;

void Renderer::select_vector_shape(int id) {
   
    selected_ids.push_back(id);
}

void Renderer::translateSelectedShapes(float offsetX, float offsetY) {
    for (int id : selected_ids) {
        for (int i = 0; i < buffer_count; ++i) {
            if (shapes[i].id == id) {
                shapes[i].position1[0] += offsetX;
                shapes[i].position1[1] += offsetY;
                shapes[i].position2[0] += offsetX;
                shapes[i].position2[1] += offsetY;
            }
        }
    }
}

void Renderer::init_buffer_model(){
    // nombre maximal de models dans le tableau
    buffer_model_count = 100;
    // position du prochain model
    buffer_model_head = 0;
    // calculer la taille de la structure générique
    buffer_model_stride = sizeof(VectorModel);
    // calculer le nombre d'octets à allouer en mémoire pour contenir le tableau
    buffer_models_size = buffer_model_count * buffer_model_stride;
    // allocation d'un espace mémoire suffisamment grand pour contenir les données
    models = (VectorModel*)std::malloc(buffer_models_size);
    // mode de dessin par défaut
    draw_mode_models = VectorModelType::none;
}

//
void Renderer::loadModels(){
    buffer_model_count = 100;
	model1.loadModel("Bender/Bender.gltf");
	model2.loadModel("Obama/Obama.fbx");
	model3.loadModel("Shinji/Shinji.obj");
    ofSetVerticalSync(true);
    svg.load("cube.svg");
    for (ofPath p : svg.getPaths()) {
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        const vector<ofPolyline>& lines = p.getOutline();
        for (const ofPolyline& line : lines) {
            outlines.push_back(line.getResampledBySpacing(1));
        }
    }
    ofLog() << " All models loaded ";
}

void Renderer::loadModel(string filename){
    
    if( model.load(filename, ofxAssimpModelLoader::OPTIMIZE_DEFAULT) ){
        if( model.hasAnimations() ){
            animationIndex = 0;
            model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
            model.getAnimation(animationIndex).play();
        }
    }else{
        ofLogError() << " can't load model: " << filename << endl;
    }
	mTimeModelLoaded = ofGetElapsedTimef();
}

void Renderer::add_vector_models(VectorModelType type)
{
    vector_position.x = 0.0f;
    vector_position.y = 0.0f;
    vector_position.z = 0.0f;

    vector_rotation.x = 0.0f;
    vector_rotation.y = 0.0f;
    vector_rotation.z = 0.0f;
    vector_proportion.x = 0.0f;
    vector_proportion.y = 0.0f;
    vector_proportion.z = 0.0f;

    switch (mode_vue)
    {
    case Mode_Vue::dessin:
        models[buffer_model_head].type = type;
        models[buffer_model_head].position1[0] = mouse_press_x;
        models[buffer_model_head].position1[1] = mouse_press_y;
        models[buffer_model_head].position1[2] = 0;
        models[buffer_model_head].rotation[0] = vector_rotation.x;
        models[buffer_model_head].rotation[1] = vector_rotation.y;
        models[buffer_model_head].rotation[2] = vector_rotation.z;
        models[buffer_model_head].proportion[0] = vector_proportion.x;
        models[buffer_model_head].proportion[1] = vector_proportion.y;
        models[buffer_model_head].proportion[2] = vector_proportion.z;

        ofLog() << "<new model at index: " << buffer_model_head << ">";
        buffer_model_head = ++buffer_model_head >= buffer_model_count ? 0 : buffer_model_head;
        break;
    
    case Mode_Vue::camera_3d:
        models[buffer_model_head].type = type;
        models[buffer_model_head].position1[0] = vector_position.x;
        models[buffer_model_head].position1[1] = vector_position.y;
        models[buffer_model_head].position1[2] = vector_position.z;
        models[buffer_model_head].rotation[0] = vector_rotation.x;
        models[buffer_model_head].rotation[1] = vector_rotation.y;
        models[buffer_model_head].rotation[2] = vector_rotation.z;
        models[buffer_model_head].proportion[0] = vector_proportion.x;
        models[buffer_model_head].proportion[1] = vector_proportion.y;
        models[buffer_model_head].proportion[2] = vector_proportion.z;

        ofPushMatrix();
        ofScale(0.4);
        ofPopMatrix();
        ofLog() << "<new model at index: " << buffer_model_head << ">";
        buffer_model_head = ++buffer_model_head >= buffer_model_count ? 0 : buffer_model_head;
    break;

    default:
        break;
    }


}

void Renderer::drawModels()
{
for (index = 0; index < buffer_model_count; ++index)
    {
        switch (models[index].type)
        {
        case VectorModelType::none:
            break;

        case VectorModelType::predef1:
            ofPushMatrix();
            ofScale(0.1);
            ofPopMatrix();

            model1.setPosition( models[index].position1[0],
                                models[index].position1[1],
                                models[index].position1[2]);
            switch (occlusion)
            {
            case Occlusion::meshfiled:
                model1.draw(OF_MESH_FILL);
                break;

            case Occlusion::wireframe:
                model1.drawWireframe();
                break;

            default:
                break;
            }
        break;

        case VectorModelType::predef2:
            model2.setPosition( models[index].position1[0],
                                models[index].position1[1],
                                models[index].position1[2]);
            switch (occlusion)
            {
            case Occlusion::meshfiled:
                model2.draw(OF_MESH_FILL);
                break;

            case Occlusion::wireframe:
                model2.drawWireframe();
                break;

            default:
                break;
            }
        break;
        case VectorModelType::predef3:
            model3.setPosition( models[index].position1[0],
                                models[index].position1[1],
                                models[index].position1[2]);
            switch (occlusion)
            {
            case Occlusion::meshfiled:
                model3.draw(OF_MESH_FILL);
                break;

            case Occlusion::wireframe:
                model3.drawWireframe();
                break;

            default:
                break;
            }
        break;
        case VectorModelType::import:
            break;
        }
 
    }
}

void Renderer::calculateBoundingBox() {
    if (!model.getMeshCount()) return; 

    minBounds.set(FLT_MAX, FLT_MAX, FLT_MAX);
    maxBounds.set(FLT_MIN, FLT_MIN, FLT_MIN);

    for (unsigned int i = 0; i < model.getMeshCount(); i++) {
        ofMesh mesh = model.getMesh(i);
        for (auto& vertex : mesh.getVertices()) {
            minBounds.x = std::min(minBounds.x, vertex.x);
            minBounds.y = std::min(minBounds.y, vertex.y);
            minBounds.z = std::min(minBounds.z, vertex.z);

            maxBounds.x = std::max(maxBounds.x, vertex.x);
            maxBounds.y = std::max(maxBounds.y, vertex.y);
            maxBounds.z = std::max(maxBounds.z, vertex.z);
        }
    }

    bBoundingBoxCalculated = true;
}


void Renderer::drawBoundingBox() {
    if (!bBoundingBoxCalculated) return;
    ofNoFill();
    ofDrawBox((minBounds.x + maxBounds.x) / 2, (minBounds.y + maxBounds.y) / 2, (minBounds.z + maxBounds.z) / 2, maxBounds.x - minBounds.x, maxBounds.y - minBounds.y, maxBounds.z - minBounds.z);
    ofFill();
}

//Fonction de configuration très inspiré des exmples du cours
void Renderer::setup_camera()
{

    switch (camera_active)
    {
    case Camera::devant:
        camera = &camera_devant;
        camera_nom = "Avant";
        break;
    case Camera::derriere:
        camera = &camera_derriere;
        camera_nom = "Arrière";
        break;
    case Camera::gauche:
        camera = &camera_gauche;
        camera_nom = "Gauche";
        break;
    case Camera::droite:
        camera = &camera_droite;
        camera_nom = "Droite";
        break;
    case Camera::dessus:
        camera = &camera_dessus;
        camera_nom = "Haut";
        break;
    case Camera::dessous:
        camera = &camera_dessous;
        camera_nom = "Bas";
        break;
    default:
        break;
    }
    camera_position = camera->getPosition();
    camera_orientation = camera->getOrientationQuat();

    // mode de projection de la caméra
    if (!is_camera_perspective)
    {
        camera->disableOrtho();
        camera->setupPerspective(false, camera_fov, camera_near, camera_far, ofVec2f(0, 0));
        camera_projection = "perspective";
    }
    else
    {
        camera->enableOrtho();
        camera_projection = "orthogonale";
    }

    camera->setPosition(camera_position);
    camera->setOrientation(camera_orientation);

}

Renderer::~Renderer()
{
    std::free(shapes);
    std::free(models);
}

void Renderer::remove_vector_shap(int index) {
    if (index < 0 || index >= buffer_head) return;

    for (int i = index; i < buffer_head - 1; i++) {
        shapes[i] = shapes[i + 1];
    }

    buffer_head--;
    ofLog() << "<removed shape at index: " << index << ">";
}

void Renderer::remove_vector_model(int index) {
    if (index < 0 || index >= buffer_model_head) return;

    for (int i = index; i < buffer_model_head - 1; i++) {
        models[i] = models[i + 1];
    }

    buffer_model_head--;
    ofLog() << "<removed model at index: " << index << ">";
}




void Renderer::select_vector_shap(int index) {
    if (index < 0 || index >= buffer_head) return;
    selectedShapeIndex = index;
    ofLog() << "<selected shape at index: " << index << ">";
}

void Renderer::select_vector_model(int index) {
    if (index < 0 || index >= buffer_model_head) return;
    selectedModelIndex = index;
    ofLog() << "<selected model at index: " << index << ">";
}

int Renderer::getBufferHead() {
    return buffer_head;
}

int Renderer::getBufferModelHead() {
    return buffer_model_head;
}

void Renderer::zoomIn() {
    camera->dolly(-speed_translation);
}

void Renderer::zoomOut() {
    camera->dolly(speed_translation);
}

void Renderer::rotateAround(float angle, ofVec3f axis) {
    camera->rotateAround(angle, axis, camera_target);
}

void Renderer::changeView(Camera newView) {
    camera_active = newView;
    setup_camera();
}
