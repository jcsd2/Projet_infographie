#include "application.h"


void Application::setup()
{
  ofSetWindowTitle("Projet_Session");

  ofLog() << "<app::setup>";

  renderer.setup();
  gui.setup("Interface");
  checkbox.setName("Gui visible");
  gui.setDefaultHeight(42);
  gui.setBorderColor(ofColor(255, 0, 0));

  checkbox = true; // Initialisation de l'indicateur du Gui
  gui.add(checkbox);
  algo_of_button = true;
  captureMode = false; // Initialisation l'indicateur du mode de capture d'écran
  captureMode_funny = false;

  //Groupe du critere 1 Image
  group_image.setup("Image");
  screenshot_button.setup("Capture d'ecran", false);
  screenshot_button.addListener(this, &Application::screenshot_button_pressed);
  group_image.add(&screenshot_button);
  screenshot_button_funny.setup("Capture d'ecran\n surprise", false);
  screenshot_button_funny.addListener(this, &Application::screenshot_funny_button_pressed);
  group_image.add(&screenshot_button_funny);
  gui.add(&group_image);
  
  //color_picker_background.set("couleur du canevas RGB", ofColor(60), ofColor(0, 0), ofColor(255, 255));
  color_picker_background_HSB.set("couleur du canevas HSB",ofColor::fromHsb(128, 255, 255));
  //group_image.add(color_picker_background);
  group_image.add(color_picker_background_HSB);

  // Initialisation et ajout du bouton Exportation
  exportation_button.setup("Exportation");
  exportation_button.addListener(this, &Application::exportation_button_pressed);
  group_image.add(&exportation_button); 

  gui.add(&group_image);

  //groupe du critere 2 Dessin vectoriel
  group_dessin_vectoriel.setup("Dessin Vectoriel");
  gui.add(&group_dessin_vectoriel);
  group_dessin_vectoriel_formes.setup("Formes a dessiner");
  group_dessin_vectoriel.add(&group_dessin_vectoriel_formes);

  //Sous-groupe pour le type d'algo pour dessiner une ligne
  group_dessin_algo_ligne.setup("Type d'algorithme \nde rasterisation");
  
  
  // Ajout des boutons pour chaque formes  
  ajout_boutons_formes();
  gui.setPosition(1,1);

}

void Application::draw()
{
  renderer.draw();
  if (checkbox)
    gui.draw();

  // Dessiner l'image importée si elle est chargée
  if (importedImage.isAllocated()) {
      importedImage.draw(0, 0); // Ajustez la position et la taille selon vos besoins
  }
}



void Application::update()
{
  //renderer.background_color1 = color_picker_background;
  renderer.background_color2 = color_picker_background_HSB;

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
  
    default:
      break;
  }
}

void Application::screenshot_button_pressed(bool& value)
{
    // Activer/désactiver le mode de capture d'écran
    captureMode = value;
    if (captureMode)
    {
        captureMode_funny = false;
        screenshot_button_funny = false;
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
}

void Application::mouseDragged(int x, int y, int button)
{
  renderer.mouse_current_x = x;
  renderer.mouse_current_y = y;
}

void Application::mouseReleased(int x, int y, int button)
{
  selection_start.set(renderer.mouse_press_x, renderer.mouse_press_y);
  selection_end.set(renderer.mouse_current_x, renderer.mouse_current_y);
  renderer.is_mouse_button_pressed = false;

  // Vérifier si le mode de capture d'écran est activé
  if (captureMode)
  {
    screenshot(x,y, false);
  }
  else if (captureMode_funny)
  {
      screenshot(x,y, true);
  }
  else if (renderer.draw_mode != VectorPrimitiveType::none)
  {
      renderer.add_vector_shape(renderer.draw_mode);
  }
  else
  {
    ofLog() << "<Fin de la sélection de zone>";
  }
}

void Application::screenshot(int x,int y, bool z)
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
      if(!z) //Screenshot normal
      {
        ofColor pixelColor = tempImage.getColor(i, j);
        sampledImage.setColor(i, j, pixelColor);
      }
      else if(z)
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

  group_dessin_vectoriel_formes.add(&group_dessin_algo_ligne);
  group_dessin_algo_ligne.setDefaultHeight(37);
  group_dessin_algo_ligne.add(algo_of_button.setup("Rasterisation par\n openFrameworks", ofParameter<bool>(true)));
  group_dessin_algo_ligne.add(algo_dda_button.setup("Rasterisation par\n DDA", ofParameter<bool>(false)));
  group_dessin_algo_ligne.add(algo_bressenham_button.setup("Rasterisation par\n Bressenham", ofParameter<bool>(false)));
  

  group_dessin_vectoriel_formes.add(square_shape_button.setup("Carre", ofParameter<bool>(false)));
  group_dessin_vectoriel_formes.add(rectangle_shape_button.setup("Rectangle", ofParameter<bool>(false)));
  group_dessin_vectoriel_formes.add(circle_shape_button.setup("Cercle", ofParameter<bool>(false)));
  group_dessin_vectoriel_formes.add(ellipse_shape_button.setup("Ellipse", ofParameter<bool>(false)));
  group_dessin_vectoriel_formes.add(triangle_shape_button.setup("Triangle", ofParameter<bool>(false)));
  group_dessin_vectoriel_formes.add(face_shape_button.setup("Face", ofParameter<bool>(false)));
  group_dessin_vectoriel_formes.add(maison_shape_button.setup("Maison", ofParameter<bool>(false)));


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

void Application::retirer_boutons_formes(){
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

}

void Application::button_none_pressed(bool &pressed)
{
  if (pressed) {
    renderer.draw_mode = VectorPrimitiveType::none;
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

void Application::button_pixel_pressed(bool &pressed)
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

void Application::button_point_pressed(bool &pressed)
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

void Application::button_line_pressed(bool &pressed)
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

void Application::button_algo_of_pressed(bool &pressed){
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

void Application::button_algo_dda_pressed(bool &pressed)
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

void Application::button_algo_bressenham_pressed(bool &pressed)
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

void Application::button_square_pressed(bool &pressed)
{
  if (pressed) {
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

void Application::button_rectangle_pressed(bool &pressed)
{
  if (pressed) {
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

void Application::button_circle_pressed(bool &pressed)
{
  if (pressed) {
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

void Application::button_ellipse_pressed(bool &pressed)
{
  if (pressed) {
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

void Application::button_triangle_pressed(bool &pressed)
{
  if (pressed) {
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

void Application::exportation_button_pressed() {
    isExporting = true;
    exportCount = 0;
    lastExportTime = ofGetElapsedTimef();
}

void Application::button_face_pressed(bool& pressed)
{
    if (pressed) {
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

void Application::exit()
{
  // Remove the listener for toggle button valueChanged event
    screenshot_button.removeListener(this, &Application::screenshot_button_pressed);
   retirer_boutons_formes();
   ofLog() << "<app::exit>";

}