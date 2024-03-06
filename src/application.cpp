#include "application.h"


void Application::setup()
{
    ofSetWindowTitle("Projet_Session");

    ofLog() << "<app::setup>";
    renderer.setup();
    
    gui.setPosition(1, 1);
    gui.setup("Interface"); //(2.5)
    checkbox.setName("Gui visible");

    checkbox = true; // Initialisation de l'indicateur du Guis
    gui.add(checkbox);



    //Groupe du critere 1 Image (1.1)
    group_image.setup("Image");
    //Bouton d'importation d'image
    importImageButton.setup("Importer Image");
    importImageButton.addListener(this, &Application::importImage);
    group_image.add(&importImageButton);
    //Bouton d'exportation d'image (1.2)
    exportation_button.setup("Exportation");
    exportation_button.addListener(this, &Application::exportation_button_pressed);
    group_image.add(&exportation_button);
    //Bouton d'echantillonnage d'image(1.3)
    screenshot_button.setup("Capture d'ecran", false);
    screenshot_button.addListener(this, &Application::screenshot_button_pressed);
    group_image.add(&screenshot_button);
    screenshot_button_funny.setup("Capture surprise", false);
    screenshot_button_funny.addListener(this, &Application::screenshot_funny_button_pressed);
    group_image.add(&screenshot_button_funny);
    captureMode = false; // Initialisation l'indicateur du mode de capture d'écran
    captureMode_funny = false;
    //Espace de couleur (1.4)
    background_rgb_button.setup("Espace RGB");
    background_hsb_button.setup("Espace HSB");
    background_rgb_button.addListener(this, &Application::background_rgb_button_pressed);
    background_hsb_button.addListener(this, &Application::background_hsb_button_pressed);
    color_picker_background.set("couleur du canevas RGB", ofColor(60), ofColor(0, 0), ofColor(255, 255));
    color_picker_background_HSB.set("couleur du canevas HSB", ofColor::fromHsb(60, 0, 255));
    group_image.add(&background_rgb_button);
    group_image.add(&background_hsb_button);
    group_image.add(color_picker_background);
    group_image.add(color_picker_background_HSB);
    //Histogramme ici (1.5)
    group_image.minimize();
    gui.add(&group_image);

    //Groupe du critere 2 Dessin vectoriel 
    group_dessin_vectoriel.setup("Dessin Vectoriel");
    // Ajout des boutons au sous - groupe des curseurs

    cursorDefaultButton.setup("Curseur par defaut");
    cursorDefaultButton.addListener(this, &Application::cursorDefaultButtonPressed);
    group_dessin_vectoriel.add(&cursorDefaultButton);
    cursorDrawLineButton.setup("Curseur de ligne");
    cursorDrawLineButton.addListener(this, &Application::cursorDrawLineButtonPressed);
    group_dessin_vectoriel.add(&cursorDrawLineButton);
    cursorDrawCircleButton.setup("Curseur de cercle ");
    cursorDrawCircleButton.addListener(this, &Application::cursorDrawCircleButtonPressed);
    group_dessin_vectoriel.add(&cursorDrawCircleButton);
    cursorSelectButton.setup("Curseur de selection");
    cursorSelectButton.addListener(this, &Application::cursorSelectButtonPressed);
    group_dessin_vectoriel.add(&cursorSelectButton);
    cursorTranslateButton.setup("Curseur de deplacement");
    cursorTranslateButton.addListener(this, &Application::cursorTranslateButtonPressed);
    group_dessin_vectoriel.add(&cursorTranslateButton);
    cursorRotateButton.setup("Curseur de rotation");
    cursorRotateButton.addListener(this, &Application::cursorRotateButtonPressed);
    group_dessin_vectoriel.add(&cursorRotateButton);





    //Outil de dessin (2.2)

    group_outils_dessin.setup("Outils Dessin");
    group_outils_dessin.add(lineThickness.setup("Epaisseur", 1.0, 0.1, 10.0));
    group_outils_dessin.add(lineColor.setup("Couleur ligne", ofColor(255), ofColor(0), ofColor(255)));
    group_outils_dessin.add(fillColor.setup("Couleur remplissage", ofColor(255), ofColor(0), ofColor(255)));
    group_dessin_vectoriel.add(&group_outils_dessin);
    //Primitives vectorielles (2.3) 
    group_dessin_vectoriel_formes.setup("Formes a dessiner");
    group_dessin_vectoriel.add(&group_dessin_vectoriel_formes);
    //Sous-groupe pour le type d'algo pour dessiner une ligne
    group_dessin_algo_ligne.setup("Algorithme \nde rasterisation");

    // Ajout des boutons pour chaque formes  
    ajout_boutons_formes(); //(Contient 2.4)
    group_dessin_vectoriel.minimize();
    gui.add(&group_dessin_vectoriel);


    //Groupe du critere 3 Transformation
    group_transformation.setup("Transformation");

    //Ajouter 3.1

    
    group_scene_control.setup("Graphe de Scene");
    group_transformation.add(&group_scene_control);

    // Ajouter un élément 
   
    addElementButton.setup("Ajouter Element");
    addElementButton.addListener(this, &Application::addElementPressed);
    group_scene_control.add(&addElementButton);

    // Supprimer un élément 
    
    removeElementButton.setup("Supprimer Element");
    removeElementButton.addListener(this, &Application::removeElementPressed);
    group_scene_control.add(&removeElementButton);

    // Sélectionner un élément 
   
    selectElementButton.setup("Selectionner Element");
    selectElementButton.addListener(this, &Application::selectElementPressed);
    group_scene_control.add(&selectElementButton);
    
    // sélection multiple (3.2)

    groupe_selection_multiple.setup("Selection multiples");
    group_transformation.add(&groupe_selection_multiple);
    selectionButton.addListener(this, &Application::selection_multiple); 
    groupe_selection_multiple.add(&selectionButton); 
    selectionButton.setup("Selection Multiple", false);


    //transformations interactives (3.3)
    groupe_transforamtion_interactive.setup ("Transformations \ninterectives");
    group_transformation.add(&groupe_transforamtion_interactive);
    noneTransformationButton.setup("Aucune");
    noneTransformationButton.addListener(this, &Application::noneTransformationButtonPressed);
    translateButton.setup("Tranlation\n(fleche clavier)");
    translateButton.addListener(this, &Application::translateButtonPressed);
    isTranslationActive = false;
    rotateButton.setup("Rotation\n(fleche <- ->)");
    rotateButton.addListener(this, &Application::rotateButtonPressed);
    isRotatingActive = false;
    scaleButton.setup("Echelle\n(fleche ^ v)");
    scaleButton.addListener(this, &Application::scaleButtonPressed);
    isScalingActive = false;
    groupe_transforamtion_interactive.add(&noneTransformationButton);
    groupe_transforamtion_interactive.add(&translateButton);
    groupe_transforamtion_interactive.add(&rotateButton);
    groupe_transforamtion_interactive.add(&scaleButton);

    //Ajouter 3.4 ici vv

    group_transformation.minimize();
    gui.add(&group_transformation);

    //Ajouter Geometrie ici vv
    groupe_geometrie.setup("Geometrie");
    none_model_button.setup("Mode : Aucun");
    import_model_button.setup("Importer modele");
    predef1_model_button.setup("Modele 1");
    predef2_model_button.setup("Modele 2");
    predef3_model_button.setup("Modele 3");
    predef4_model_button.setup("Modele 4");
    remove_last_model_button.setup("Retirer dernier\n modele");
    none_model_button.addListener(this, &Application::non_model_button_pressed);
    import_model_button.addListener(this, &Application::import_model_button_pressed);
    predef1_model_button.addListener(this, &Application::predef1_model_button_pressed);
    predef2_model_button.addListener(this, &Application::predef2_model_button_pressed);
    predef3_model_button.addListener(this, &Application::predef3_model_button_pressed);
    predef4_model_button.addListener(this, &Application::predef4_model_button_pressed);
    remove_last_model_button.addListener(this,&Application::remove_last_model_button_pressed);
    groupe_geometrie.add(&none_model_button);
    groupe_geometrie.add(&import_model_button);
    groupe_geometrie.add(&predef1_model_button);
    groupe_geometrie.add(&predef2_model_button);
    groupe_geometrie.add(&predef3_model_button);
    groupe_geometrie.add(&predef4_model_button);
    groupe_geometrie.add(&remove_last_model_button);

    // Chargez les modèles
       

    //4.1
    groupe_geometrie.add(drawBoundingBoxButton.setup("Dessiner arrete", false));

    //4.2

    // Configuration du groupe pour "Primitives Géométriques"
    groupe_primitive_geometrie.setup("Primitives \nGeometriques");
    groupe_geometrie.add(&groupe_primitive_geometrie);

    // Configuration et ajout du toggle "Triangle"
    cubeButton.setup("Cube", false);
    groupe_primitive_geometrie.add(&cubeButton);
    cubeButton.addListener(this, &Application::cubeButtonPressed);
    // Configuration et ajout du toggle "Triangle"
    sphereButton.setup("Sphere", false);
    groupe_primitive_geometrie.add(&sphereButton);
    sphereButton.addListener(this, &Application::sphereButtonPressed);
 

    //4.3
    groupe_geometrie.minimize();
    gui.add(&groupe_geometrie);


    // Camera SECTION 5
    groupe_camera.setup("Camera");

    // 5.1 Caméra interactive



    // 5.2 Mode de projection
    mode_projection.setup("Mode de projection");
    groupe_camera.add(&mode_projection);

    
    perspectiveButton.setup("Perspective", false);
    mode_projection.add(&perspectiveButton);
    perspectiveButton.addListener(this, &Application::perspectiveButtonPressed);

    
    orthogonaleButton.setup("Orthogonale", false);
    mode_projection.add(&orthogonaleButton);
    orthogonaleButton.addListener(this, &Application::orthogonaleButtonPressed);

    
    mode_projection.minimize();

    //5.3 Point de vue multiple
    mode_dessincam_button.setup("Mode dessin");
    mode_1cam_button.setup("Mode 1 camera");
    mode_2cam_button.setup("Mode 2 cameras");
    mode_dessincam_button.addListener(this, &Application::mode_dessin_pressed);
    mode_1cam_button.addListener(this, &Application::mode_1cam_pressed);
    mode_2cam_button.addListener(this, &Application::mode_2cam_pressed);
    groupe_camera.add(&mode_dessincam_button);
    groupe_camera.add(&mode_1cam_button);
    groupe_camera.add(&mode_2cam_button);
    //5.4 Occlusion


    groupe_camera.minimize();

    
    gui.add(&groupe_camera);


    //5
    //5.1
    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;
    //5.2
    
    
}


