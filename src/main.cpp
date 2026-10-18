#include <SFML/Graphics.hpp>

int main() {
    // Fenster mit 800x600 Pixeln erstellen
    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake - SFML");
    window.setFramerateLimit(60);

    // Hauptschleife des Spiels
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Fenster schließen, wenn auf das 'X' geklickt wird
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Zeichnen
        window.clear(sf::Color::Black);
        
        // Hier werden später Schlange und Essen gezeichnet!

        window.display();
    }

    return 0;
}