#include "renderer.h"

/*
* brief: Initialise les paramètres et variables nécessaires pour le rendu
*/

void Renderer::setup()
{
    // Frame
    ofSetFrameRate(60);
    // Couleur Background
    ofSetBackgroundColor(20);


    // Nombre maximal de primitives vectorielles dans le tableau
    buffer_count = 100;
    // Position de la prochaine primitive
    buffer_head = 0;
    // Calculer la taille de la structure générique commune à toutes les primitives vectorielles
    buffer_stride = sizeof(VectorPrimitive);
    // Calculer le nombre d'octets à allouer en mémoire pour contenir le tableau de primitives vectorielles
    buffer_size = buffer_count * buffer_stride;
    // Allocation d'un espace mémoire suffisamment grand pour contenir les données de l'ensemble des primitives vectorielles
    shapes = (VectorPrimitive*)std::malloc(buffer_size);
    // Mode de dessin par défaut
    draw_mode = VectorPrimitiveType::none;

    //Initialisation
 
    //  Couleur de la ligne de contour (color_stroke)
    stroke_color_r = 255;
    stroke_color_g = 255;
    stroke_color_b = 255;
    stroke_color_a = 255;
    // Couleur de la zone de remplissage (color_fill())
    fill_color_r = 0;
    fill_color_g = 0;
    fill_color_b = 255;
    fill_color_a = 255;
    // Largeur de la ligne de contour
    stroke_width_default = 500;
    radius = 4.0f;
    // Mode de couleur
    color_mode = BackgroundColorType::rgb;

    // Speed deplacement
    speed_delta = 100.0f;
    delta_x = speed_delta;
    delta_y = speed_delta;
    delta_z = speed_delta;
    angle = 0.0f;


    mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
    is_mouse_button_pressed = false;
    is_active_histogram = false;

    //Initialisation variables modele 3D
    modelIndex = 1;
    animationIndex = 0;
    mTimeModelLoaded = 0.f;
    animationPosition = 0;
    bAnimate = true;

    is_camera_perspective = true;

    //Initialisation camera
    camera_position = {0.0f, 0.0f, 50.0f};
    camera_target = {0.0f, 0.0f, 0.0f};
    camera_near = 10.0f;
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
    is_flip_axis_y = true;
    offset_x = 0;
    offset_z = 0;

    //Mode de vue dessin ou 3d
    mode_vue = Mode_Vue::dessin;

    loadModels();
    init_buffer_model();

    //Section cam interactive 5.1
    setup_camera();
    //Section cam occlu 5.4
    occlusion = Occlusion::meshfiled;

    //Section 6.1
    load_shader_illumination();
    shader__illumination_active = ShaderType::blinn_phong;
    box_shader.set(60);
    box_shader.setPosition(0, 0, 0);

    // Section filtrage 6.2
    is_bilineaire = false;
    is_trilineaire = false;
    is_anisotropique = false;

    //Section mapping 6.3
    tone_mapping_exposure = 1.0f;
    tone_mapping_gamma = 2.2f;
    tone_mapping_toggle = true;
    shader_mapping.load("Tone_mapping_shader/tone_mapping_vs.glsl", "Tone_mapping_shader/tone_mapping_fs.glsl");

    //Section 7.1 init des couleur
    fill_color_illumination.set(0.0f, 0.0f, 1.0f);
    ambient_color_illumination.set(1.0f, 1.0f, 1.0f);
    diffuse_color.set(1.0f, 1.0f, 1.0f);
    specular_color.set(1.0f, 1.0f, 1.0f);
    shininess = 128.0f;
    update_material();

    //Section materiaux 7.2
  
    is_material_cube = false;
    is_material_sphere = false;
    is_material_prisme = false;

    default_cube_width = 100.0f;
    default_cube_height = 100.0f;
    default_cube_depth = 100.0f;

    default_sphere_radius = 50.0f;

    default_prisme_width = 100.0f;
    default_prisme_height = 200.0f;
    default_prisme_depth = 100.0f;



    //Section 7.3
    shader_lighting.load("light_shader/light_330_vs.glsl", "light_shader/light_330_fs.glsl");
    init_buffer_lumiere();
    add_lumiere();
    pixels_rouge.allocate(2, 2, OF_PIXELS_RGB);
    pixels_rouge.setColor(ofColor(255, 0, 0)); // Rouge pur
    texture_box.loadData(pixels_rouge);
    //Initialisation box
    box.set(50);
    box.setPosition(0, 0, 0);

    //Initialisation lumiere
    ambiant_color_init();
    ajout_lumiere();

    //cubemap.loadImages("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg");

   //Section 8.2
    is_coon_parametrique = false;
    
}

/*
* brief: Exécute les opérations de dessin à chaque frame en fonction du mode de vue.
*/