void Application::draw()
{
    
    if (checkbox)
        gui.draw();

    drawCursor();

    // Dessiner l'image importée si elle est chargée
    //if (importedImage.isAllocated()) {
        //importedImage.draw(0, 0); // Ajustez la position et la taille selon vos besoins
    //}
    renderer.draw();
}




void Application::update()
{
    renderer.background_color1 = color_picker_background;
    renderer.background_color2 = color_picker_background_HSB;



    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

        if (is_key_press_left)
        {
            if(isTranslationActive){
                if(renderer.draw_mode != VectorPrimitiveType::none)
                {
                    renderer.translateLastShape(-(renderer.delta_x * time_elapsed), 0);
                }
                if(renderer.draw_mode_models != VectorModelType::none)
                {
                    renderer.translateLastModel(-(renderer.delta_x * time_elapsed), 0, 0);
                }
            }
            else if(isRotatingActive)
            {
                renderer.rotatePrimitive(renderer.speed_delta/2.0 * time_elapsed);
            }
        }
        if (is_key_press_right)
        {
            if(isTranslationActive){
                if(renderer.draw_mode != VectorPrimitiveType::none) 
                {
                    renderer.translateLastShape(renderer.delta_x * time_elapsed, 0);
                }
                
                if(renderer.draw_mode_models != VectorModelType::none)
                {
                    renderer.translateLastModel(renderer.delta_x * time_elapsed, 0, 0);
                }
            }
            else if(isRotatingActive)
            {
                renderer.rotatePrimitive(-renderer.speed_delta/2.0 * time_elapsed);
            }
        }
        if (is_key_press_up)
        {
            if(isTranslationActive){
                if(renderer.draw_mode != VectorPrimitiveType::none) {
                    renderer.translateLastShape(0, -renderer.delta_y * time_elapsed);
                }
                if(renderer.draw_mode_models != VectorModelType::none)
                {
                    renderer.translateLastModel(0, -renderer.delta_y * time_elapsed, 0);
                }
            }
            if(isScalingActive){
                float scaleFactor = 1.01;
                renderer.scalePrimitive(scaleFactor);
            }
        }
        if (is_key_press_down)
        {
            if(isTranslationActive){
                if(renderer.draw_mode != VectorPrimitiveType::none) {
                    renderer.translateLastShape(0, renderer.delta_y * time_elapsed);
                }
                if(renderer.draw_mode_models != VectorModelType::none)
                {
                    renderer.translateLastModel(0, renderer.delta_y * time_elapsed, 0);
                }
            }
            if(isScalingActive){
                float scaleFactor = 0.99;
                renderer.scalePrimitive(scaleFactor);
            }
        }

    if (isExporting && exportCount < 5) {
        float currentTime = ofGetElapsedTimef();
        if (currentTime - lastExportTime >= 2) { // 2 secondes
            ofImage image;
            image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            string fileName = "Exportation" + ofToString(exportCount) + ".png";
            image.save(fileName);
            exportCount++;
            lastExportTime = currentTime; // Mise a jour du temps pour la prochaine exportation

            if (exportCount >= 5) {
                isExporting = false; // Arret de l'exportation après 5 images
            }
        }
    }

    renderer.lineThickness = lineThickness; // Epaisseur
    renderer.lineColor = lineColor; // Contour
    renderer.fillColor = fillColor; // Remplissage

    renderer.update();

}

