# Mond3D

**Mond3D** ist ein OpenGL C++ - Projekt, das eine 3D-Darstellung des Mondes unter Verwendung von OpenGL und Shadern generiert. Der Benutzer kann den Mond in Echtzeit drehen, zoomen und verschiedene Transformationen anwenden. 

## Inhaltsverzeichnis

- Autor
- Features
- Screenshots
- Voraussetzungen
- Abhängigkeiten
- Projektstruktur
- Lizenz
- Kontakt
- Credits
- Verwendung
- Fehlerbehebung

## Autor

- **Yasin** - [GitHub-Profil](https://github.com/yasincodes97)

## Features

- **3D-Rotation**: Der Mond kann mithilfe den Pfeiltasten `rechts`und `links` um die eigene Achse rotiert werden.
- **Texturen und Beleuchtung**: Texturen und Beleuchtung verbessern das Aussehen des Mondes.
- **Zoomen**: Die Kamera lässt sich in rein- und rauszoomen mit den Pfeiltasten `hoch` und `runter`.
- **Transformationen**: Der Mond kann auf der X- und Y-Achse gedreht und geneigt werden mit den Tasten `x` und `y`.
- **Auto-Run**: Mit der Taste `r` beginnt der Mond sich automatisch zu drehen.

## Screenshots

![Mond3D - Picture1](resources/pic1.jpg)
![Mond3D - Picture1](resources/pic2.jpg)

## Voraussetzungen

- **C++ Compiler**:   Unterstützung für C++11 oder höher
- **`OpenGL`**:       Version 3.3 oder höher
- **GLFW**:           3.3 oder höher
- **GLEW**:           2.1 oder höher
- **GLM**:            Mathe-Bibliothek für Vektoren und Matrizen
- **`CMake`**:        Für die Erstellung des Projekts

## Abhängigkeiten

- **`OpenGL`**
- **GLFW**
- **GLAD**
- **GLM**
- **`CMake`**

## Projektstruktur

- **`main.cpp`**:                            Hauptprogramm
- **`Playground.hpp`**:                      Header File der Hauptklasse für das Spiel- und Anzeigelogik
- **`Playground.cpp`**:                      Implementierung der Spiel- und Anzeigelogik
- **`RenderObject.hpp`**:                    Header File zur Darstellung von 3D Objekten im 3D Raum
- **`RenderObject.cpp`**:                    Implementierung der Render-Logik
- **`SimpleFragmentShader.fragmentshader`**: Fragment-Shader-Datei
- **`SimpleVertexShader.vertexshader`**:     Vertex-Shader-Datei

## Lizenz

Das Projekt ist Open-Source und kann ohne Lizenzierung verwendet und angepasst werden.

## Kontakt

Fühle dich frei, Anpassungen oder Anregungen vorzunehmen.
Bei Fragen gerne mich kontaktieren.

## Credits

Ich möchte den folgenden Ressourcen danken, die mir bei der Entwicklung dieses Projekts geholfen haben:

- **GLFW** und **GLEW** für Fensterverwaltung und OpenGL-Loader
- **GLM** für die Mathe-Bibliothek
- **OpenGL Tutorials**: 
    - [opengl-tutorial](https://www.opengl-tutorial.org/)
    - [opengl-tutorial](https://www.youtube.com/watch?v=bgvPAI2Cp2s)
    - [opengl-tutorial](https://www.youtube.com/watch?v=EUFUQkbC4Gk)
    - [resources](https://sketchfab.com)
    - [resources](https://github.com/1kar/OpenGL-SolarSystem)

## Verwendung

1. **Installation**
   ```bash
   git clone <https://github.com/yasincodes97/Moon3D.git>
   cd Moon3D
   mkdir build
   cd build
   cmake ..
   make
   ```

2. **Ausführung**:
   ```bash
   ./Moon3D
   ```

3. **Alternative**
`Moon3D/playground/playground.exe` ausführen

## Fehlerbehebung

- **Problem beim Starten**: Stelle sicher, dass OpenGL 3.3+ auf deinem System unterstützt wird. Einige ältere GPUs sind nicht kompatibel.
- **Shader-Fehler**: Überprüfe, ob die Shader-Dateien (SimpleVertexShader.vertexshader, SimpleFragmentShader.fragmentshader) im `resources/`-Ordner verfügbar sind und korrekt geladen werden.
- **Kompilierungsfehler**: Prüfe, ob alle Bibliotheken korrekt installiert sind und die `CMakeLists.txt` angepasst ist.
- **Alternativ** einfach die playground.exe File im Moon3D/playground starten.