void Renderer::draw()
{
    switch (mode_vue)
    {
    case Mode_Vue::dessin:
        
        //Endroit pour activer/desactiver la lumiere
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
        if(tone_mapping_activated)
        {
            apply_tone_mapping();
        }
        break;

        
        
    case Mode_Vue::camera_3d:

        //Shader lumiere test1
        

        camera->begin();
        is_camera_interactive = true;
        ofScale(1.0f, is_flip_axis_y ? -1.0f : 1.0f);
        ofFill();
        ofTranslate(camera_target);
        ofSetColor(127);
        ofPushMatrix();
        ofRotateDeg(90);
        ofScale(3);
        ofDrawGridPlane();
        ofPopMatrix();
        node.setPosition(0,0,0);
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


        //Endroit pour activer la lumiere
        //il faut appeler shader_lighting.begin() avant de dessiner les objets
        //mettre ajout_lumiere dans setup et ambiantcolor init aussi pour couleur ambiante
        //dessiner les objets
        //mettre shader_lighting.end() apres avoir dessiner les objets


        //Teste box_shader avec le materiau global et le shader_illumination
        shader_lighting.begin();
        
        shader_illumination->begin();
        ofPushMatrix();
        material_global.begin();
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofDrawBox(0, 0, 0, 100);
        ofPopStyle();
        material_global.end();
        ofPopMatrix();
        shader_illumination->end();

        shader_lighting.end();

        //ofPopStyle();
        draw_primitives();
        drawModels();

        camera->end();

        //Endroit pour desactiver la lumiere
        

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

    if (is_bilineaire) {
        bilineaire_application();
    }
    else if (is_trilineaire) {
        trilineaire_application();
    }
    else if (is_anisotropique) {
        anisotropique_application();
    }
    if (is_material_cube) {
        // Dessinez un cube à la position du dernier clic.
        drawCube_mat(mouse_press_x, mouse_press_y, 0); // Z est 0 si vous dessinez en 2D, ajustez si nécessaire.
    }

    if (is_material_sphere) {
        // Dessinez une sphère à la position du dernier clic.
        drawSphere_mat(mouse_press_x, mouse_press_y, 0); // Ajustez Z si nécessaire.
    }

    if (is_material_prisme) {
        // Dessinez un prisme à la position du dernier clic.
        drawPrisme_mat(mouse_press_x, mouse_press_y, 0); // Ajustez Z si nécessaire.
    }


    cubemap.bind();
    
    cubemap.unbind();

}

/*
* brief: Réinitialise les formes et modèles vectoriels ainsi que la caméra à leurs états par défaut.
*/

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

/*
* brief: Ajoute une forme vectorielle à la scène en utilisant les paramètres actuels.
*/

void Renderer::add_vector_shape(VectorPrimitiveType type)
{

    shapes[buffer_head].type = type;

    shapes[buffer_head].position1[0] = mouse_press_x;
    shapes[buffer_head].position1[1] = mouse_press_y;

    shapes[buffer_head].position2[0] = mouse_current_x;
    shapes[buffer_head].position2[1] = mouse_current_y;


    shapes[buffer_head].stroke_width = (lineThickness > 0) ? lineThickness : stroke_width_default;
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
    buffer_head = ++buffer_head >= buffer_count ? 0 : buffer_head;
}

/*
* brief: Importe une image et la définit comme la forme vectorielle courante à dessiner.
*/

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

/*
* brief: Dessine un pixel à la position spécifiée.
*/

void Renderer::draw_pixel(float x, float y) const
{
    int pixel_x = floorf(x);
    int pixel_y = floorf(y);

    ofDrawRectangle(pixel_x, pixel_y, 1, 1);
}

/*
* brief: Dessine un point à la position spécifiée avec le rayon donné.
*/

void Renderer::draw_point(float x, float y, float radius) const
{
    ofDrawEllipse(x, y, radius, radius);
}

/*
* brief: Dessine une ligne entre deux points en utilisant l'algorithme de rendu de ligne spécifié.
*/

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

/*
* brief: Dessine une ligne entre deux points en utilisant l'algorithme DDA.
*/

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

/*
* brief: Dessine une ligne entre deux points en utilisant l'algorithme Bresenham.
*/

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

/*
* brief: Dessine un carré avec les coordonnées spécifiées.
*/

void Renderer::draw_square(float x1, float y1, float x2, float y2) const
{
    float squareX = min(x1, x2);
    float squareY = min(y1, y2);
    float squareSize = min(abs(x2 - x1), abs(y2 - y1));
    ofDrawRectangle(squareX, squareY, squareSize, squareSize);
}

/*
* brief: Dessine un rectangle avec les coordonnées spécifiées.
*/

void Renderer::draw_rectangle(float x1, float y1, float x2, float y2) const
{
    ofDrawRectangle(x1, y1, x2 - x1, y2 - y1);
}

/*
* brief: Dessine un cercle basé sur deux points spécifiés.
*/

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

/*
* brief: Dessine une ellipse basée sur deux points spécifiés.
*/

void Renderer::draw_ellipse(float x1, float y1, float x2, float y2) const
{
    float diameter_x = x2 - x1;
    float diameter_y = y2 - y1;

    ofDrawEllipse(x1 + diameter_x / 2.0f, y1 + diameter_y / 2.0f, diameter_x, diameter_y);
}

/*
* brief: Dessine un triangle basé sur deux points spécifiés.
*/

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

/*
* brief: Dessine une face
*/

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

/*
* brief: Dessine une maison
*/

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

/*
* brief: Dessine toutes les primitives vectorielles actuellement stockées.
*/

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
                shapes[index].position1[0],
                shapes[index].position1[1],
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

/*
* brief: Dessine un cube rouge
*/


void Renderer::drawCube(float x, float y, float z, float width, float height, float depth) const {

    ofSetColor(255, 0, 0);
    ofFill();
    ofDrawBox(x, y, z, width, height, depth);

}

/*
* brief: Dessine une sphere verte
*/


void Renderer::drawSphere(float x, float y, float z, float radius) const {


    ofSetColor(0, 255, 0);
    ofFill();
    ofDrawSphere(x, y, z, radius);

}

/*
* brief: Dessine un cube en utilisant une image SVG pour sa texture.
*/


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

/*
* brief: Génère un ensemble de modèles à des positions aléatoires dans une plage donnée.
*/

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

/*
* brief: Définit l'algorithme de rendu de ligne à utiliser pour dessiner les lignes.
*/

void Renderer::setLineRenderer(LineRenderer renderer)
{
    lineRenderer = renderer;
}

/*
* brief: Récupère l'indice de la dernière primitive vectorielle ajoutée au buffer.
*/

int Renderer::get_last_primitive() const {
    int lastIndex = buffer_head - 1;
    if (lastIndex < 0) {
        lastIndex = buffer_count - 1;
    }
    return lastIndex;
}

/*
* brief: Récupère l'indice du dernier modèle vectoriel ajouté au buffer.
*/

int Renderer::get_last_model() const {
   
    int lastIndex = buffer_model_head - 1;
    if (lastIndex < 0) {
        lastIndex = buffer_model_count - 1;
    }
    return lastIndex;
}

/*
* brief: Sélectionne une forme spécifique pour les opérations ultérieures.
*/

void Renderer::selectShape(int id) {
    if (std::find(selectedIds.begin(), selectedIds.end(), id) == selectedIds.end()) {
        selectedIds.push_back(id);
    }
}

/*
* brief: Désélectionne une forme spécifique.
*/

void Renderer::deselectShape(int id) {
    selectedIds.erase(std::remove(selectedIds.begin(), selectedIds.end(), id), selectedIds.end());
}

/*
* brief: Efface toutes les sélections actuelles.
*/

void Renderer::clearSelection() {
    selectedIds.clear();
}

/*
* brief: Met à jour les attributs des formes sélectionnées.
*/

void Renderer::updateSelectedShapesAttribute(float newStrokeWidth, const ofColor& newStrokeColor, const ofColor& newFillColor) {
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

/*
* brief: Translate la dernière forme ajoutée par les décalages spécifiés.
*/

void Renderer::translateLastShape(float offsetX, float offsetY) {
    int dernier_primitive = get_last_primitive();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_count) {

        shapes[dernier_primitive].position1[0] += offsetX;
        shapes[dernier_primitive].position1[1] += offsetY;
        shapes[dernier_primitive].position2[0] += offsetX;
        shapes[dernier_primitive].position2[1] += offsetY;
        
    }
}

/*
* brief: Translate le dernier modèle ajouté par les décalages spécifiés.
*/

void Renderer::translateLastModel(float offsetX, float offsetY, float offsetZ) {
    int dernier_primitive = get_last_model();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_model_count) {
        models[dernier_primitive].position1[0] += offsetX;
        models[dernier_primitive].position1[1] += offsetY;
    }
}