void Application::importImage() {
    renderer.import_image();
}

void Application::exportation_button_pressed() {
    isExporting = true;
    exportCount = 0;
    lastExportTime = ofGetElapsedTimef();
}

void Application::screenshot_button_pressed(bool& value)
{
    // Activer/désactiver le mode de capture d'écran
    captureMode = value;
    if (captureMode)
    {
        ofLog() << "Mode de capture d'écran activé.";
    }
    else
    {
        ofLog() << "Mode de capture d'écran désactivé.";
    }
}

void Application::screenshot_funny_button_pressed(bool& value)
{
    // Activer/désactiver le mode de capture d'écran
    captureMode_funny = value;
    if (captureMode_funny)
    {
        captureMode = false;
        screenshot_button = false;
        ofLog() << "Mode de capture d'écran surprise activé.";
    }
    else
    {
        ofLog() << "Mode de capture d'écran désactivé.";
    }
}

void Application::background_rgb_button_pressed()
{
    renderer.color_mode = BackgroundColorType::rgb;
}

void Application::background_hsb_button_pressed()
{
    renderer.color_mode = BackgroundColorType::hsb;
}



void Application::mouseMoved(int x, int y)
{
    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;
}

void Application::mousePressed(int x, int y, int button)
{
    renderer.is_mouse_button_pressed = true;

    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;

    renderer.mouse_press_x = x;
    renderer.mouse_press_y = y;

    ofLog() << "<app::mouse pressed  at: (" << x << ", " << y << ")>";



    if (isSelectionModeActive) {
        bool isSelected = false;
        int selectedShapeId = -1;

        // Trouver la forme sous le curseur
        for (int i = 0; i < renderer.buffer_count; i++) {
            if (isInside(x, y, renderer.shapes[i])) {
                isSelected = true;
                selectedShapeId = i;
                break;
            }
        }

        // Sélectionner ou désélectionner la forme
        if (isSelected) {
            if (std::find(renderer.selectedIds.begin(), renderer.selectedIds.end(), selectedShapeId) != renderer.selectedIds.end()) {
                renderer.deselectShape(selectedShapeId);
            }
            else {
                renderer.selectShape(selectedShapeId);
            }
        }
    }
}


void Application::mouseDragged(int x, int y, int button) {
    if (isSelectionModeActive) {
        int dx = x - renderer.mouse_current_x;
        int dy = y - renderer.mouse_current_y;

        for (int i : renderer.selectedIds) {
            renderer.shapes[i].position1[0] += dx;
            renderer.shapes[i].position1[1] += dy;
            renderer.shapes[i].position2[0] += dx;
            renderer.shapes[i].position2[1] += dy;
        }
    }

    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;
}

