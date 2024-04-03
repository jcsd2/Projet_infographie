#include "application.h"

/*
 * brief: Configuration inital de l'application.
 */


void Application::setup()
{
    ofSetWindowTitle("Projet_Session");

    ofLog() << "<app::setup>";
    renderer.setup();

    gui.setPosition(1, 1);
    gui.setup("Interface"); // (Section 2.5)
    checkbox.setName("Gui visible");

    checkbox = true; // Initialisation de l'indicateur du GUI
    gui.add(checkbox);

    // Groupe du critere 1 : Image

    group_image.setup("Image");
    // Bouton d'importation d'image(1.1)
    importImageButton.setup("Importer Image");
    importImageButton.addListener(this, &Application::importImage);
    group_image.add(&importImageButton);
    // Bouton d'exportation d'image (1.2)
    exportation_button.setup("Exportation");
    exportation_button.addListener(this, &Application::exportation_button_pressed);
    group_image.add(&exportation_button);
    // Bouton d'echantillonnage d'image(1.3)
    screenshot_button.setup("Capture d'ecran", false);
    screenshot_button.addListener(this, &Application::screenshot_button_pressed);
    group_image.add(&screenshot_button);
    screenshot_button_funny.setup("Capture surprise", false);
    screenshot_button_funny.addListener(this, &Application::screenshot_funny_button_pressed);
    group_image.add(&screenshot_button_funny);
    captureMode = false;
    captureMode_funny = false;
    // Espace de couleur (1.4)
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
    // Histogramme (1.5)
    histogramme_button.setup("Afficher histogramme");
    histogramme_button.addListener(this, &Application::histogramme_button_pressed);
    group_image.add(&histogramme_button);
    //Gui
    group_image.minimize();
    gui.add(&group_image);


    // Groupe du critere 2 Dessin vectoriel 
    group_dessin_vectoriel.setup("Dessin Vectoriel");

    // Curseur dynamique (2.1)
    group_curseurs.setup("Curseurs");
    cursorDefaultButton.setup("Curseur par defaut");
    cursorDefaultButton.addListener(this, &Application::cursorDefaultButtonPressed);
    group_curseurs.add(&cursorDefaultButton);
    cursorDrawLineButton.setup("Curseur de ligne");
    cursorDrawLineButton.addListener(this, &Application::cursorDrawLineButtonPressed);
    group_curseurs.add(&cursorDrawLineButton);
    cursorDrawCircleButton.setup("Curseur de cercle ");
    cursorDrawCircleButton.addListener(this, &Application::cursorDrawCircleButtonPressed);
    group_curseurs.add(&cursorDrawCircleButton);
    cursorSelectButton.setup("Curseur de selection");
    cursorSelectButton.addListener(this, &Application::cursorSelectButtonPressed);
    group_curseurs.add(&cursorSelectButton);
    cursorTranslateButton.setup("Curseur de deplacement");
    cursorTranslateButton.addListener(this, &Application::cursorTranslateButtonPressed);
    group_curseurs.add(&cursorTranslateButton);
    cursorRotateButton.setup("Curseur de rotation");
    cursorRotateButton.addListener(this, &Application::cursorRotateButtonPressed);
    group_curseurs.add(&cursorRotateButton);
    cursor = CursorState::CURSOR_DEFAULT;
    group_dessin_vectoriel.add(&group_curseurs);
    
    // Outil de dessin (2.2)
    group_outils_dessin.setup("Outils Dessin");
    group_outils_dessin.add(lineThickness.setup("Epaisseur", 1.0, 0.1, 10.0));
    group_outils_dessin.add(lineColor.setup("Couleur ligne", ofColor(255), ofColor(0), ofColor(255)));
    group_outils_dessin.add(fillColor.setup("Couleur remplissage", ofColor(255), ofColor(0), ofColor(255)));
    group_dessin_vectoriel.add(&group_outils_dessin);
    //Primitives vectorielles (2.3) 
    group_dessin_vectoriel_formes.setup("Formes a dessiner");
    group_dessin_vectoriel.add(&group_dessin_vectoriel_formes);
    group_dessin_algo_ligne.setup("Algorithme \nde rasterisation");
    // Formes vectorielles (2.4) (Sous-groupe) 
    ajout_boutons_formes();
    group_dessin_vectoriel.minimizeAll();
    group_dessin_vectoriel.minimize();
    //2.5
    gui.add(&group_dessin_vectoriel);

    //Groupe du critere 3 Transformation
    group_transformation.setup("Transformation");

    // Graphe de scene (3.1)
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
    selectedShapeIndex = -1;
    selectedModelIndex = -1;

    // Selection multiple (3.2)
    groupe_selection_multiple.setup("Selection multiples");
    group_transformation.add(&groupe_selection_multiple);
    selectionButton.addListener(this, &Application::selection_multiple);
    groupe_selection_multiple.add(&selectionButton);
    selectionButton.setup("Selection Multiple", false);

    // Transformations interactives (3.3)
    groupe_transforamtion_interactive.setup("Transformations \ninterectives");
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

    // Historique de transformation (3.4)
    historique_group.setup("Historique\n Undo/Redo");
    undo_button.setup("Undo");
    redo_button.setup("Redo");
    undo_button.addListener(this, &Application::undo_button_pressed);
    redo_button.addListener(this, &Application::redo_button_pressed);
    historique_group.add(&undo_button);
    historique_group.add(&redo_button);
    group_transformation.add(&historique_group);

    //Systeme de coordonnees 3.5

    //Gui Section 3
    group_transformation.minimize();
    gui.add(&group_transformation);

    // Groupe du critere 4 Geometrie
    groupe_geometrie.setup("Geometrie");

    // Boîte de délimitation (4.1)
    //DrawZone in renderer

    // Primitives géométriques (4.2)
    groupe_primitive_geometrie.setup("Primitives \nGeometriques");
    groupe_geometrie.add(&groupe_primitive_geometrie);
    // Primitive Cube 3D
    cubeButton.setup("Cube", false);
    groupe_primitive_geometrie.add(&cubeButton);
    cubeButton.addListener(this, &Application::cubeButtonPressed);
    // Primitive Sphere 3D
    sphereButton.setup("Sphere", false);
    groupe_primitive_geometrie.add(&sphereButton);
    sphereButton.addListener(this, &Application::sphereButtonPressed);

    // Modele 3D (4.3)
    none_model_button.setup("Mode : Aucun");
    import_model_button.setup("Importer modele");
    predef1_model_button.setup("Modele 1");
    predef2_model_button.setup("Modele 2");
    predef3_model_button.setup("Modele 3");
    remove_last_model_button.setup("Retirer dernier\n modele");
    none_model_button.addListener(this, &Application::non_model_button_pressed);
    import_model_button.addListener(this, &Application::import_model_button_pressed);
    predef1_model_button.addListener(this, &Application::predef1_model_button_pressed);
    predef2_model_button.addListener(this, &Application::predef2_model_button_pressed);
    predef3_model_button.addListener(this, &Application::predef3_model_button_pressed);
    remove_last_model_button.addListener(this, &Application::remove_last_model_button_pressed);
    groupe_geometrie.add(&none_model_button);
    groupe_geometrie.add(&import_model_button);
    groupe_geometrie.add(&predef1_model_button);
    groupe_geometrie.add(&predef2_model_button);
    groupe_geometrie.add(&predef3_model_button);
    groupe_geometrie.add(&remove_last_model_button);
    // Animation (4.4)
    geometrie_animation.setup("Animation");
    animation_button.setup("Activer animation");
    animation_button.addListener(this, &Application::animation_button_pressed);
    animation_svg_object_active = false;
    geometrie_animation.add(&animation_button);
    groupe_geometrie.add(&geometrie_animation);
    // Instanciation (4.5)
    instanciation_button.setup("Instanciation\n du cube");
    instanciation_button.addListener(this, &Application::instanciation_button_pressed);
    groupe_geometrie.add(&instanciation_button);
    //Gui section 4
    groupe_geometrie.minimize();
    gui.add(&groupe_geometrie);

    // Groupe du critere 5 Camera
    groupe_camera.setup("Camera");

    // Caméra interactive (5.1)
    camera_interactive_button.setup("Camera interactive");
    groupe_camera.add(&camera_interactive_button);
    camera_interactive_button.addListener(this, &Application::toggleCameraInteractive);
    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;
    is_key_home = false;
    is_key_end = false;
    is_key_ins = false;
    is_key_del = false;
    is_key_pgup = false;
    is_key_pgdown = false;
    // Modes de projection (5.2)
    mode_projection.setup("Mode de projection");
    groupe_camera.add(&mode_projection);
    perspectiveButton.setup("Perspective", false);
    mode_projection.add(&perspectiveButton);
    perspectiveButton.addListener(this, &Application::perspectiveButtonPressed);
    orthogonaleButton.setup("Orthogonale", false);
    mode_projection.add(&orthogonaleButton);
    orthogonaleButton.addListener(this, &Application::orthogonaleButtonPressed);
    mode_projection.minimizeAll();
    // Point de vue multiple (5.3) et 5.5
    mode_dessincam_button.setup("Mode dessin");
    mode_1cam_button.setup("Mode 1 camera");
    mode_2cam_button.setup("Mode 2 cameras");
    mode_dessincam_button.addListener(this, &Application::mode_dessin_pressed);
    mode_1cam_button.addListener(this, &Application::mode_1cam_pressed);
    mode_2cam_button.addListener(this, &Application::mode_2cam_pressed);
    groupe_camera.add(&mode_dessincam_button);
    groupe_camera.add(&mode_1cam_button);
    groupe_camera.add(&mode_2cam_button);
    // Occlusion (5.4)
    meshfilled_button.setup("Mode meshfilled");
    wireframe_button.setup("Mode wireframe");
    meshfilled_button.addListener(this, &Application::meshfilled_button_pressed);
    wireframe_button.addListener(this, &Application::wireframe_button_pressed);
    groupe_camera.add(&meshfilled_button);
    groupe_camera.add(&wireframe_button);
    //Gui 5
 
    groupe_camera.minimize();
    gui.add(&groupe_camera);
   

    //Section 6 commence ici
    groupe_texture.setup("Texture");
    groupe_texture.setBorderColor(ofColor::cornflowerBlue);
    //Coordonnes de texture 6.1
    groupe_coordonnes.setup("Coordonnes");
    groupe_coordonnes.setBorderColor(ofColor(255, 165, 0));
    groupe_texture.add(&groupe_coordonnes);


    //Filtrage 6.2
    groupe_filtrage.setup("Filtrage");
    groupe_filtrage.setBorderColor(ofColor(255, 165, 0));

  
    filtrage_bilineaire_button.setup("Bilinéaire");
    filtrage_bilineaire_button.addListener(this, &Application::bilineaireButtonPressed);
    groupe_filtrage.add(&filtrage_bilineaire_button);


    filtrage_trilineaire_button.setup("Trilinéaire");
    filtrage_trilineaire_button.addListener(this, &Application::trilineaireButtonPressed);
    groupe_filtrage.add(&filtrage_trilineaire_button);

  
    filtrage_anisotropique_button.setup("Anisotropique");
    filtrage_anisotropique_button.addListener(this, &Application::anisotropiqueButtonPressed);
    groupe_filtrage.add(&filtrage_anisotropique_button);
    groupe_filtrage.minimize();
    groupe_texture.add(&groupe_filtrage);
    

    //Mappage 6.3
    groupe_mappage_tonal.setup("Mappage tonal");
    groupe_mappage_tonal.setBorderColor(ofColor::chartreuse);
    reset_slider_button.setup("Reset sliders");
    reset_slider_button.addListener(this, &Application::reset_mapping_slidder);
    slider_exposure.set("Exposition",renderer.tone_mapping_exposure,0.0f, 5.0f);
    slider_gamma.set("Gamma", renderer.tone_mapping_gamma, 0.0f, 5.0f);
    default_mapping_button.setup("Default mapping");
    default_mapping_button.addListener(this, &Application::default_mapping);
    mapping_Active = false;
    if(renderer.tone_mapping_toggle)
    {toggle_tone_mapping.set("ACES", true);} 
    else {toggle_tone_mapping.set("Reinhard", false);}
    groupe_mappage_tonal.add(&reset_slider_button);
    groupe_mappage_tonal.add(slider_exposure);
    groupe_mappage_tonal.add(slider_gamma);
    groupe_mappage_tonal.add(&default_mapping_button);
    groupe_mappage_tonal.add(toggle_tone_mapping);
    groupe_mappage_tonal.minimize();
    groupe_texture.add(&groupe_mappage_tonal);

    //Section 6.4

    groupe_texture.minimize();
    gui.add(&groupe_texture);
    /*groupe_cubemap.setup("Cubemap");
    groupe_cubemap.setBorderColor(ofColor::purple);

    cubemap_charger_button.setup("Charger Cubemap");
    cubemap_charger_button.addListener(this, &Application::chargerCubemapButtonPressed);
    groupe_cubemap.add(&cubemap_charger_button);

    slider_reflexion.setup("Réflexion", 0.5, 0.0, 1.0);
    groupe_cubemap.add(&slider_reflexion);

    toggle_use_cubemap.setup("Utiliser Cubemap", false);
    groupe_cubemap.add(&toggle_use_cubemap);

    groupe_cubemap.minimize();
   groupe_texture.add(&groupe_cubemap);

    gui.add(&groupe_texture);*/




    //Section 7 commence ici

    groupe_illumination_cl.setup("Illumination Classic");
    groupe_illumination_cl.setBorderColor(ofColor::purple);

    //Modele d'illumination 7,1
    groupe_couleur_illumination.setup("Couleur d'illumination");
    groupe_couleur_illumination.setBorderColor(ofColor::darkOrange);
    groupe_couleur_illumination.add(color_picker_ambient.set("ambient color", ofColor(63, 63, 63), ofColor(0, 0), ofColor(255, 255)));
    groupe_couleur_illumination.add(color_picker_diffuse.set("diffuse color", ofColor(174, 223, 134), ofColor(0, 0), ofColor(255, 255)));
    groupe_couleur_illumination.add(color_picker_specular.set("specular color", ofColor(255, 255, 255), ofColor(0, 0), ofColor(255, 255)));
    groupe_couleur_illumination.add(color_picker_fill.set("fill color", ofColor(255, 255, 255), ofColor(0, 0), ofColor(255, 255)));
    groupe_couleur_illumination.add(shininess.set("shininess", 128, 0, 128));
    groupe_couleur_illumination.minimize();
    groupe_illumination_cl.add(&groupe_couleur_illumination);


    groupe_modele_illumination.setup("Modele d'illumination");
    groupe_modele_illumination.setBorderColor(ofColor::darkOrange);
    color_fill_illumination_button.setup("Modele de remplissage");
    color_fill_illumination_button.addListener(this, &Application::colorFillIlluminationButtonPressed);
    groupe_modele_illumination.add(&color_fill_illumination_button);
    lambert_illumination_button.setup("Modele de Lambert");
    lambert_illumination_button.addListener(this, &Application::lambertIlluminationButtonPressed);
    groupe_modele_illumination.add(&lambert_illumination_button);
    normal_illumination_button.setup("Modele de normal");
    normal_illumination_button.addListener(this, &Application::normalIlluminationButtonPressed);
    groupe_modele_illumination.add(&normal_illumination_button);
    gouraud_illumination_button.setup("Modele de Gouraud");
    gouraud_illumination_button.addListener(this, &Application::gouraudIlluminationButtonPressed);
    groupe_modele_illumination.add(&gouraud_illumination_button);
    phong_illumination_button.setup("Modele de Phong");
    phong_illumination_button.addListener(this, &Application::phongIlluminationButtonPressed);
    groupe_modele_illumination.add(&phong_illumination_button);
    blinnPhong_illumination_button.setup("Modele de Blinn-Phong");
    blinnPhong_illumination_button.addListener(this, &Application::blinnPhongIlluminationButtonPressed);
    groupe_modele_illumination.add(&blinnPhong_illumination_button);
    groupe_modele_illumination.minimize();
    groupe_illumination_cl.add(&groupe_modele_illumination);

    //Materiaux 7.2
    groupe_materiaux.setup("Materiaux");
    groupe_materiaux.setBorderColor(ofColor(255, 165, 0));

    cube_materiaux_button.setup("Cube");
    cube_materiaux_button.addListener(this, &Application::cubeMateriauxButtonPressed);
    groupe_materiaux.add(&cube_materiaux_button);

    sphere_materiaux_button.setup("Sphere");
    sphere_materiaux_button.addListener(this, &Application::sphereMateriauxButtonPressed);
    groupe_materiaux.add(&sphere_materiaux_button);

    prisme_materiaux_button.setup("Prisme");
    prisme_materiaux_button.addListener(this, &Application::prismeMateriauxButtonPressed);
    groupe_materiaux.add(&prisme_materiaux_button);

    groupe_illumination_cl.add(&groupe_materiaux);


    //Types de lumiere 7.3
    groupe_lumiere.setup("Types de lumiere");
    groupe_lumiere.setBorderColor(ofColor::darkOrange);
    color_picker_constante.set("Constante de couleur", ofColor(60), ofColor(0, 0), ofColor(255, 255));
    nbre_lumiere_total_label.set("Nombre de lumieres total", renderer.get_nbre_lumiere_total(), renderer.get_nbre_lumiere_total(), renderer.get_nbre_lumiere_total());
    nbre_lumiere_directionnelle_label.set("Nombre de lumieres directionnelles", renderer.get_nbre_lumiere_directionnelle(), renderer.get_nbre_lumiere_directionnelle(), renderer.get_nbre_lumiere_directionnelle());
    position_x_lumiere__directionnelle_label.set("Position X lumiere directionnelle", renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_directionnelle()), renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_directionnelle()), renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_directionnelle()));
    position_y_lumiere_directionnelle_label.set("Position Y lumiere directionnelle", renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_directionnelle()), renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_directionnelle()), renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_directionnelle()));
    nbre_lumiere_ponctuelle_label.set("Nombre de lumieres ponctuelles", renderer.get_nbre_lumiere_ponctuelle(), renderer.get_nbre_lumiere_ponctuelle(), renderer.get_nbre_lumiere_ponctuelle());
    position_x_lumiere__ponctuelle_label.set("Position X lumiere ponctuelle", renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_ponctuelle()), renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_ponctuelle()), renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_ponctuelle()));
    position_y_lumiere_ponctuelle_label.set("Position Y lumiere ponctuelle", renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_ponctuelle()), renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_ponctuelle()), renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_ponctuelle()));
    nbre_lumiere_projecteur_label.set("Nombre de lumieres projecteur", renderer.get_nb_lumiere_projecteur(), renderer.get_nb_lumiere_projecteur(), renderer.get_nb_lumiere_projecteur());
    position_x_lumiere_projecteur_label.set("Position X lumiere projecteur", renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_projecteur()), renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_projecteur()), renderer.get_position_lumiere_x(renderer.get_derniere_lumiere_projecteur()));
    position_y_lumiere_projecteur_label.set("Position Y lumiere projecteur", renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_projecteur()), renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_projecteur()), renderer.get_position_lumiere_y(renderer.get_derniere_lumiere_projecteur()));
    groupe_lumiere.add(color_picker_constante);
    groupe_lumiere.add(nbre_lumiere_total_label);
    groupe_lumiere.add(nbre_lumiere_directionnelle_label);
    groupe_lumiere.add(position_x_lumiere__directionnelle_label);
    groupe_lumiere.add(position_y_lumiere_directionnelle_label);
    groupe_lumiere.add(nbre_lumiere_ponctuelle_label);
    groupe_lumiere.add(position_x_lumiere__ponctuelle_label);
    groupe_lumiere.add(position_y_lumiere_ponctuelle_label);
    groupe_lumiere.add(nbre_lumiere_projecteur_label);
    groupe_lumiere.add(position_x_lumiere_projecteur_label);
    groupe_lumiere.add(position_y_lumiere_projecteur_label);
    groupe_lumiere.minimize();
    groupe_illumination_cl.add(&groupe_lumiere);

    //Lumiere multiples 7.4

    gui.add(&groupe_illumination_cl);

    //Section 8 commence ici

    groupe_topologie.setup("Topologie");
    groupe_topologie.setBorderColor(ofColor::red);

    //Courbe parametrique 8.1

    //Surface parametrique 8.2
    surface_parametrique_button.setup("Suface parametrique Coon");
    surface_parametrique_button.addListener(this, &Application::surface_parametrique_button_pressed);
    groupe_topologie.add(&surface_parametrique_button);

    //Shader de tesselation 8.3

    //Triangulation 8.4

    gui.add(&groupe_topologie);

    //Section 9 commence ici

    groupe_rayon.setup("Rayon");
    groupe_rayon.setBorderColor(ofColor::papayaWhip);

    //Intersection 9.1

    //Reflexion 9.2

    //Refraction 9.3

    //Ombrage 9.4

    gui.add(&groupe_rayon);

    //Section 10 commence ici

    groupe_illumination_moderne.setup("Illumination Moderne");
    groupe_illumination_moderne.setBorderColor(ofColor::blue);

    //HDR 10.1

    //Eclairage environnemental 10.2

    //Metallicite 10.3

    //Microfacettes 10.4

    gui.add(&groupe_illumination_moderne);

    


}


    /*
     * brief: Dessine les éléments graphiques de l'application.
     */

    void Application::draw()
    {

        renderer.draw();

        if (checkbox){gui.draw();}
        drawCursor();

    }


    /*
     * brief: Met à jour l'état de l'application.
     */

    void Application::update()
    {
        
        // Couleur de fond
        renderer.background_color1 = color_picker_background;
        renderer.background_color2 = color_picker_background_HSB;

        //Mapping
        renderer.tone_mapping_exposure = slider_exposure;
        renderer.tone_mapping_gamma = slider_gamma;
        renderer.tone_mapping_toggle = toggle_tone_mapping;
        if (renderer.tone_mapping_toggle) {toggle_tone_mapping.set("aces filmic", true);} 
        else {toggle_tone_mapping.set("reinhard", false);}
        
        //Couleur de constante 7.3 et 7.1
        renderer.constant_color = color_picker_constante;
        updateColorIllumination();
        
        // Temps
        time_current = ofGetElapsedTimef();
        time_elapsed = time_current - time_last;
        time_last = time_current;

        if (renderer.is_camera_interactive)
        {
            renderer.is_camera_move_forward = is_key_press_up;
            renderer.is_camera_move_backward = is_key_press_down;
            renderer.is_camera_move_left = is_key_press_left;
            renderer.is_camera_move_right = is_key_press_right;

            renderer.is_camera_move_up = is_key_home;
            renderer.is_camera_move_down = is_key_end;

            renderer.is_camera_pan_left = is_key_del;
            renderer.is_camera_pan_right = is_key_pgdown;

            renderer.is_camera_tilt_up = is_key_pgup;
            renderer.is_camera_tilt_down = is_key_ins;

        }

        if (is_key_press_left)
        {

            if (isTranslationActive) { // Translation
                if (renderer.draw_mode != VectorPrimitiveType::none)
                {
                    renderer.translateLastShape(-(renderer.delta_x * time_elapsed), 0);
                }
                if (renderer.draw_mode_models != VectorModelType::none)
                {
                    renderer.translateLastModel(-(renderer.delta_x * time_elapsed), 0, 0);
                }
            }
            else if (isRotatingActive) // Rotation
            {
                renderer.rotatePrimitive(renderer.speed_delta / 2.0 * time_elapsed);
            }
        }
        if (is_key_press_right)
        {
            if (isTranslationActive) { // Primitives
                if (renderer.draw_mode != VectorPrimitiveType::none)
                {
                    renderer.translateLastShape(renderer.delta_x * time_elapsed, 0);
                }

                if (renderer.draw_mode_models != VectorModelType::none) // Modeles
                {
                    renderer.translateLastModel(renderer.delta_x * time_elapsed, 0, 0);
                }
            }
            else if (isRotatingActive) // Rotation
            {
                renderer.rotatePrimitive(-renderer.speed_delta / 2.0 * time_elapsed);
            }
        }
        if (is_key_press_up)
        {
            if (isTranslationActive) { //Primitives
                if (renderer.draw_mode != VectorPrimitiveType::none) {
                    renderer.translateLastShape(0, -renderer.delta_y * time_elapsed);
                }
                if (renderer.draw_mode_models != VectorModelType::none)
                {
                    renderer.translateLastModel(0, -renderer.delta_y * time_elapsed, 0);
                }
            }
            if (isScalingActive) { // Scale
                float scaleFactor = 1.01;
                renderer.scalePrimitive(scaleFactor);
            }
        }
        if (is_key_press_down)
        {
            if (isTranslationActive) { // Translation 

                if (renderer.draw_mode != VectorPrimitiveType::none) {

                    renderer.translateLastShape(0, renderer.delta_y * time_elapsed);
                }
                if (renderer.draw_mode_models != VectorModelType::none)
                {
                    renderer.translateLastModel(0, renderer.delta_y * time_elapsed, 0);
                }
            }
            if (isScalingActive) { // Scale
                float scaleFactor = 0.99;
                renderer.scalePrimitive(scaleFactor);
            }
        }

        if (isExporting && exportCount < 5) { // Exportation
            float currentTime = ofGetElapsedTimef();
            if (currentTime - lastExportTime >= 2) { // Delai de 2 secondes
                ofImage image;
                image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
                string fileName = "Exportation" + ofToString(exportCount) + ".png";
                image.save(fileName);
                exportCount++;
                lastExportTime = currentTime;

                if (exportCount >= 5) {
                    isExporting = false; // Arret de l'exportation après 5 images
                }
            }
        }

        // Mise à jour des propriétés graphiques
        renderer.lineThickness = lineThickness; // Epaisseur
        renderer.lineColor = lineColor; // Ligne de contour
        renderer.fillColor = fillColor; // Couleur de remplissage

        renderer.update();

    }