/*
* brief: Effectue une rotation de la forme spécifiée.
*/

void Renderer::rotatePrimitive(float angle){
    int dernier_primitive = get_last_primitive();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_count) {
  
        float centerX = (shapes[dernier_primitive].position1[0] + shapes[dernier_primitive].position2[0]) / 2.0f;
        float centerY = (shapes[dernier_primitive].position1[1] + shapes[dernier_primitive].position2[1]) / 2.0f;

        ofPushMatrix();
        ofTranslate(centerX, centerY);
        ofRotateZDeg(angle);
        ofTranslate(-centerX, -centerY);


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

/*
* brief: Effectue une rotation d'un point autour d'un centre donné.
*/

void Renderer::rotatePoint(float x, float y, float centerX, float centerY, float angle, float& rotatedX, float& rotatedY) {
    float radians = ofDegToRad(angle);
    float cosA = cos(radians);
    float sinA = sin(radians);

    rotatedX = cosA * (x - centerX) - sinA * (y - centerY) + centerX;
    rotatedY = sinA * (x - centerX) + cosA * (y - centerY) + centerY;
}

/*
* brief: Met à l'échelle la dernière primitive vectorielle ajoutée.
*/

void Renderer::scalePrimitive(float scaleFactor){
    int dernier_primitive = get_last_primitive();
    if (dernier_primitive >= 0 && dernier_primitive < buffer_count) {
        shapes[dernier_primitive].position2[0] *= scaleFactor;
        shapes[dernier_primitive].position2[1] *= scaleFactor;
    }
}

/*
* brief: Annule la dernière opération effectuée.
*/

void Renderer::undo(){
    if(!undoStack.empty())
    {

        redoStack.push(shapes[buffer_head]);


        shapes[buffer_head] = undoStack.top();
        undoStack.pop();
    }
}

/*
* brief: Réapplique l'opération annulée la plus récente.
*/

void Renderer::redo(){
    if(!redoStack.empty())
    {
        undoStack.push(shapes[buffer_head]);

   
        shapes[buffer_head] = redoStack.top();
        redoStack.pop();
    }
}

/*
* brief: Applique les changements et nettoie la pile de redo.
*/

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

/*
* brief: Met à jour les paramètres et effectue les calculs nécessaires à chaque frame.
*/

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

    camera->setFov(camera_fov);


    step += 0.001;
    if (step > 1) {
        step -= 1;
    }

    deg += 1;
    if (deg > 360) {
        deg = 0;
    }
    //mise a jour shader illumination 
    update_shader_illumination();
    //mise a jour des lumieres
    mise_a_jour_lumiere();
}

/*
* brief: Dessine une zone de sélection rectangulaire.
*/

void Renderer::draw_zone(float x1, float y1, float x2, float y2) const
{
    float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
    float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());

    ofPushMatrix();
    ofSetLineWidth(radius);

    ofNoFill();
    ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
    ofFill();

    ofDrawEllipse(x1, y1, radius, radius);
    ofDrawEllipse(x1, y2_clamp, radius, radius);
    ofDrawEllipse(x2_clamp, y1, radius, radius);
    ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
    ofPopMatrix();
}

/*
* brief: Calcule et affiche un histogramme basé sur l'image à l'écran.
*/

void Renderer::histogram()
{
    int x = ofGetWidth();
    int y = ofGetHeight();
    histogram_im.allocate(x, y, OF_IMAGE_COLOR);
    histogram_im.grabScreen(0,0,x,y);

    ofImage grayImage;
    grayImage = histogram_im;
    grayImage.setImageType(OF_IMAGE_GRAYSCALE);

    vector<int> histogram(256, 0);


    for (std::vector<int>::size_type y = 0; y < grayImage.getHeight(); y++) {
        for (std::vector<int>::size_type x = 0; x < grayImage.getWidth(); x++) 
        {
            ofColor color = grayImage.getColor(x, y);
            histogram[color.r]++;
        }
    }
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(ofColor::red);
    ofScale(0.4);
    ofTranslate(ofGetWidth() * 2.0,0);
    for (std::vector<int>::size_type i = 0; i < histogram.size(); i++) 
    {
        ofDrawRectangle(i * 2, ofGetHeight(), 2, -histogram[i] / 100.0);
    }
    ofPopStyle();
    ofPopMatrix();
}

/*
* brief: Génère un identifiant unique pour les nouvelles primitives vectorielles ou modèles.
*/

int Renderer::generate_unique_id() {
    static int current_id = 0;
    return current_id++;
}

/*
* brief: Supprime la forme vectorielle avec l'ID spécifié.
*/

void Renderer::remove_vector_shape(int id) {
    for (int i = 0; i < buffer_count; ++i) {
        if (shapes[i].id == id) {
           
            shapes[i].type = VectorPrimitiveType::none;
            break;
        }
    }
}


std::vector<int> selected_ids;

/*
* brief: Selectionner shape
*/