void Application::mouseReleased(int x, int y, int button)
{
    selection_start.set(renderer.mouse_press_x, renderer.mouse_press_y);
    selection_end.set(renderer.mouse_current_x, renderer.mouse_current_y);
    renderer.is_mouse_button_pressed = false;

    if (captureMode)
    {
        screenshot(x, y, false);
    }
    else if (captureMode_funny)
    {
        screenshot(x, y, true);
    }
    else if (!isSelectionModeActive) // ELEVER LAJOUT DE FORME SI SELECTION MULTIPLE EST ACTIVÉ
    {
        if (renderer.draw_mode != VectorPrimitiveType::none)
        {
            renderer.add_vector_shape(renderer.draw_mode);
        }
        else if (renderer.draw_mode_models != VectorModelType::none)
        {
            renderer.add_vector_models(renderer.draw_mode_models);
        }
    }
    else
    {
        ofLog() << "<Fin de la selection de zone>";
    }
}

void Application::screenshot(int x, int y, bool z)
{
    // Générer un timestamp unique pour créer un nom de fichier différent à chaque capture
    std::string timestamp = ofGetTimestampString("-%Y%m%d-%H%M%S");
    std::string filename = "screenshot" + timestamp + ".png";
    std::string folderPath = ofToDataPath("screenshot/");

    // Construire le chemin complet du fichier dans le répertoire du binaire
    std::string filePath = ofFilePath::join(ofFilePath::getCurrentExeDir(), folderPath + filename);

    // Assurez-vous que le dossier existe
    ofDirectory::createDirectory(folderPath, true, true);

    // Effectuer la capture d'écran de la zone sélectionnée
    int x1 = min(selection_start.x, selection_end.x);
    int y1 = min(selection_start.y, selection_end.y);
    int x2 = max(selection_start.x, selection_end.x);
    int y2 = max(selection_start.y, selection_end.y);

    // Créer une image temporaire pour stocker la zone sélectionnée
    ofImage tempImage;
    tempImage.grabScreen(x1, y1, x2 - x1, y2 - y1);

    // Créer une nouvelle image pour le résultat de l'échantillonnage
    ofImage sampledImage;
    sampledImage.allocate(x2 - x1, y2 - y1, OF_IMAGE_COLOR);

    // Algorithme d'échantillonnage
    for (int i = 0; i < sampledImage.getWidth(); ++i)
    {
        for (int j = 0; j < sampledImage.getHeight(); ++j)
        {
            // Algorithme d'échantillonnage ici
            // Exemple : simplement copier la couleur
            if (!z) //Screenshot normal
            {
                ofColor pixelColor = tempImage.getColor(i, j);
                sampledImage.setColor(i, j, pixelColor);
            }
            else if (z)
            {
                ofColor pixelColor = tempImage.getColor(i, j);
                //Appliquer le filtre multicolor
                int red = pixelColor.r;
                int green = (pixelColor.g + i) % 255; // Modifier la couleur en fonction de la position en X
                int blue = (pixelColor.b + j) % 255;  // Modifier la couleur en fonction de la position en Y

                // Mettre à jour la couleur du pixel dans l'image résultante
                sampledImage.setColor(i, j, ofColor(red, green, blue, pixelColor.a));

            }
        }
    }

    // Mettre à jour les pixels de l'image résultante
    sampledImage.update();

    // Enregistrer l'image résultante avec le nom et l'emplacement fournis par l'utilisateur
    sampledImage.save(filePath);

    ofLog() << "<Screenshot de la zone sélectionnée avec échantillonnage>";

}