/*
* brief: Importe une image dans le rendu.
*/

void Application::importImage() {
    renderer.import_image();
}

/*
* brief: Déclenche le début de l'exportation d'images.
*/

void Application::exportation_button_pressed() {
    isExporting = true;
    exportCount = 0;
    lastExportTime = ofGetElapsedTimef();
}

/*
* brief: Gère l'activation/désactivation du mode de capture d'écran.
*/

void Application::screenshot_button_pressed(bool& value)
{
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

/*
 * brief: Bascule le mode de capture d'écran surprise.
 */

void Application::screenshot_funny_button_pressed(bool& value)
{
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

/*
 * brief: Définit le mode de couleur de fond sur RGB.
 */

void Application::background_rgb_button_pressed()
{
    renderer.color_mode = BackgroundColorType::rgb;
}

/*
 * brief: Définit le mode de couleur de fond sur HSB.
 */

void Application::background_hsb_button_pressed()
{
    renderer.color_mode = BackgroundColorType::hsb;
}

/*
 * brief: Active ou désactive l'affichage de l'histogramme.
 */

void Application::histogramme_button_pressed()
{
    renderer.is_active_histogram = !renderer.is_active_histogram;
}

/* 
brief: Position de la souris dans le rendu.
*/

void Application::mouseMoved(int x, int y)
{
    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;
}

/*
 * brief: Met à jour la position de la souris dans le rendu.
 */

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

/*
 * brief: Gère l'événement de glisser de la souris.
 */

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

/*
 * brief: Gère l'événement de relâchement du bouton de la souris.
 */

void Application::mouseReleased(int x, int y, int button)
{
    selection_start.set(renderer.mouse_press_x, renderer.mouse_press_y);
    selection_end.set(renderer.mouse_current_x, renderer.mouse_current_y);
    renderer.is_mouse_button_pressed = false;

    if (captureMode) // Mode capture
    {
        screenshot(x, y, false);
    }
    else if (captureMode_funny) // Mode funny
    {
        screenshot(x, y, true);
    }
    else if (!isSelectionModeActive) // Selection Active
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

/*
 * brief: Capture d'écran de la zone sélectionnée.
 */

void Application::screenshot(int x, int y, bool z)
{
    // Preparation du nom du fichier sur une timestamp
    std::string timestamp = ofGetTimestampString("-%Y%m%d-%H%M%S");
    std::string filename = "screenshot" + timestamp + ".png";
    std::string folderPath = ofToDataPath("screenshot/");

    // Creation du dossier
    std::string filePath = ofFilePath::join(ofFilePath::getCurrentExeDir(), folderPath + filename);

    // Assurer la creation
    ofDirectory::createDirectory(folderPath, true, true);

    // Zone de capture
    int x1 = min(selection_start.x, selection_end.x);
    int y1 = min(selection_start.y, selection_end.y);
    int x2 = max(selection_start.x, selection_end.x);
    int y2 = max(selection_start.y, selection_end.y);

    // Capture de la zone selectionnee
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

/*
 * brief: Initialise et ajoute les boutons de formes et d'options de dessin au GUI.
 */

void Application::ajout_boutons_formes()
{
    //Boutons pour section 2.3
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

    //Boutons 2.4
    group_dessin_vectoriel_formes.add(face_shape_button.setup("Face", ofParameter<bool>(false)));
    group_dessin_vectoriel_formes.add(maison_shape_button.setup("Maison", ofParameter<bool>(false)));

    //Boutons ? 4
    groupe_geometrie.add(cubeButton.setup("Cube", ofParameter<bool>(false)));
    groupe_geometrie.add(sphereButton.setup("Sphere", ofParameter<bool>(false)));
    groupe_camera.add(perspectiveButton.setup("Perspective", ofParameter<bool>(false)));
    groupe_camera.add(orthogonaleButton.setup("Orthogonale", ofParameter<bool>(false)));

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

/*
 * brief: Retire les écouteurs des boutons lors de la suppression des formes.
 */

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
    animation_button.removeListener(this, &Application::animation_button_pressed);
    instanciation_button.removeListener(this, &Application::instanciation_button_pressed);
}

/*
 * brief: Gestionnaire pour le bouton "None"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Pixel"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Point"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Ligne"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Algo"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Algo DDA"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Algo Bresenham"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Carre"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Rectangle"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Cercle"
 */


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

/*
 * brief: Gestionnaire pour le bouton "Ellipse"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Triangle"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Face"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Maison"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Selection multiple"
 */

void Application::selection_multiple(bool& pressed) {
    isSelectionModeActive = pressed;

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

/*
 * brief: Vérifie si un point est à l'intérieur d'une forme vectorielle donnée.
 */

bool Application::isInside(int x, int y, const VectorPrimitive& shape) {
    if (shape.type == VectorPrimitiveType::square) { // Carre
        float squareX = std::min(shape.position1[0], shape.position2[0]);
        float squareY = std::min(shape.position1[1], shape.position2[1]);
        float squareSize = std::min(abs(shape.position2[0] - shape.position1[0]), abs(shape.position2[1] - shape.position1[1]));
        return x >= squareX && x <= squareX + squareSize && y >= squareY && y <= squareY + squareSize;
    }
    else if (shape.type == VectorPrimitiveType::rectangle) { // Rectangle
        float left = std::min(shape.position1[0], shape.position2[0]);
        float right = std::max(shape.position1[0], shape.position2[0]);
        float top = std::min(shape.position1[1], shape.position2[1]);
        float bottom = std::max(shape.position1[1], shape.position2[1]);
        return x >= left && x <= right && y >= top && y <= bottom;
    }
    else if (shape.type == VectorPrimitiveType::circle) { // Cercle
        float centerX = (shape.position1[0] + shape.position2[0]) / 2;
        float centerY = (shape.position1[1] + shape.position2[1]) / 2;
        float radius = ofDist(shape.position1[0], shape.position1[1], shape.position2[0], shape.position2[1]) / 2;
        return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius;
    }
    else if (shape.type == VectorPrimitiveType::ellipse) { // Ellipse
        float centerX = (shape.position1[0] + shape.position2[0]) / 2;
        float centerY = (shape.position1[1] + shape.position2[1]) / 2;
        float radiusX = abs(shape.position2[0] - shape.position1[0]) / 2;
        float radiusY = abs(shape.position2[1] - shape.position1[1]) / 2;
        return (pow(x - centerX, 2) / pow(radiusX, 2)) + (pow(y - centerY, 2) / pow(radiusY, 2)) <= 1;
    }
    else if (shape.type == VectorPrimitiveType::face) { // Face

        float centerX = (shape.position1[0] + shape.position2[0]) / 2;
        float centerY = (shape.position1[1] + shape.position2[1]) / 2;
        float radius = ofDist(shape.position1[0], shape.position1[1], shape.position2[0], shape.position2[1]) / 2;
        return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius;
    }
    else if (shape.type == VectorPrimitiveType::maison) { // Maison

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
    else if (shape.type == VectorPrimitiveType::cube) { // Cube

        float rectLeft = shape.position1[0] - 50;
        float rectTop = shape.position1[1] - 50;
        float rectRight = rectLeft + 100;
        float rectBottom = rectTop + 100;

        return x >= rectLeft && x <= rectRight && y >= rectTop && y <= rectBottom;
    }
    else if (shape.type == VectorPrimitiveType::sphere) { // Sphere

        float centerX = shape.position1[0];
        float centerY = shape.position1[1];
        float radius = 50;

        return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius;
    }

    return false;
}

/*
 * brief: Désactive tous les modes de transformation
 */

void Application::noneTransformationButtonPressed(){
    isTranslationActive = false;
    isRotatingActive = false;
    isScalingActive = false;
    ofLog() << "<mode: transformation : none>" << isTranslationActive;
}

/*
 * brief: Active ou désactive le mode de translation
 */

void Application::translateButtonPressed() 
{

        isTranslationActive = !isTranslationActive;
        isRotatingActive = false;
        isScalingActive = false;
        ofLog() << "<mode: translation>" << isTranslationActive;
}

/*
 * brief: Active ou désactive le mode de rotation
 */

void Application::rotateButtonPressed() 
{
        isTranslationActive = false;
        isRotatingActive = !isRotatingActive;
        isScalingActive = false;
        ofLog() << "<mode: rotation>" << isRotatingActive ;
}

/*
 * brief: Active ou désactive le mode Scale
 */

void Application::scaleButtonPressed() 
{
        isTranslationActive = false;
        isRotatingActive = false;
        isScalingActive = !isScalingActive;
        ofLog() << "<mode: scale>" << isScalingActive;
}

/*
 * brief: Annule la dernière action effectuée.
 */


void Application::undo_button_pressed()
{
    renderer.undo();
}

/*
 * brief: Rétablit l'action précédemment annulée.
 */

void Application::redo_button_pressed()
{
    renderer.redo();
}

/*
 * brief: Gestionnaire pour le bouton "Cube"
 */

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

/*
 * brief: Gestionnaire pour le bouton "Sphere"
 */

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

/*
 * brief: Activation de l'animation d'objet SVG.
 */

void Application::animation_button_pressed()
{
    renderer.animation_svg_object_active = !(renderer.animation_svg_object_active);
}

/*
 * brief: Activation de l'instanciation.
 */

void Application::instanciation_button_pressed()
{
    renderer.instanciation_active = !(renderer.instanciation_active);
}

/*
 * brief: Activation mode de camera en perspective.
 */

void Application::perspectiveButtonPressed() {
    renderer.is_camera_perspective = true;
    renderer.setup_camera();
}

/*
 * brief: Activation mode de camera orthogonale.
 */

void Application::orthogonaleButtonPressed() {
    renderer.is_camera_perspective = false;
    renderer.setup_camera();
}

/*
 * brief: Interactivité de la caméra.
 */

void Application::toggleCameraInteractive() {
    renderer.is_camera_interactive = !renderer.is_camera_interactive;
    renderer.setup_camera();
    ofLogNotice() << "mode cam active" << renderer.is_camera_interactive;

}

/*
 * brief: Curseur personnalise en fonction de l'etat du curseur
 */

void Application::drawCursor() {

    int mouseX = ofGetMouseX(); // Position en X
    int mouseY = ofGetMouseY(); // Position en Y

    switch (cursor) {
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

/*
 * brief: Desactive les modes de dessin de formes et de modeles
 */

void Application::non_model_button_pressed() {
    renderer.draw_mode = VectorPrimitiveType::none;
    renderer.draw_mode_models = VectorModelType::none;
}

/*
 * brief: Ouvre un dialogue pour selectionner un modèle et l'importer
 */

void Application::import_model_button_pressed() {
    ofFileDialogResult result = ofSystemLoadDialog("Select a folder", true);
    ofLogNotice() << "Dialog result: " << result.bSuccess;
    if (result.bSuccess)
    {
        string folderPath = result.filePath;
        ofLogNotice() << "Selected folder: " << folderPath;
    }
    else
    {
        ofLogNotice() << "Dialog canceled by the user";
    }
}

/*
 * brief: Active le dessin du modèle prédéfini 1
 */

void Application::predef1_model_button_pressed(){
        renderer.draw_mode = VectorPrimitiveType::none;
        renderer.draw_mode_models = VectorModelType::predef1;
}

/*
 * brief: Active le dessin du modèle prédéfini 2
 */

void Application::predef2_model_button_pressed(){
        renderer.draw_mode = VectorPrimitiveType::none;
        renderer.draw_mode_models = VectorModelType::predef2;
}

/*
 * brief: Active le dessin du modèle prédéfini 3
 */

void Application::predef3_model_button_pressed(){

        renderer.draw_mode = VectorPrimitiveType::none;
        renderer.draw_mode_models = VectorModelType::predef3;
}


/*
 * brief: Retire le dernier modele affiche
 */

void Application::remove_last_model_button_pressed(){
}

/*
 * brief: Actions a realiser lorsqu'une touche est pressee
 */

void Application::keyPressed(int key)
{
    switch (key)
    {
    case OF_KEY_LEFT: // Déplacer la caméra ou l'objet vers la gauche
        is_key_press_left = true;
        break;
    case OF_KEY_UP: // Zoom avant
        is_key_press_up = true;
        break;
    case OF_KEY_RIGHT: // Déplacer la caméra ou l'objet vers la droite
        is_key_press_right = true;
        break;
    case OF_KEY_DOWN: // Zoom arrière
        is_key_press_down = true;
        break;
    case 57362: //Cam up (home)
        is_key_home = true;
        break;
    case 57363: //Cam down (end)
        is_key_end = true;
        break;
    case 127: //Pan left (ins)
        is_key_del = true;
        break;
    case 57361: //Pan right (pgdown)
        is_key_pgdown = true;
        break;
    case 57360: //Tilt up (pgup)
        is_key_pgup = true;
        break;
    case 57364: //Tilt down (ins)
        is_key_ins = true;
        break;

    default:
        ofLog() << key;
        break;
    }
}


void Application::keyReleased(int key)
{
    switch (key)
    {
    case 117: // Key U
        checkbox = !checkbox;
        ofLog() << "<toggle ui: " << checkbox << ">";
        break;
    case 114: // Key R
        renderer.reset();
        break;
    case OF_KEY_LEFT: // Key ←
        is_key_press_left = false;
        renderer.is_camera_move_left = false;
        break;

    case OF_KEY_UP: // Key ↑
        is_key_press_up = false;
        renderer.is_camera_move_forward = false;
        break;

    case OF_KEY_RIGHT: // Key →
        is_key_press_right = false;
        renderer.is_camera_move_right = false;
        break;

    case OF_KEY_DOWN: // Key ↓
        is_key_press_down = false;
        renderer.is_camera_move_backward = false;
        break;
    case 57362: // Cam up (home)
        is_key_home = false;
        renderer.is_camera_move_up = false;
        break;
    case 57363: // Cam down (end)
        is_key_end = false;
        renderer.is_camera_move_down = false;
        break;
    case 127: // Pan left (ins)
        is_key_del = false;
        renderer.is_camera_pan_left = false;
        break;
    case 57361: // Pan right (pgdown)
        is_key_pgdown = false;
        renderer.is_camera_pan_right = false;
        break;
    case 57360: // Tilt up (pgup)
        is_key_pgup = false;
        renderer.is_camera_tilt_up = false;
        break;
    case 57364: // Tilt down (ins)
        is_key_ins = false;
        renderer.is_camera_tilt_down = false;
        break;

    case 49: // Key 1
        renderer.camera_active = Camera::devant;
        renderer.setup_camera();
        ofLog() << "<app::Cam Devant";
        break;

    case 50: // Key 2
        renderer.camera_active = Camera::derriere;
        renderer.setup_camera();
        ofLog() << "<app::Cam Derriere>";
        break;

    case 51: // Key 3
        renderer.camera_active = Camera::gauche;
        renderer.setup_camera();
        ofLog() << "<app::Cam Gauche>";
        break;

    case 52: // Key 4
        renderer.camera_active = Camera::droite;
        renderer.setup_camera();
        ofLog() << "<app::cam Droite>";
        break;

    case 53: // Key 5
        renderer.camera_active = Camera::dessus;
        renderer.setup_camera();
        ofLog() << "<app::cam Dessus>";
        break;

    case 54: // Key 6
        renderer.camera_active = Camera::dessous;
        renderer.setup_camera();
        ofLog() << "<app::cam Dessous>";
        break;

    case 'p': // Key P
        renderer.is_camera_perspective = true;
        renderer.setup_camera();
        ofLog() << "<perpective projection>";
        break;

    case 'o': // Key O
        renderer.is_camera_perspective = false;
        renderer.setup_camera();
        ofLog() << "<orthographic projection>";
        break;

    case 102: // Key F
        renderer.is_flip_axis_y = !renderer.is_flip_axis_y;
        ofLog() << "Flip y axe";
        break;

    default:
        ofLog() << "<app::key num>" << key;
        break;
    }
}

/*
 * brief: Etat du curseur sur par défaut
 */

void Application::cursorDefaultButtonPressed() {

    cursor = CURSOR_DEFAULT;

}

/*
 * brief: Etat du curseur pour dessiner des lignes
 */

void Application::cursorDrawLineButtonPressed() {

    cursor = CURSOR_DRAW_LINE;

}

/*
 * brief: Etat du curseur pour dessiner des cercles
 */

void Application::cursorDrawCircleButtonPressed() {

    cursor = CURSOR_DRAW_CIRCLE;

}

/*
 * brief: Etat du curseur pour la selection
 */

void Application::cursorSelectButtonPressed() {

    cursor = CURSOR_SELECT;

}

/*
 * brief: Définit l'état du curseur pour la translation
 */

void Application::cursorTranslateButtonPressed() {

    cursor = CURSOR_TRANSLATE;

}

/*
 * brief: Définit l'état du curseur pour la rotation
 */

void Application::cursorRotateButtonPressed() {

    cursor = CURSOR_ROTATE;

}

/*
 * brief: Ajoute nouvel element dans la scene
 */

void Application::addElementPressed() {


}

/*
 * brief: Supprime element selectionne de la scene
 */

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


/*
 * brief: Mode de dessin en 2D
 */

void Application::mode_dessin_pressed()
{
    renderer.mode_vue = Mode_Vue::dessin;
}

/*
 * brief: Mode de la caméra a une seule camera 3D
 */

void Application::mode_1cam_pressed()
{
    renderer.mode_vue = Mode_Vue::camera_3d;

}

/*
 * brief: Mode de la camera a deux camras 3D.
 */

void Application::mode_2cam_pressed()
{
    renderer.mode_vue = Mode_Vue::double_cam;
    //renderer.mode_cam = true;
}



/*
 * brief: Algorithme de filtrage bileneaire
 */

void Application::bilineaireButtonPressed() {
    ofLog() << "<Bilineaire>";
    renderer.bilineaire_application();

}

/*
 * brief: Algorithme de filtrage trilineaire
 */

void Application::trilineaireButtonPressed() {
    ofLog() << "<Trilineaire>";
    renderer.trilineaire_application();

}

/*
 * brief: Algorithme de filtrage convulation
 */

void Application::anisotropiqueButtonPressed() {
    ofLog() << "<Anisotropique>";
   
    renderer.anisotropique_application();

}

/*
 * brief: Mode d'affichage Mesh
 */


void Application::meshfilled_button_pressed()
{
    renderer.occlusion = Occlusion::meshfiled;
}

/*
 * brief: Mode affichage Wire
 */

void Application::wireframe_button_pressed()
{
    renderer.occlusion = Occlusion::wireframe;
}

//Section 6.3
void Application::default_mapping()
{
    renderer.tone_mapping_activated = mapping_Active;
    ofLog() << "Tone mapping" << mapping_Active;
    mapping_Active = !mapping_Active;
    
}

void Application::reset_mapping_slidder()
{
  slider_exposure = 1.0f;
  slider_gamma = 2.2f;
}


//Section 6.4
/*void Application::chargerCubemapButtonPressed()
{

    //ofLogNotice() << "Chargement du Cubemap";

    /*bool chargementReussi = monCubemap.load("chemin/vers/les/textures/cubemap");

    if (chargementReussi) {
        ofLogNotice() << "Cubemap chargé avec succès.";
    }
    else {
        ofLogError() << "Erreur lors du chargement du Cubemap.";
    }


}*/

//section 7

//Section 7.1

void Application::updateColorIllumination()
{
    renderer.color_fill = color_picker_fill;
    renderer.color_ambient = color_picker_ambient;
    renderer.color_diffuse = color_picker_diffuse;
    renderer.color_specular = color_picker_specular;
    renderer.shininess = shininess;
}

//Bouton modele illumination
void Application::colorFillIlluminationButtonPressed() {
    ofLog() << "<Color Fill Illumination>";
    renderer.shader_type = ShaderType::COLOR_FILL;
}

void Application::lambertIlluminationButtonPressed() {
    ofLog() << "<Lambert Illumination>";
    renderer.shader_type = ShaderType::LAMBERT;
}

void Application::normalIlluminationButtonPressed()
{
    ofLog() << "<Normal Illumination>";
    renderer.shader_type = ShaderType::NORMAL;

}

void Application::gouraudIlluminationButtonPressed() {
    ofLog() << "<Gouraud Illumination>";
    renderer.shader_type = ShaderType::GOURAUD;
}

void Application::phongIlluminationButtonPressed() {
    ofLog() << "<Phong Illumination>";
    renderer.shader_type = ShaderType::PHONG;
}

void Application::blinnPhongIlluminationButtonPressed() {
    ofLog() << "<Blinn Phong Illumination>";
    renderer.shader_type = ShaderType::BLINN_PHONG;
}


/*
 * brief: Cube avec materiaux
 */
void Application::cubeMateriauxButtonPressed() {
    ofLog() << "<Cube Materiaux>";
    renderer.is_material_cube = true;
    renderer.is_material_sphere = false;
    renderer.is_material_prisme = false;
}

// Sphere avec Materiaux

void Application::sphereMateriauxButtonPressed() {
    ofLog() << "<Sphere Materiaux>";
    renderer.is_material_cube = false;
    renderer.is_material_sphere = true;
    renderer.is_material_prisme = false;
}

// Prisme avec Matériaux

void Application::prismeMateriauxButtonPressed() {
    ofLog() << "<Prisme Materiaux>";
    renderer.is_material_cube = false;
    renderer.is_material_sphere = false;
    renderer.is_material_prisme = true;
}

//section 7.3

// Section 8.2 Suface Parametrique Coon ou bezier

void Application::surface_parametrique_button_pressed() {
    ofLog() << "<Surface Parametrique>";
    renderer.is_coon_parametrique = true;
}

/*
 * brief: Nettoyer, supprimer, fermeture de l'application
 */

void Application::exit()
{
    screenshot_button.removeListener(this, &Application::screenshot_button_pressed);
    screenshot_button_funny.removeListener(this, &Application::screenshot_funny_button_pressed);
    background_rgb_button.removeListener(this, &Application::background_rgb_button_pressed);
    background_hsb_button.removeListener(this, &Application::background_hsb_button_pressed);
    histogramme_button.removeListener(this, &Application::histogramme_button_pressed);

    retirer_boutons_formes();
    translateButton.removeListener(this, &Application::translateButtonPressed);
    rotateButton.removeListener(this, &Application::rotateButtonPressed);
    scaleButton.removeListener(this, &Application::scaleButtonPressed);

    undo_button.removeListener(this, &Application::undo_button_pressed);
    redo_button.removeListener(this, &Application::redo_button_pressed);

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

    remove_last_model_button.removeListener(this, &Application::remove_last_model_button_pressed);

    addElementButton.removeListener(this, &Application::addElementPressed);
    removeElementButton.removeListener(this, &Application::removeElementPressed);

    mode_dessincam_button.removeListener(this, &Application::mode_dessin_pressed);
    mode_1cam_button.removeListener(this, &Application::mode_1cam_pressed);
    mode_2cam_button.removeListener(this, &Application::mode_2cam_pressed);

    meshfilled_button.removeListener(this, &Application::meshfilled_button_pressed);
    wireframe_button.removeListener(this, &Application::wireframe_button_pressed);
    camera_interactive_button.removeListener(this, &Application::toggleCameraInteractive);

    //Enlever boutons 6.3
    reset_slider_button.removeListener(this, &Application::reset_mapping_slidder);
    default_mapping_button.removeListener(this, &Application::default_mapping);

    //Enlever boutons 7.1
    color_fill_illumination_button.removeListener(this, &Application::colorFillIlluminationButtonPressed);
    lambert_illumination_button.removeListener(this, &Application::lambertIlluminationButtonPressed);
    normal_illumination_button.removeListener(this, &Application::normalIlluminationButtonPressed);
    gouraud_illumination_button.removeListener(this, &Application::gouraudIlluminationButtonPressed);
    phong_illumination_button.removeListener(this, &Application::phongIlluminationButtonPressed);
    blinnPhong_illumination_button.removeListener(this, &Application::blinnPhongIlluminationButtonPressed);

    ofLog() << "<app::exit>";

}