void Renderer::select_vector_shape(int id) {
   
    selected_ids.push_back(id);
}

/*
* brief: Translate les formes vectorielles sélectionnées.
*/

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

/*
* brief: Initialise le buffer pour les modèles vectoriels.
*/

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

/*
* brief: Charge et prépare les modèles 3D pour le rendu.
*/

void Renderer::loadModels(){
    buffer_model_count = 100;
	model1.load("Bender/Bender.gltf");
	model2.load("Obama/Obama.fbx");
	model3.load("Shinji/Shinji.obj");
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

/*
* brief: Charge un modèle 3D à partir d'un fichier.
*/

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

/*
* brief: Ajoute un modèle vectoriel à la scène en fonction du type spécifié et du mode de vue actuel.
*/

void Renderer::add_vector_models(VectorModelType type)
{
    vector_position.x = 0.0f;
    vector_position.y = 0.0f;
    vector_position.z = -20.0f;

    vector_rotation.x = 0.0f;
    vector_rotation.y = 0.0f;
    vector_rotation.z = 0.0f;
    vector_proportion.x = 1.0f;
    vector_proportion.y = 1.0f;
    vector_proportion.z = 1.0f;

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
        models[buffer_model_head].proportion[0] = vector_proportion.x * 0.07f;
        models[buffer_model_head].proportion[1] = vector_proportion.y * 0.07f;
        models[buffer_model_head].proportion[2] = vector_proportion.z * 0.07f;

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

/*
* brief: Dessine les modèles en fonction de leur type et de l'occlusion spécifiée.
*/

void Renderer::drawModels()
{
for (index = 0; index < buffer_model_count; ++index)
    {
        switch (models[index].type)
        {
        case VectorModelType::none:
            break;

        case VectorModelType::predef1:
            model1.setPosition( models[index].position1[0],
                                models[index].position1[1],
                                models[index].position1[2]);
            model1.setScale(models[index].proportion[0],
                                models[index].proportion[1],
                                models[index].proportion[2]) ;
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
            model2.setScale(models[index].proportion[0],
                                models[index].proportion[1],
                                models[index].proportion[2]) ;
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
            model3.setScale(models[index].proportion[0],
                                models[index].proportion[1],
                                models[index].proportion[2]) ;
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

/*
* brief: Calcule la boîte de delimitation pour le modèle chargé.
*/

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

/*
* brief: Dessine la boîte englobante autour du modèle.
*/

void Renderer::drawBoundingBox() {
    if (!bBoundingBoxCalculated) return;
    ofNoFill();
    ofDrawBox((minBounds.x + maxBounds.x) / 2, (minBounds.y + maxBounds.y) / 2, (minBounds.z + maxBounds.z) / 2, maxBounds.x - minBounds.x, maxBounds.y - minBounds.y, maxBounds.z - minBounds.z);
    ofFill();
}

/*
* brief: Configure la caméra en fonction du mode de vue actuel.
*/

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

    // Mode de projection
    if (is_camera_perspective)
    {
        camera->disableOrtho();
        camera->setupPerspective(false, camera_fov, camera_near, camera_far, ofVec2f(0, 0));
        camera_projection = "perspective";
        ofLog() << "Mode perspective";
    }
    else
    {
        camera->enableOrtho();
        camera_projection = "orthogonale";
        ofLog() << "Mode ortho";
    }

    camera->setPosition(camera_position);
    camera->setOrientation(camera_orientation);

}



/*
* brief : Supprime la forme vectorielle a l'index specifie, decale toutes les formes suivantes
*/

void Renderer::remove_vector_shap(int index) {
    if (index < 0 || index >= buffer_head) return;

    for (int i = index; i < buffer_head - 1; i++) {
        shapes[i] = shapes[i + 1];
    }

    buffer_head--;
    ofLog() << "<removed shape at index: " << index << ">";
}

/*
* brief : Supprime le modele vectoriel a l'index specifie, decale toutes les formes suivantes
*/

void Renderer::remove_vector_model(int index) {
    if (index < 0 || index >= buffer_model_head) return;

    for (int i = index; i < buffer_model_head - 1; i++) {
        models[i] = models[i + 1];
    }

    buffer_model_head--;
    ofLog() << "<removed model at index: " << index << ">";
}

/*
* brief : Sélectionne la forme vectorielle à l'index spécifié
*/

void Renderer::select_vector_shap(int index) {
    if (index < 0 || index >= buffer_head) return;
    selectedShapeIndex = index;
    ofLog() << "<selected shape at index: " << index << ">";
}

/*
* brief : Sélectionne le modèle vectoriel à l'index spécifié pour des opérations ultérieures
*/

void Renderer::select_vector_model(int index) {
    if (index < 0 || index >= buffer_model_head) return;
    selectedModelIndex = index;
    ofLog() << "<selected model at index: " << index << ">";
}

/*
* brief : Retourne l'index actuel de la tête du buffer pour les formes vectorielles
*/

int Renderer::getBufferHead() {
    return buffer_head;
}

/*
* brief : Retourne l'index actuel de la tête du buffer pour les modèles vectoriels
*/

int Renderer::getBufferModelHead() {
    return buffer_model_head;
}

/*
* brief : Effectue un zoom avant en déplaçant la caméra vers l'avant
*/

void Renderer::zoomIn() {
    camera->dolly(-speed_translation);
}

/*
* brief : Effectue un zoom arrière en déplaçant la caméra vers l'arrière
*/

void Renderer::zoomOut() {
    camera->dolly(speed_translation);
}

/*
* brief : Fait pivoter la caméra autour d'un axe et d'un angle donnés
*/

void Renderer::rotateAround(float angle, ofVec3f axis) {
    camera->rotateAroundDeg(angle, axis, camera_target);
}

/*
* brief : Change la vue de la caméra en fonction du mode spécifié
*/

void Renderer::changeView(Camera newView) {
    camera_active = newView;
    setup_camera();
}

//Section 6.1 loading shaders illumination
void Renderer::load_shader_illumination()
{
    shader_color_fill.load(
    "illumination_shader/color_fill_330_vs.glsl",
    "illumination_shader/color_fill_330_fs.glsl");
    shader_lambert.load(
    "illumination_shader/lambert_330_vs.glsl",
    "illumination_shader/lambert_330_fs.glsl");
    shader_gouraud.load(
    "illumination_shader/gouraud_330_vs.glsl",
    "illumination_shader/gouraud_330_fs.glsl");
    shader_phong.load(
    "illumination_shader/phong_330_vs.glsl",
    "illumination_shader/phong_330_fs.glsl");
    shader_blinn_phong.load(
    "illumination_shader/blinn_phong_330_vs.glsl",
    "illumination_shader/blinn_phong_330_fs.glsl");
}

//Fonction pour passer attribut au shader dillumination
void Renderer::update_shader_illumination()
{
    switch (shader__illumination_active)
  {
    case ShaderType::color_fill:
        shader_illumination_name = "Color Fill";
        shader_illumination = &shader_color_fill;
        shader_illumination->begin();
        shader_illumination->setUniform3f("color", fill_color_illumination.x, fill_color_illumination.y, fill_color_illumination.z);
        shader_illumination->end();
        break;

    case ShaderType::lambert:
        shader_illumination_name = "Lambert";
        shader_illumination = &shader_lambert;
        shader_illumination->begin();
        shader_illumination->setUniform3f("color_ambient", ambient_color_illumination.x, ambient_color_illumination.y, ambient_color_illumination.z);
        shader_illumination->setUniform3f("color_diffuse", diffuse_color.x, diffuse_color.y, diffuse_color.z);
        //envoyer les positions de tous les lumiere de lumieres[1 'a 3].position
        shader_illumination->setUniform3f("light_position", lumieres[0].position);
        shader_illumination->setUniform3f("light_position", lumieres[1].position);
        shader_illumination->setUniform3f("light_position", lumieres[2].position);
        shader_illumination->setUniform3f("light_position", lumieres[3].position);
        shader_illumination->end();
      break;

    case ShaderType::gouraud:
        shader_illumination_name = "Gouraud";
        shader_illumination = &shader_gouraud;
        shader_illumination->begin();
        shader_illumination->setUniform3f("color_ambient", ambient_color_illumination.x, ambient_color_illumination.y, ambient_color_illumination.z);
        shader_illumination->setUniform3f("color_diffuse", diffuse_color.x, diffuse_color.y, diffuse_color.z);
        shader_illumination->setUniform3f("color_specular", specular_color.x, specular_color.y, specular_color.z);
        shader_illumination->setUniform1f("brightness", shininess);
        shader_illumination->setUniform3f("light_position", lumieres[0].position);
        shader_illumination->setUniform3f("light_position", lumieres[1].position);
        shader_illumination->setUniform3f("light_position", lumieres[2].position);
        shader_illumination->setUniform3f("light_position", lumieres[3].position);
      shader_illumination->end();
      break;

    case ShaderType::phong:
        shader_illumination_name = "Phong";
        shader_illumination = &shader_phong;
        shader_illumination->begin();
        shader_illumination->setUniform3f("color_ambient", ambient_color_illumination.x, ambient_color_illumination.y, ambient_color_illumination.z);
        shader_illumination->setUniform3f("color_diffuse", diffuse_color.x, diffuse_color.y, diffuse_color.z);
        shader_illumination->setUniform3f("color_specular", specular_color.x, specular_color.y, specular_color.z);
        shader_illumination->setUniform1f("brightness", shininess);
        shader_illumination->setUniform3f("light_position", lumieres[0].position);
        shader_illumination->setUniform3f("light_position", lumieres[1].position);
        shader_illumination->setUniform3f("light_position", lumieres[2].position);
        shader_illumination->setUniform3f("light_position", lumieres[3].position);
        shader_illumination->end();
      break;

    case ShaderType::blinn_phong:
        shader_illumination_name = "Blinn-Phong";
        shader_illumination = &shader_blinn_phong;
        shader_illumination->begin();
        shader_illumination->setUniform3f("color_ambient", ambient_color_illumination.x, ambient_color_illumination.y, ambient_color_illumination.z);
        shader_illumination->setUniform3f("color_diffuse", diffuse_color.x, diffuse_color.y, diffuse_color.z);
        shader_illumination->setUniform3f("color_specular", specular_color.x, specular_color.y, specular_color.z);
        shader_illumination->setUniform1f("brightness", shininess);
        shader_illumination->setUniform3f("light_position", lumieres[0].position);
        shader_illumination->setUniform3f("light_position", lumieres[1].position);
        shader_illumination->setUniform3f("light_position", lumieres[2].position);
        shader_illumination->setUniform3f("light_position", lumieres[3].position);
      shader_illumination->end();
      break;

    default:
      break;
  }

}



//update du material global dillumination
void Renderer::update_material()
{   
    //ofColor fill_couleur_illumination = ofColor(fill_color_illumination.x, fill_color_illumination.y, fill_color_illumination.z); 
    ofColor couleur_ambiance = ofColor(ambient_color_illumination.x, ambient_color_illumination.y, ambient_color_illumination.z);
    ofColor couleur_diffuse = ofColor(diffuse_color.x, diffuse_color.y, diffuse_color.z);
    ofColor couleur_emissive = ofColor(emissive_color.x, emissive_color.y, emissive_color.z);
    ofColor couleur_speculaire = ofColor(specular_color.x, specular_color.y, specular_color.z);
    material_global.setAmbientColor(couleur_ambiance);
    material_global.setDiffuseColor(couleur_diffuse);
    material_global.setEmissiveColor(couleur_emissive);
    material_global.setSpecularColor(couleur_speculaire);
    material_global.setShininess(shininess);
}

// Section 6.2 Filtrage


/*
* brief : Filtrage bilineaire
*/

void Renderer::bilineaire_application() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  
}

/*
* brief : Filtrage trilineaire
*/

void Renderer::trilineaire_application() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    
}

/*
* brief : Filtrage anisotropique
*/

void Renderer::anisotropique_application() {
    GLfloat largest_supported_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);



}


//Section 6.3
void Renderer::apply_tone_mapping()
{
    
    screen_capture_mapping.grabScreen(0,0, ofGetWidth(),ofGetHeight());
    shader_mapping.begin();
    shader_mapping.setUniformTexture("image", screen_capture_mapping.getTexture(), 1);
    shader_mapping.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
    shader_mapping.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
    shader_mapping.setUniform1f("tone_mapping_toggle", tone_mapping_toggle);
    screen_capture_mapping.update();
    screen_capture_mapping.draw(0,screen_capture_mapping.getWidth(), screen_capture_mapping.getHeight());
    ofLog() << "Drawn with shaders mapping";
    ofLog() << "exposure" << tone_mapping_exposure;
    ofLog() << "gamme" << tone_mapping_gamma;
    ofLog() << "toggle" << tone_mapping_toggle;
    shader_mapping.end();
}


/*
* brief: Cube avec materiaux
*/

void Renderer::drawCube_mat(float x, float y, float z) {
    material_cube.begin();
    ofSetColor(255, 234, 0);
    material_cube.setAmbientColor(ofColor(128, 0, 128)); // Couleur ambiante mauve
    material_cube.setDiffuseColor(ofColor(128, 0, 128)); // Couleur diffuse mauve
    material_cube.setSpecularColor(ofColor(255, 255, 255)); // Couleur spéculaire blanche
    material_cube.setShininess(0.5); // Brillance
    ofDrawBox(x, y, z, default_cube_width, default_cube_height, default_cube_depth);
    material_cube.end();
}



/*
* brief: Sphere avec materiaux
*/

void Renderer::drawSphere_mat(float x, float y, float z) {
    material_sphere.begin();
    ofSetColor(231, 0, 211);
    material_sphere.setAmbientColor(ofColor(255, 165, 0)); // Couleur ambiante orange
    material_sphere.setDiffuseColor(ofColor(255, 165, 0)); // Couleur diffuse orange
    material_sphere.setSpecularColor(ofColor(255, 255, 255)); // Couleur spéculaire blanche
    material_sphere.setShininess(0.5); // Brillance
    ofDrawSphere(x, y, z, default_sphere_radius);
    material_sphere.end();
}



/*
* brief: Prisme avec materiaux
*/

void Renderer::drawPrisme_mat(float x, float y, float z) {
    material_prisme.begin();
    ofSetColor(0, 11, 111);
    material_prisme.setAmbientColor(ofColor(128, 128, 128)); // Couleur ambiante grise
    material_prisme.setDiffuseColor(ofColor(128, 128, 128)); // Couleur diffuse grise
    material_prisme.setSpecularColor(ofColor(255, 255, 255)); // Couleur spéculaire blanche
    material_prisme.setShininess(0.5); // Brillance
    ofDrawBox(x, y, z, default_prisme_width, default_prisme_height, default_prisme_depth);
    material_prisme.end();
}



void Renderer::init_buffer_lumiere()
{
    buffer_lumieres_count = 4;
    buffer_lumieres_head = 0;
    buffer_lumieres_stride = sizeof(VectorLumiere);
    buffer_lumieres_size = buffer_lumieres_count * buffer_lumieres_stride;
    lumiere = (VectorLumiere*)std::malloc(buffer_lumieres_size);
    
}

//ajout des lumieres
void Renderer::add_lumiere()
{
    for (int buffer_lumieres_head = 0; buffer_lumieres_head < buffer_lumieres_count; ++buffer_lumieres_head)
    {
        lumiere[buffer_lumieres_head].type = buffer_lumieres_head;

        if(buffer_lumieres_head == 0) //Luimiere ambiante
        {
            lumiere[buffer_lumieres_head].color[0] = 0.0f; // Couleur blanche
            lumiere[buffer_lumieres_head].color[1] = 0.0f; // Couleur blanche
            lumiere[buffer_lumieres_head].color[2] = 0.0f; // Couleur blanche
        }

        else if (buffer_lumieres_head == 1) // Lumière directionnelle
        {
            lumiere[buffer_lumieres_head].color[0] = 0.0f;
            lumiere[buffer_lumieres_head].color[1] = 0.0f;
            lumiere[buffer_lumieres_head].color[2] = 0.0f;

            lumiere[buffer_lumieres_head].position[0] = 50.0f;
            lumiere[buffer_lumieres_head].position[1] = 0.0f;
            lumiere[buffer_lumieres_head].position[2] = 0.0f;
        }
        else if (buffer_lumieres_head == 2) // Lumière ponctuelle
        {
            // Définir la couleur de la lumière
            lumiere[buffer_lumieres_head].color[0] = 0.0f; // Couleur blanche
            lumiere[buffer_lumieres_head].color[1] = 0.0f;
            lumiere[buffer_lumieres_head].color[2] = 0.0f;

            lumiere[buffer_lumieres_head].position[0] = -50.0f;
            lumiere[buffer_lumieres_head].position[1] = 30.0f;
            lumiere[buffer_lumieres_head].position[2] = 0.0f;
            // Définir d'autres attributs de la lumière
            lumiere[buffer_lumieres_head].attenuation = 1.0f;
            lumiere[buffer_lumieres_head].linearAttenuation = 0.0f;
            lumiere[buffer_lumieres_head].quadraticAttenuation = 0.0f;

        }
        else if (buffer_lumieres_head == 3) // Lumière de type projecteur
        {

            lumiere[buffer_lumieres_head].color[0] = 0.0f; // Couleur blanche
            lumiere[buffer_lumieres_head].color[1] = 0.0f;
            lumiere[buffer_lumieres_head].color[2] = 0.0f;
            lumiere[buffer_lumieres_head].position[0] = 0.0f;
            lumiere[buffer_lumieres_head].position[1] = 50.0f;
            lumiere[buffer_lumieres_head].position[2] = 0.0f;
            // Définir la cible de la lumière
            lumiere[buffer_lumieres_head].target[0] = 0.0f; // Ajustez ces valeurs en fonction de votre scène
            lumiere[buffer_lumieres_head].target[1] = 0.0f;
            lumiere[buffer_lumieres_head].target[2] = 0.0f;

            // Définir d'autres attributs de la lumière
            lumiere[buffer_lumieres_head].attenuation = 0.2f;
            lumiere[buffer_lumieres_head].linearAttenuation = 0.0f;
            lumiere[buffer_lumieres_head].quadraticAttenuation = 0.0f;
        }
        
        ofLog() << "<new lumiere at index: " << buffer_lumieres_head << ">";
        ofLog() << "Lumiere " << buffer_lumieres_head << " couleur : " << lumiere[buffer_lumieres_head].color[0] << " " << lumiere[buffer_lumieres_head].color[1] << " " << lumiere[buffer_lumieres_head].color[2];
        ofLog() << "Lumiere " << buffer_lumieres_head << " position : " << lumiere[buffer_lumieres_head].position[0] << " " << lumiere[buffer_lumieres_head].position[1] << " " << lumiere[buffer_lumieres_head].position[2];
    }
}

void Renderer::update_lumiere()
{
    //gizmo pour chaque lumiere
    for (int i = 0; i < buffer_lumieres_count; ++i) {
        if (lumiere[i].type == 0) {
            ofSetColor(lumiere[i].color[0], lumiere[i].color[1], lumiere[i].color[2]);
            ofDrawSphere(lumiere[i].position[0], lumiere[i].position[1], lumiere[i].position[2], 1);
        }
        else if (lumiere[i].type == 1) {
            ofSetColor(255, 255, 255);
            ofDrawSphere(lumiere[i].position[0], lumiere[i].position[1], lumiere[i].position[2], 1);
        }
        else if (lumiere[i].type == 2) {
            ofSetColor(lumiere[i].color[0], lumiere[i].color[1], lumiere[i].color[2]);
            ofDrawSphere(lumiere[i].position[0], lumiere[i].position[1], lumiere[i].position[2], 1);
        }
        else if (lumiere[i].type == 3) {
            ofSetColor(lumiere[i].color[0], lumiere[i].color[1], lumiere[i].color[2]);
            ofDrawSphere(lumiere[i].position[0], lumiere[i].position[1], lumiere[i].position[2], 1);
        }
    }

    // Activez le shader
    shader_lumiere.begin();

    // Passez les lumières au shader
    for (int i = 0; i < buffer_lumieres_count; ++i) {

        //Constante de couleur
        lumiere[i].color[0] = constant_color.r;
        lumiere[i].color[1] = constant_color.g;
        lumiere[i].color[2] = constant_color.b;

        // Créez des chaînes pour les noms des uniformes
        std::string prefix = "lumiere[" + std::to_string(i) + "].";
        std::string typeUniform = prefix + "type";
        std::string colorUniform = prefix + "color";
        std::string positionUniform = prefix + "position";
        std::string targetUniform = prefix + "target"; // Nouvelle variable pour la cible
        std::string attenuationUniform = prefix + "attenuation";
        std::string linearAttenuationUniform = prefix + "linearAttenuation";
        std::string quadraticAttenuationUniform = prefix + "quadraticAttenuation";

        // Passez les attributs de chaque lumière au shader
        shader_lumiere.setUniform1i(typeUniform.c_str(), lumiere[i].type);
        shader_lumiere.setUniform3f(colorUniform.c_str(), lumiere[i].color[0], lumiere[i].color[1], lumiere[i].color[2]);
        shader_lumiere.setUniform3f(positionUniform.c_str(), lumiere[i].position[0], lumiere[i].position[1], lumiere[i].position[2]);
        shader_lumiere.setUniform3f(targetUniform.c_str(), lumiere[i].target[0], lumiere[i].target[1], lumiere[i].target[2]); // Passer la cible au shader
        shader_lumiere.setUniform1f(attenuationUniform.c_str(), lumiere[i].attenuation);
        shader_lumiere.setUniform1f(linearAttenuationUniform.c_str(), lumiere[i].linearAttenuation);
        shader_lumiere.setUniform1f(quadraticAttenuationUniform.c_str(), lumiere[i].quadraticAttenuation);
        
        // Journalisation pour déboguer
        //ofLog() << "Lumiere " << i << " couleur : " << lumiere[i].color[0] << " " << lumiere[i].color[1] << " " << lumiere[i].color[2];
        //ofLog() << "Lumiere " << i << " position : " << lumiere[i].position[0] << " " << lumiere[i].position[1] << " " << lumiere[i].position[2];
    }

}

int Renderer::get_nbre_lumiere_total()
{
    return buffer_lumieres_count;
}

int Renderer::get_nbre_lumiere_directionnelle()
{
    int nbre_lumiere_directionnelle = 0;
    for (int i = 0; i < buffer_lumieres_count; ++i) {
        if (lumiere[i].type == 0) {
            nbre_lumiere_directionnelle++;
        }
    }
    return nbre_lumiere_directionnelle;
}

int Renderer::get_position_lumiere_x(int index)
{
    
    return lumiere[index].position[0];
}

int Renderer::get_position_lumiere_y(int index)
{
    return lumiere[index].position[1];
}

int Renderer::get_derniere_lumiere_directionnelle()
{
    int derniere_lumiere_directionnelle = -1;
    for (int i = 0; i < buffer_lumieres_count; ++i) {
        if (lumiere[i].type == 0) {
            derniere_lumiere_directionnelle = i;
        }
    }
    return derniere_lumiere_directionnelle;
}

int Renderer::get_nbre_lumiere_ponctuelle()
{
    int nbre_lumiere_ponctuelle = 0;
    for (int i = 0; i < buffer_lumieres_count; ++i) {
        if (lumiere[i].type == 1) {
            nbre_lumiere_ponctuelle++;
        }
    }
    return nbre_lumiere_ponctuelle;
}

int Renderer::get_derniere_lumiere_ponctuelle()
{
    int derniere_lumiere_ponctuelle = -1;
    for (int i = 0; i < buffer_lumieres_count; ++i) {
        if (lumiere[i].type == 1) {
            derniere_lumiere_ponctuelle = i;
        }
    }
    return derniere_lumiere_ponctuelle;
}

int Renderer::get_nb_lumiere_projecteur()
{
    int nb_lumiere_projecteur = 0;
    for (int i = 0; i < buffer_lumieres_count; ++i) {
        if (lumiere[i].type == 2) {
            nb_lumiere_projecteur++;
        }
    }
    return nb_lumiere_projecteur;
}

int Renderer::get_derniere_lumiere_projecteur()
{
    int derniere_lumiere_projecteur = -1;
    for (int i = 0; i < buffer_lumieres_count; ++i) {
        if (lumiere[i].type == 2) {
            derniere_lumiere_projecteur = i;
        }
    }
    return derniere_lumiere_projecteur;
}

void Renderer::texture_init()
{
    // Créer une texture
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Paramètres du material
    ofImage texture;
    

    // Charger l'image
    ofLoadImage(texture, "textures/asuka_sad.jpg");

    // Envoyer l'image à la carte graphique
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.getWidth(), texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getPixels().getData());

    // Paramètres de la texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //doit faire dans draw:
    //glBindTexture(GL_TEXTURE_2D, texture_id);

    // Dessinez l'objet ici
    // ...

    // Désactivez la texture
    //glBindTexture(GL_TEXTURE_2D, 0);

}

