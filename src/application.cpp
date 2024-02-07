#include "application.h"


void Application::setup()
{
  ofSetWindowTitle("Projet_Session");

  ofLog() << "<app::setup>";

  renderer.setup();
  gui.setup("Interface");
  checkbox.setName("Gui visible");
  checkbox = true; // Initialisation de l'indicateur du Gui
  gui.add(checkbox);

  is_selecting = false; // Initialisation de l'indicateur de sélection
  captureMode = false; // Initialisation l'indicateur du mode de capture d'écran
  
  //Groupe du critere 1 Image
  group_image.setup("Image");
  screenshot_button.setup("Capture d'ecran", false);
  screenshot_button.addListener(this, &Application::screenshot_button_pressed);
  group_image.add(&screenshot_button);
  gui.add(&group_image);

  //groupe du critere 2 Dessin vectoriel
  group_dessin_vectoriel.setup("Dessin Vectoriel");
  gui.add(&group_dessin_vectoriel);
}

void Application::draw()
{
  renderer.draw();
  if (checkbox)
    gui.draw();
}


void Application::update()
{
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
        ofLog() << "Mode de capture d'écran activé.";
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
  // Finir la sélection de zone
  is_selecting = false;
  // Vérifier si le mode de capture d'écran est activé
  if (captureMode)
  {
    screenshot(x,y);
  }
  else
  {
    ofLog() << "<Fin de la sélection de zone>";
  }
    
}
void Application::screenshot(int x,int y)
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
      ofColor pixelColor = tempImage.getColor(i, j);
      sampledImage.setColor(i, j, pixelColor);
    }
  }

  // Mettre à jour les pixels de l'image résultante
  sampledImage.update();

  // Enregistrer l'image résultante avec le nom et l'emplacement fournis par l'utilisateur
  sampledImage.save(filePath);

  ofLog() << "<Screenshot de la zone sélectionnée avec échantillonnage>";

}

void Application::exit()
{
  // Remove the listener for toggle button valueChanged event
    screenshot_button.removeListener(this, &Application::screenshot_button_pressed);
    ofLog() << "<app::exit>";
}