void Application::ajout_boutons_formes()
{
    group_dessin_vectoriel_formes.add(none_shape_button.setup("None", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(pixel_shape_button.setup("Pixel", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(point_shape_button.setup("Point", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(line_shape_button.setup("Ligne", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.setDefaultHeight(36); //Changer le grosseur du gui pour le text des boutons
    group_dessin_vectoriel_formes.add(&group_dessin_algo_ligne);
    group_dessin_algo_ligne.add(algo_of_button.setup("Rasterisation par\n openFrameworks", ofParameter<bool>(false)));
    group_dessin_algo_ligne.add(algo_dda_button.setup("Rasterisation par\n DDA", ofParameter<bool>(false)));
    group_dessin_algo_ligne.add(algo_bressenham_button.setup("Rasterisation par\n Bressenham", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(square_shape_button.setup("Carre", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(rectangle_shape_button.setup("Rectangle", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(circle_shape_button.setup("Cercle", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(ellipse_shape_button.setup("Ellipse", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(triangle_shape_button.setup("Triangle", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(face_shape_button.setup("Face", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(maison_shape_button.setup("Maison", ofParameter<bool>(false)));
    groupe_geometrie.add(cubeButton.setup("Cube", ofParameter<bool>(false)));
    groupe_geometrie.add(sphereButton.setup("Sphere", ofParameter<bool>(false)));

    // Ajout des listeners pour chaque bouton
    none_shape_button.addListener(this, &Application::button_none_pressed);
    pixel_shape_button.addListener(this, &Application::button_pixel_pressed);
    point_shape_button.addListener(this, &Application::button_point_pressed);
    line_shape_button.addListener(this, &Application::button_line_pressed);
    algo_of_button.addListener(this, &Application::button_algo_of_pressed);
    algo_dda_button.addListener(this, &Application::button_algo_dda_pressed);
    algo_bressenham_button.addListener(this, &Application::button_algo_bressenham_pressed);
    square_shape_button.addListener(this, &Application::button_square_pressed);
    rectangle_shape_button.addListener(this, &Application::button_rectangle_pressed);
    circle_shape_button.addListener(this, &Application::button_circle_pressed);
    ellipse_shape_button.addListener(this, &Application::button_ellipse_pressed);
    triangle_shape_button.addListener(this, &Application::button_triangle_pressed);
    face_shape_button.addListener(this, &Application::button_face_pressed);
    maison_shape_button.addListener(this, &Application::button_maison_pressed);



}

void Application::retirer_boutons_formes() {
    none_shape_button.removeListener(this, &Application::button_none_pressed);
    pixel_shape_button.removeListener(this, &Application::button_pixel_pressed);
    point_shape_button.removeListener(this, &Application::button_point_pressed);
    line_shape_button.removeListener(this, &Application::button_line_pressed);
    algo_dda_button.removeListener(this, &Application::button_algo_dda_pressed);
    algo_bressenham_button.removeListener(this, &Application::button_algo_bressenham_pressed);
    square_shape_button.removeListener(this, &Application::button_square_pressed);
    rectangle_shape_button.removeListener(this, &Application::button_rectangle_pressed);
    circle_shape_button.removeListener(this, &Application::button_circle_pressed);
    ellipse_shape_button.removeListener(this, &Application::button_ellipse_pressed);
    triangle_shape_button.removeListener(this, &Application::button_triangle_pressed);
    face_shape_button.removeListener(this, &Application::button_face_pressed);
    maison_shape_button.removeListener(this, &Application::button_maison_pressed);
    cubeButton.removeListener(this, &Application::cubeButtonPressed);
    sphereButton.removeListener(this, &Application::sphereButtonPressed);
}

void Application::button_none_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode = VectorPrimitiveType::none;
        renderer.draw_mode_models = VectorModelType::none;
        isTranslationActive = false;
        ofLog() << "<mode: none>";
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_pixel_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode = VectorPrimitiveType::pixel;
        ofLog() << "<mode: pixel>";
        none_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_point_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode = VectorPrimitiveType::point;
        ofLog() << "<mode: point>";
        none_shape_button = false;
        pixel_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_line_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode = VectorPrimitiveType::line;
        
       
        ofLog() << "<mode: line>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_algo_of_pressed(bool& pressed) {
    if (pressed) {
        renderer.draw_mode = VectorPrimitiveType::line;
        renderer.setLineRenderer(LineRenderer::none);
        algo_of_button = true;
        algo_dda_button = false;
        algo_bressenham_button = false;
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = true;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        ofLog() << "<mode: line of>";
    }

}

void Application::button_algo_dda_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode = VectorPrimitiveType::line;
        renderer.setLineRenderer(LineRenderer::dda);
        algo_of_button = false;
        algo_dda_button = true;
        algo_bressenham_button = false;
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = true;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        ofLog() << "<mode: line dda>";
    }
}

void Application::button_algo_bressenham_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode = VectorPrimitiveType::line;
        renderer.setLineRenderer(LineRenderer::bresenham);
        algo_of_button = false;
        algo_dda_button = false;
        algo_bressenham_button = true;
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = true;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        ofLog() << "<mode: line bressenham>";
    }
}

void Application::button_square_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode_models = VectorModelType::none;
        renderer.draw_mode = VectorPrimitiveType::square;
        ofLog() << "<mode: square>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_rectangle_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode_models = VectorModelType::none;
        renderer.draw_mode = VectorPrimitiveType::rectangle;
        ofLog() << "<mode: rectangle>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_circle_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode_models = VectorModelType::none;
        renderer.draw_mode = VectorPrimitiveType::circle;
        ofLog() << "<mode: circle>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_ellipse_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode_models = VectorModelType::none;
        renderer.draw_mode = VectorPrimitiveType::ellipse;
        ofLog() << "<mode: ellipse>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_triangle_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode_models = VectorModelType::none;
        renderer.draw_mode = VectorPrimitiveType::triangle;
        ofLog() << "<mode: triangle>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        maison_shape_button = false;
        face_shape_button = false;
    }
}

void Application::button_face_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode_models = VectorModelType::none;
        renderer.draw_mode = VectorPrimitiveType::face;
        ofLog() << "<mode: face>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        maison_shape_button = false;
    }

}

void Application::button_maison_pressed(bool& pressed)
{
    if (pressed) {
        renderer.draw_mode_models = VectorModelType::none;
        renderer.draw_mode = VectorPrimitiveType::maison;
        ofLog() << "<mode: maison>";
        none_shape_button = false;
        pixel_shape_button = false;
        point_shape_button = false;
        line_shape_button = false;
        square_shape_button = false;
        rectangle_shape_button = false;
        circle_shape_button = false;
        ellipse_shape_button = false;
        triangle_shape_button = false;
        face_shape_button = false;
    }
}

// Sélection multiple 3.2 a revoir

void Application::selection_multiple(bool& pressed) {
    isSelectionModeActive = pressed;
    ofLog() << "<mode: selection multiple> " << (pressed ? "active" : "desactive");

    none_shape_button = false;
    pixel_shape_button = false;
    point_shape_button = false;
    line_shape_button = false;
    square_shape_button = false;
    rectangle_shape_button = false;
    circle_shape_button = false;
    ellipse_shape_button = false;
    triangle_shape_button = false;
    face_shape_button = false;
    maison_shape_button = false;
    cubeButton = false;
    sphereButton = false;
}

bool Application::isInside(int x, int y, const VectorPrimitive& shape) {
    if (shape.type == VectorPrimitiveType::square) {
        float squareX = std::min(shape.position1[0], shape.position2[0]);
        float squareY = std::min(shape.position1[1], shape.position2[1]);
        float squareSize = std::min(abs(shape.position2[0] - shape.position1[0]), abs(shape.position2[1] - shape.position1[1]));
        return x >= squareX && x <= squareX + squareSize && y >= squareY && y <= squareY + squareSize;
    }
    else if (shape.type == VectorPrimitiveType::rectangle) {
        float left = std::min(shape.position1[0], shape.position2[0]);
        float right = std::max(shape.position1[0], shape.position2[0]);
        float top = std::min(shape.position1[1], shape.position2[1]);
        float bottom = std::max(shape.position1[1], shape.position2[1]);
        return x >= left && x <= right && y >= top && y <= bottom;
    }
    else if (shape.type == VectorPrimitiveType::circle) {
        float centerX = (shape.position1[0] + shape.position2[0]) / 2;
        float centerY = (shape.position1[1] + shape.position2[1]) / 2;
        float radius = ofDist(shape.position1[0], shape.position1[1], shape.position2[0], shape.position2[1]) / 2;
        return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius;
    }
    else if (shape.type == VectorPrimitiveType::ellipse) {
        float centerX = (shape.position1[0] + shape.position2[0]) / 2;
        float centerY = (shape.position1[1] + shape.position2[1]) / 2;
        float radiusX = abs(shape.position2[0] - shape.position1[0]) / 2;
        float radiusY = abs(shape.position2[1] - shape.position1[1]) / 2;
        return (pow(x - centerX, 2) / pow(radiusX, 2)) + (pow(y - centerY, 2) / pow(radiusY, 2)) <= 1;
    }
    else if (shape.type == VectorPrimitiveType::face) {
     
        float centerX = (shape.position1[0] + shape.position2[0]) / 2;
        float centerY = (shape.position1[1] + shape.position2[1]) / 2;
        float radius = ofDist(shape.position1[0], shape.position1[1], shape.position2[0], shape.position2[1]) / 2;
        return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius;
    }
    else if (shape.type == VectorPrimitiveType::maison) {
      
        float rectLeft = shape.position1[0];
        float rectTop = shape.position1[1] + (shape.position2[1] - shape.position1[1]) / 3;
        float rectWidth = shape.position2[0] - shape.position1[0];
        float rectHeight = 2 * (shape.position2[1] - shape.position1[1]) / 3;
        bool inRectangle = x >= rectLeft && x <= rectLeft + rectWidth && y >= rectTop && y <= rectTop + rectHeight;
    
        float x1 = shape.position1[0];
        float y1 = shape.position1[1] + (shape.position2[1] - shape.position1[1]) / 3;
        float x2 = shape.position2[0];
        float y2 = rectTop;
        float x3 = (shape.position1[0] + shape.position2[0]) / 2;
        float y3 = shape.position1[1];
        float areaTotal = abs((x1 * (y1 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
        float area1 = abs((x * (y1 - y3) + x2 * (y3 - y) + x3 * (y - y1)) / 2.0);
        float area2 = abs((x1 * (y - y3) + x * (y3 - y1) + x3 * (y1 - y)) / 2.0);
        float area3 = abs((x1 * (y1 - y) + x2 * (y - y1) + x * (y1 - y2)) / 2.0);
        bool inTriangle = areaTotal == area1 + area2 + area3;
        return inRectangle || inTriangle;
    }
    else if (shape.type == VectorPrimitiveType::cube) {
        
        float rectLeft = shape.position1[0] - 50;
        float rectTop = shape.position1[1] - 50;
        float rectRight = rectLeft + 100;
        float rectBottom = rectTop + 100; 

        return x >= rectLeft && x <= rectRight && y >= rectTop && y <= rectBottom;
    }
    else if (shape.type == VectorPrimitiveType::sphere) {
        
        float centerX = shape.position1[0];
        float centerY = shape.position1[1];
        float radius = 50;

        return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius;
    }

    return false;
}


void Application::noneTransformationButtonPressed(){
    isTranslationActive = false;
    isRotatingActive = false;
    isScalingActive = false;
    ofLog() << "<mode: transformation : none>" << isTranslationActive;
}

void Application::translateButtonPressed() 
{

        isTranslationActive = !isTranslationActive;
        isRotatingActive = false;
        isScalingActive = false;
        ofLog() << "<mode: translation>" << isTranslationActive;
}

void Application::rotateButtonPressed() 
{
        isTranslationActive = false;
        isRotatingActive = !isRotatingActive;
        isScalingActive = false;
        ofLog() << "<mode: rotation>" << isRotatingActive ;
}

void Application::scaleButtonPressed() 
{
        isTranslationActive = false;
        isRotatingActive = false;
        isScalingActive = !isScalingActive;
        ofLog() << "<mode: scale>" << isScalingActive;
}

//4.2 Primitive geometriques
void Application::cubeButtonPressed(bool& pressed) {
    {
        if (pressed) {
            renderer.draw_mode = VectorPrimitiveType::cube;
            ofLog() << "<mode: cube>";
            none_shape_button = false;
            pixel_shape_button = false;
            point_shape_button = false;
            line_shape_button = false;
            square_shape_button = false;
            rectangle_shape_button = false;
            circle_shape_button = false;
            ellipse_shape_button = false;
            triangle_shape_button = false;
            face_shape_button = false;
            sphereButton = false;
        }
    }
}

void Application::sphereButtonPressed(bool& pressed) {
    {
        if (pressed) {
            renderer.draw_mode = VectorPrimitiveType::sphere;
            ofLog() << "<mode: sphere>";
            none_shape_button = false;
            pixel_shape_button = false;
            point_shape_button = false;
            line_shape_button = false;
            square_shape_button = false;
            rectangle_shape_button = false;
            circle_shape_button = false;
            ellipse_shape_button = false;
            triangle_shape_button = false;
            face_shape_button = false;
            cubeButton = false;
        }
    }
}

void Application::perspectiveButtonPressed() {
    renderer.is_camera_perspective = true;
    renderer.setup_camera();
}

void Application::orthogonaleButtonPressed() {
    renderer.is_camera_perspective = false;
    renderer.setup_camera();
}



void Application::drawCursor() {
    // Obtenir la position actuelle de la souris
    int mouseX = ofGetMouseX();
    int mouseY = ofGetMouseY();

    switch (currentCursorState) {
    case CURSOR_DEFAULT:
        // Dessin du curseur par défaut ou ne rien faire pour utiliser le curseur système
        break;
    case CURSOR_DRAW_LINE:
        // Dessiner une représentation visuelle pour le dessin de ligne
        ofDrawLine(mouseX - 10, mouseY, mouseX + 10, mouseY);
        ofDrawLine(mouseX, mouseY - 10, mouseX, mouseY + 10);
        break;
    case CURSOR_DRAW_CIRCLE:
        // Dessiner une représentation visuelle pour le dessin de cercle
        ofDrawCircle(mouseX, mouseY, 5);
        break;
    case CURSOR_SELECT:
        // Dessiner une représentation visuelle pour le mode sélection
        ofNoFill();
        ofDrawRectangle(mouseX - 5, mouseY - 5, 10, 10);
        break;
    case CURSOR_TRANSLATE:
        // Dessiner une représentation visuelle pour le mode translation
        ofDrawArrow(ofPoint(mouseX, mouseY), ofPoint(mouseX + 10, mouseY), 4);
        break;
    case CURSOR_ROTATE:
        // Dessiner une représentation visuelle pour le mode rotation
        ofDrawCircle(mouseX, mouseY, 5);
        ofDrawLine(mouseX, mouseY, mouseX + 10, mouseY);
        ofDrawBitmapString("R", mouseX + 12, mouseY + 3);
        break;
    default:
        break;
    }
}

void Application::non_model_button_pressed(){
    renderer.draw_mode = VectorPrimitiveType::none;
    renderer.draw_mode_models = VectorModelType::none;
}

void Application::import_model_button_pressed(){
    ofFileDialogResult result = ofSystemLoadDialog("Select a folder", true);
    ofLogNotice() << "Dialog result: " << result.bSuccess;
    if (result.bSuccess)
    {
        // Retrieve the selected folder path
        string folderPath = result.filePath;

        // Use the selected folder path as needed
        ofLogNotice() << "Selected folder: " << folderPath;
    }
    else
    {
        // User canceled the dialog
        ofLogNotice() << "Dialog canceled by the user";
    }
}
void Application::predef1_model_button_pressed(){
        renderer.draw_mode = VectorPrimitiveType::none;
        renderer.draw_mode_models = VectorModelType::predef1;
}
void Application::predef2_model_button_pressed(){
        renderer.draw_mode = VectorPrimitiveType::none;
        renderer.draw_mode_models = VectorModelType::predef2;
}
void Application::predef3_model_button_pressed(){

        renderer.draw_mode = VectorPrimitiveType::none;
        renderer.draw_mode_models = VectorModelType::predef3;
}
void Application::predef4_model_button_pressed() { 
    renderer.draw_mode = VectorPrimitiveType::none;
    renderer.draw_mode_models = VectorModelType::predef4;
}

//Fonction pour retirer le dernier modele affiche
void Application::remove_last_model_button_pressed(){
}

void Application::keyPressed(int key)
{
  switch (key)
  {
    case OF_KEY_LEFT: // key ←
      is_key_press_left = true;
      break;

    case OF_KEY_UP: // key ↑
      is_key_press_up = true;
      break;

    case OF_KEY_RIGHT: // key →
      is_key_press_right = true;
      break;

    case OF_KEY_DOWN: // key ↓
      is_key_press_down = true;
      break;

    case 'z':
        //position_.undo();
        break;

    case 'x':
        //position_.redo();
        break;

    default:
      break;
  }
}

void Application::keyReleased(int key)
{
    switch (key)
    {
    case 117: //key u
        checkbox = !checkbox;
        ofLog() << "<toggle ui: " << checkbox << ">";
        break;
    case 114: // key r
        renderer.reset();
        break;
    case OF_KEY_LEFT: // key ←
        is_key_press_left = false;
      
      break;

    case OF_KEY_UP: // key ↑
        is_key_press_up = false;
      break;

    case OF_KEY_RIGHT: // key →
        is_key_press_right = false;
      break;

    case OF_KEY_DOWN: // key ↓
        is_key_press_down = false;
      break;

    case 49: //Key numpad1
        renderer.camera_active = Camera::devant;
        renderer.setup_camera();
        ofLog() << "<app::cam 1";
    break;

    case 50: //Key numpad2
        renderer.camera_active = Camera::derriere;
        renderer.setup_camera();
        ofLog() << "<app::cam 2>";
    break;

    case 51: //Key numpad2
        renderer.camera_active = Camera::gauche;
        renderer.setup_camera();
    break;

    case 52: //Key numpad4
        renderer.camera_active = Camera::droite;
        renderer.setup_camera();
    break;

    case 53: //Key numpad5
        renderer.camera_active = Camera::dessus;
        renderer.setup_camera();
    break;

    case 54: //Key numpad5
        renderer.camera_active = Camera::dessous;
        renderer.setup_camera();
    break;

    case 'p': // key p 
        renderer.is_camera_perspective = true;
        renderer.setup_camera();
        ofLog() << "<perpective projection>";
        break;

    case 'o': // key o
        renderer.is_camera_perspective = false;
        renderer.setup_camera();
        ofLog() << "<orthographic projection>";
        break;


    default:
        ofLog() << "<app::key num>" << key;
    break;
    }
}



void Application::cursorDefaultButtonPressed() {
    
        currentCursorState = CURSOR_DEFAULT;
    
}

void Application::cursorDrawLineButtonPressed() {
    
        currentCursorState = CURSOR_DRAW_LINE;
    
}


void Application::cursorDrawCircleButtonPressed() {
   
        currentCursorState = CURSOR_DRAW_CIRCLE;
    
}

void Application::cursorSelectButtonPressed() {
    
        currentCursorState = CURSOR_SELECT;
    
}

void Application::cursorTranslateButtonPressed() {
   
        currentCursorState = CURSOR_TRANSLATE;
    
}

void Application::cursorRotateButtonPressed() {
    
        currentCursorState = CURSOR_ROTATE;
    
}

void Application::addElementPressed() {

    renderer.add_vector_shape(VectorPrimitiveType::line);

    renderer.add_vector_models(VectorModelType::predef1);
}




void Application::removeElementPressed() {

    if (selectedShapeIndex != -1) {
        renderer.remove_vector_shape(selectedShapeIndex);
        selectedShapeIndex = -1;
    }

    if (selectedModelIndex != -1) {
        renderer.remove_vector_model(selectedModelIndex);
        selectedModelIndex = -1;
    }
}


void Application::selectElementPressed() {

    static bool selectPrimitiveNext = true;

    if (selectPrimitiveNext) {
        selectedShapeIndex = (selectedShapeIndex + 1) % renderer.getBufferHead();
        renderer.select_vector_shape(selectedShapeIndex);
        ofLog() << "Primitive sélectionnée à l'index: " << selectedShapeIndex;
    }
    else {

        selectedModelIndex = (selectedModelIndex + 1) % renderer.getBufferModelHead();
        renderer.select_vector_model(selectedModelIndex);
        ofLog() << "Modèle sélectionné à l'index: " << selectedModelIndex;
    }

    selectPrimitiveNext = !selectPrimitiveNext;
}



void Application::mode_dessin_pressed()
{
    renderer.mode_vue = Mode_Vue::dessin;
}

void Application::mode_1cam_pressed()
{
    renderer.mode_vue = Mode_Vue::camera_3d;
    //renderer.mode_cam = false;
}
void Application::mode_2cam_pressed()
{
    renderer.mode_vue = Mode_Vue::double_cam;
    //renderer.mode_cam = true;
}

void Application::exit()
{
    // Remove the listener for toggle button valueChanged event
    screenshot_button.removeListener(this, &Application::screenshot_button_pressed);
    screenshot_button_funny.removeListener(this, &Application::screenshot_funny_button_pressed);
    background_rgb_button.removeListener(this, &Application::background_rgb_button_pressed);
    background_hsb_button.removeListener(this, &Application::background_hsb_button_pressed);

    retirer_boutons_formes();
    translateButton.removeListener(this, &Application::translateButtonPressed);
    rotateButton.removeListener(this, &Application::rotateButtonPressed);
    scaleButton.removeListener(this, &Application::scaleButtonPressed);

    cursorDefaultButton.removeListener(this, &Application::cursorDefaultButtonPressed);
    cursorDrawLineButton.removeListener(this, &Application::cursorDrawLineButtonPressed);
    cursorDrawCircleButton.removeListener(this, &Application::cursorDrawCircleButtonPressed);
    cursorSelectButton.removeListener(this, &Application::cursorSelectButtonPressed);
    noneTransformationButton.removeListener(this, &Application::noneTransformationButtonPressed);
    cursorTranslateButton.removeListener(this, &Application::cursorTranslateButtonPressed);
    cursorRotateButton.removeListener(this, &Application::cursorRotateButtonPressed);
    
    none_model_button.removeListener(this, &Application::non_model_button_pressed);
    import_model_button.removeListener(this, &Application::import_model_button_pressed);
    predef1_model_button.removeListener(this, &Application::predef1_model_button_pressed);
    predef2_model_button.removeListener(this, &Application::predef2_model_button_pressed);
    predef3_model_button.removeListener(this, &Application::predef3_model_button_pressed);
    predef4_model_button.removeListener(this, &Application::predef4_model_button_pressed);
    remove_last_model_button.removeListener(this, &Application::remove_last_model_button_pressed);

    addElementButton.removeListener(this, &Application::addElementPressed);
    removeElementButton.removeListener(this, &Application::removeElementPressed);
    selectElementButton.removeListener(this, &Application::selectElementPressed);

    mode_dessincam_button.removeListener(this, &Application::mode_dessin_pressed);
    mode_1cam_button.removeListener(this, &Application::mode_1cam_pressed);
    mode_2cam_button.removeListener(this, &Application::mode_2cam_pressed);

    ofLog() << "<app::exit>";

}