//initialisation couleur ambiance
void Renderer::ambiant_color_init()
{
    constant_color.r = 0.0f;
    constant_color.g = 0.0f;
    constant_color.b = 0.0f;

    //ou sphere_material_ambient.setAmbientColor(picker_color); avec un color picker dans application
}

//Ajout des lumieres
void Renderer::ajout_lumiere()
{
      // initialisation des lumieres
  lumieres[0].type = 0; // Lumière ambiante
  lumieres[0].color = ofVec3f(0.5f, 0.5f, 0.5f); // Couleur ambiante

  //lumieres directionnelles 'a la meme position que le gizmo
  lumieres[1].type = 1; // Lumière directionnelle
  lumieres[1].color = ofVec3f(1.0f, 1.0f, 1.0f); // Couleur de la lumière
  lumieres[1].position = ofVec3f(-200.0f, 0.0f, 0.0f); // Position de la lumière
  lumieres[1].target = ofVec3f(1.0f, 0.0f, 0.0f); // Cible de la lumière vers l'axe des x positif

  //lumiere ponctuelle 'a 200,200,0
  lumieres[2].type = 2; // Lumière ponctuelle
  lumieres[2].color = ofVec3f(1.0f, 1.0f, 1.0f); // Couleur de la lumière
  lumieres[2].position = ofVec3f(200.0f, 200.0f, 0.0f); // Position de la lumière
  lumieres[2].target = ofVec3f(0.0f, 0.0f, 0.0f); // Cible de la lumière vers l'origine
  lumieres[2].attenuation = 6.0f; // Atténuation de la lumière
  lumieres[2].linearAttenuation = 7.0f; // Atténuation linéaire de la lumière
  lumieres[2].quadraticAttenuation = 9.0f; // Atténuation quadratique de la lumière
  
  //lumiere spot 'a 0,50,200 pointant vers 0,0,0
  lumieres[3].type = 3; // Lumière spot
  lumieres[3].color = ofVec3f(1.0f, 1.0f, 1.0f); // Couleur de la lumière
  lumieres[3].position = ofVec3f(0.0f, 50.0f, 200.0f); // Position de la lumière
  lumieres[3].target = ofVec3f(0.0f, 0.0f, 0.0f); // Cible de la lumière vers l'origine
  lumieres[3].attenuation = 6.0f; // Atténuation de la lumière
  lumieres[3].linearAttenuation = 7.0f; // Atténuation linéaire de la lumière
  lumieres[3].quadraticAttenuation = 9.0f; // Atténuation quadratique de la lumière
  lumieres[3].spotExponent = 10.0f; // Exposant du cône de lumière
  lumieres[3].spotCutoffAngle = 45.0f; // Coupure du cône de lumière
}

