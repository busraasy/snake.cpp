#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <string>

int main() {
    // Zufallsgenerator für das Futter initialisieren
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Fenster initialisieren (800x600 Pixel)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Snake - C++ & SFML");
    window.setFramerateLimit(60);

    // Uhr für die Geschwindigkeits-Steuerung der Schlange
    sf::Clock clock;
    const float timePerFrame = 0.15f; 
    sf::Time elapsedV = sf::Time::Zero;

    const float tileSize = 20.f;

    // --- SCHRIFTART UND TEXT-ELEMENTE ---
    // Versucht eine Systemschriftart zu laden (unter Windows "arial.ttf" in C:/Windows/Fonts/)
    // Falls das nicht klappt, lege eine arial.ttf direkt in dein Projektverzeichnis.
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.openFromFile("arial.ttf")) {
            // Hier greift der Fall, falls keine der beiden Schriften gefunden wird
        }
    }

    int score = 0;
    int highScore = 0;

    sf::Text scoreText(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.f, 5.f});

    sf::Text highScoreText(font);
    highScoreText.setCharacterSize(18);
    highScoreText.setFillColor(sf::Color(200, 200, 100));
    highScoreText.setPosition({630.f, 5.f});

    auto updateText = [&]() {
        scoreText.setString("Score: " + std::to_string(score));
        highScoreText.setString("Highscore: " + std::to_string(highScore));
    };
    updateText();

    // Funktion zum Zurücksetzen des Spiels
    auto resetGame = [&](std::vector<sf::Vector2f>& body, sf::Vector2f& dir) {
        body.clear();
        body.push_back({400.f, 300.f}); // Kopf
        body.push_back({380.f, 300.f}); // Körper 1
        body.push_back({360.f, 300.f}); // Körper 2
        dir = {tileSize, 0.f};

        if (score > highScore) {
            highScore = score;
        }
        score = 0;
        updateText();
    };

    // Die Schlange initialisieren
    std::vector<sf::Vector2f> snakeBody;
    sf::Vector2f direction(tileSize, 0.f);
    sf::Vector2f nextDirection = direction;
    resetGame(snakeBody, direction);
    nextDirection = direction;

    // Rechteck-Vorlage zum Zeichnen
    sf::RectangleShape block(sf::Vector2f(tileSize - 2.f, tileSize - 2.f));

    // Futter (roter Apfel)
    sf::RectangleShape food(sf::Vector2f(tileSize - 2.f, tileSize - 2.f));
    food.setFillColor(sf::Color::Red);
    
    auto spawnFood = [&]() {
        float randomX = static_cast<float>((std::rand() % (800 / static_cast<int>(tileSize))) * tileSize);
        float randomY = static_cast<float>((std::rand() % (600 / static_cast<int>(tileSize))) * tileSize);
        food.setPosition({randomX, randomY});
    };
    spawnFood();

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        elapsedV += dt;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Tastatursteuerung
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && direction.y == 0.f) {
            nextDirection = {0.f, -tileSize};
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && direction.y == 0.f) {
            nextDirection = {0.f, tileSize};
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && direction.x == 0.f) {
            nextDirection = {-tileSize, 0.f};
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && direction.x == 0.f) {
            nextDirection = {tileSize, 0.f};
        }

        // --- BEWEGUNG & LOGIK (TICK) ---
        if (elapsedV.asSeconds() >= timePerFrame) {
            direction = nextDirection;

            sf::Vector2f newHead = snakeBody[0] + direction;

            // 1. WANDKOLLISION PRÜFEN
            if (newHead.x < 0.f || newHead.x >= 800.f || newHead.y < 0.f || newHead.y >= 600.f) {
                resetGame(snakeBody, direction);
                nextDirection = direction;
                spawnFood();
                elapsedV -= sf::seconds(timePerFrame);
                continue; 
            }

            // 2. SELBSTKOLLISION PRÜFEN
            bool selfCollision = false;
            for (size_t i = 0; i < snakeBody.size(); ++i) {
                if (newHead == snakeBody[i]) {
                    selfCollision = true;
                    break;
                }
            }

            if (selfCollision) {
                resetGame(snakeBody, direction);
                nextDirection = direction;
                spawnFood();
                elapsedV -= sf::seconds(timePerFrame);
                continue;
            }

            // Vorne anfügen
            snakeBody.insert(snakeBody.begin(), newHead);

            // Kollision mit Futter prüfen
            block.setPosition(newHead);
            if (block.getGlobalBounds().findIntersection(food.getGlobalBounds())) {
                score += 10; // Punkte für Futter
                if (score > highScore) {
                    highScore = score;
                }
                updateText();
                spawnFood(); 
            } else {
                snakeBody.pop_back(); 
            }

            elapsedV -= sf::seconds(timePerFrame);
        }

        // --- ZEICHNEN ---
        window.clear(sf::Color(15, 25, 35));

        // Feines Raster (Grid) zeichnen
        sf::VertexArray gridLines(sf::PrimitiveType::Lines);
        for (float x = 0; x < 800; x += tileSize) {
            sf::Vertex v1; v1.position = {x, 0.f}; v1.color = sf::Color(25, 40, 55); gridLines.append(v1);
            sf::Vertex v2; v2.position = {x, 600.f}; v2.color = sf::Color(25, 40, 55); gridLines.append(v2);
        }
        for (float y = 0; y < 600; y += tileSize) {
            sf::Vertex v1; v1.position = {0.f, y}; v1.color = sf::Color(25, 40, 55); gridLines.append(v1);
            sf::Vertex v2; v2.position = {800.f, y}; v2.color = sf::Color(25, 40, 55); gridLines.append(v2);
        }
        window.draw(gridLines);

        // Futter zeichnen
        window.draw(food);

        // Schlange zeichnen
        for (size_t i = 0; i < snakeBody.size(); ++i) {
            block.setPosition(snakeBody[i]);
            if (i == 0) {
                block.setFillColor(sf::Color::Green);
            } else {
                block.setFillColor(sf::Color(0, 180, 0));
            }
            window.draw(block);
        }

        // Punktezähler & Highscore über dem Spielfeld zeichnen
        window.draw(scoreText);
        window.draw(highScoreText);

        window.display();
    }
    return 0;
}