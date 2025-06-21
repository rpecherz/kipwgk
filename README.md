# kipwgk – Edytor Krzywych i Powierzchni

(under development ...)


Aplikacja graficzna 2D/3D oparta na Qt i OpenGL. Umożliwia rysowanie i edycję różnorakich krzywych interpolacyjnych, pracę z otoczkami wypukłymi, animację punktów kontrolnych oraz renderowanie w pipelinie OpenGL z użyciem shaderów i buforów wierzchołków.

## Funkcje

* Rysowanie i edycja krzywych interpolacyjnych
* Dodawanie, przesuwanie i usuwanie punktów kontrolnych
* Obliczanie otoczek wypukłych
* Implementacja algorytmów rysowania bez zewnętrznych bibliotek
* Widok siatki, zoom i interaktywne GUI
* Tryb animacji z regulacją prędkości
* Obsługa kolorów, szerokości i stylów linii

## Technologie

* Qt 6 / Qt Widgets
* OpenGL 4.5 (Core Profile)
* GLSL
* C++20

## Kompilacja i uruchomienie

### Wymagania

* Qt 6.x
* Kompilator C++ z obsługą C++20 (GCC 11+, Clang, MSVC 2022+)
* OpenGL 4.5+

### QMake

```bash
qmake
make
./CurveEditor
```

### CMake

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/CurveEditor
```

## Planowane funkcje
* Obsługa powierzchni 3D (np. Bézier, triangles)
* Eksport do formatów SVG i PNG
* Tryb animacji z zapisem do wideo