//mise a jour des lumieres
void Renderer::mise_a_jour_lumiere()
{
    ofFloatColor ambient_color = material_global.getAmbientColor();
  ofLog() << "ambient_color: " << ambient_color;
  shader_lighting.setUniform3f("material_ambient", ambient_color.r, ambient_color.g, ambient_color.b);
  shader_lighting.setUniform1i("lumiere[0].type", lumieres[0].type);
  shader_lighting.setUniform3f("lumiere[0].color", lumieres[0].color);

  shader_lighting.setUniform1i("lumiere[1].type", lumieres[1].type);
  shader_lighting.setUniform3f("lumiere[1].color", lumieres[1].color);
  shader_lighting.setUniform3f("lumiere[1].position", lumieres[1].position);
  shader_lighting.setUniform3f("lumiere[1].target", lumieres[1].target);
  
  //Lumiere ponctuelle
  shader_lighting.setUniform1i("lumiere[2].type", lumieres[2].type);
  shader_lighting.setUniform3f("lumiere[2].color", lumieres[2].color);
  shader_lighting.setUniform3f("lumiere[2].position", lumieres[2].position);
  shader_lighting.setUniform3f("lumiere[2].target", lumieres[2].target);
  shader_lighting.setUniform1f("lumiere[2].attenuation", lumieres[2].attenuation);
  shader_lighting.setUniform1f("lumiere[2].linearAttenuation", lumieres[2].linearAttenuation);
  shader_lighting.setUniform1f("lumiere[2].quadraticAttenuation", lumieres[2].quadraticAttenuation);

  //Lumiere spot
  shader_lighting.setUniform1i("lumiere[3].type", lumieres[3].type);
  shader_lighting.setUniform3f("lumiere[3].color", lumieres[3].color);
  shader_lighting.setUniform3f("lumiere[3].position", lumieres[3].position);
  shader_lighting.setUniform3f("lumiere[3].target", lumieres[3].target);
  shader_lighting.setUniform1f("lumiere[3].attenuation", lumieres[3].attenuation);
  shader_lighting.setUniform1f("lumiere[3].linearAttenuation", lumieres[3].linearAttenuation);
  shader_lighting.setUniform1f("lumiere[3].quadraticAttenuation", lumieres[3].quadraticAttenuation);
  shader_lighting.setUniform1f("lumiere[3].spotExponent", lumieres[3].spotExponent);
  shader_lighting.setUniform1f("lumiere[3].spotCutoffAngle", lumieres[3].spotCutoffAngle);
}

/*
* brief: Destructeur Renderer
*/

Renderer::~Renderer()
{
    std::free(shapes);
    std::free(models);
    shader_mapping.unload();
    shader_lumiere.unload();
    std::free(lumiere);

}





