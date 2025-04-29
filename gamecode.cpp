#include<SFML/Graphics.hpp>
#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
using namespace sf;
// Structure to represent a bullet
struct Bullet {
    sf::RectangleShape shape;

    Bullet(float x, float y) {
        shape.setSize(sf::Vector2f(5.f, 15.f));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);
    }
};


struct Asteroid {
    sf::RectangleShape shape;
    static sf::Texture astTexture;

    Asteroid(float x, float y) {
        if (astTexture.getSize().x == 0) {
            if (!astTexture.loadFromFile("asteroid.png")) {
                std::cerr << "Error loading asteroid texture!" << std::endl;
            }
        }
        shape.setTexture(&astTexture);
        shape.setSize(sf::Vector2f(40, 30));
        shape.setPosition(x, y);
    }
};
// Game state enumeration
enum GameState {
    Playing,
    Paused
};
// Game state
GameState currentState = Playing;

sf::Texture Asteroid::astTexture;

void displayLeaderboard() {
    ifstream inputFile("leaderboard.txt");
    if (!inputFile) {
        cout << "Leaderboard is empty or not found.\n";
        return;
    }

    string name;
    int score;
    int rank = 1;

    cout << "\nTop 5 Leaderboard:\n";
    while (inputFile >> name >> score && rank <= 5) {
        cout << rank << ". " << name << " - " << score << endl;
        rank++;
    }
    cout << " ************** ";
    inputFile.close();
}

// Function to update the leaderboard in the file
void updateLeaderboard(const string& playerName, int score) {
    vector<pair<string, int>> leaderboard;
    string name;
    int currentScore;

    // Read the current leaderboard from the file
    ifstream inputFile("leaderboard.txt");
    while (inputFile >> name >> currentScore) {
        leaderboard.push_back({ name, currentScore });
    }
    inputFile.close();

    // Add the new score to the leaderboard
    leaderboard.push_back({ playerName, score });

    // Sort the leaderboard in descending order of scores
    sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
        });

    // Keep only the top 5 scores
    if (leaderboard.size() > 5) {
        leaderboard.resize(5);
    }

    // Write the updated leaderboard back to the file
    ofstream outputFile("leaderboard.txt");
    for (const auto& entry : leaderboard) {
        outputFile << entry.first << " " << entry.second << endl;
    }
    outputFile.close();
}

// Function to get the player's name using an SFML window
string getPlayerName() {
    sf::RenderWindow window(sf::VideoMode(400, 200), "Enter Your Name");
    sf::Font font;

    // Load a font for displaying text
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error loading font. Ensure 'arial.ttf' is in the same directory.\n";
        return "Player";
    }

    sf::Text prompt("Enter your name:", font, 30);
    prompt.setFillColor(sf::Color::Black);
    prompt.setPosition(20, 20);

    sf::Text inputText("", font, 30);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(20, 70);

    string playerName;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return playerName.empty() ? "Player" : playerName;
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty()) {
                    playerName.pop_back(); // Handle backspace
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                    playerName += static_cast<char>(event.text.unicode); // Add valid characters
                }
                inputText.setString(playerName);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                window.close();
                return playerName.empty() ? "Player" : playerName;
            }
        }

        window.clear(sf::Color::White);
        window.draw(prompt);
        window.draw(inputText);
        window.display();
    }
    return playerName.empty() ? "Player" : playerName;
}

// Function to show the Game Over screen
void showGameOverScreen(int finalScore) {
    sf::RenderWindow gameOverWindow(sf::VideoMode(400, 300), "Game Over");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    // Game Over text
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(100.f, 50.f);

    // Final Score text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString(" Score :" + to_string(finalScore));
    scoreText.setPosition(120.f, 150.f);

    // Instruction to close
    sf::Text instructionText,displayscore;
    instructionText.setFont(font);
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::Yellow);
    instructionText.setString("Press escape to exit");
    instructionText.setPosition(100.f, 220.f);
    // leader board show command
    displayscore.setFont(font);
    displayscore.setCharacterSize(20);
    displayscore.setFillColor(sf::Color::Green);
    displayscore.setString("Press Y to show leader board");
    displayscore.setPosition(100.f, 270.f);

    while (gameOverWindow.isOpen()) {
        sf::Event event;
        while (gameOverWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                gameOverWindow.close();
            }
            if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Y) {
                displayLeaderboard();
            }

        }

        gameOverWindow.clear();
        gameOverWindow.draw(gameOverText);
        gameOverWindow.draw(scoreText);
        gameOverWindow.draw(instructionText);
        gameOverWindow.draw(displayscore);
        gameOverWindow.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SPACE SHOOTER!");
    window.setMouseCursorVisible(false);
    sf::RectangleShape rw, exit, start, cursor, ship;
    sf::Font font;
    if (!font.loadFromFile("seguibli.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }
    sf::Text text, buton;
    text.setFont(font);
    text.setString("     SPACE\n SHOOTER !");
    text.setCharacterSize(54);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(250, 40);

    sf::Texture ba, bc, st, cr, ro;
    ro.loadFromFile("ship.png");
    ship.setTexture(&ro);
    ship.setSize(sf::Vector2f(300, 300));
    ship.setPosition(300, 140);


    //background
    ba.loadFromFile("back.jpg");
    rw.setTexture(&ba);
    rw.setSize(sf::Vector2f(800, 800));
    //exit
    bc.loadFromFile("exit.png");
    exit.setTexture(&bc);
    exit.setSize(sf::Vector2f(100, 50));
    exit.setPosition(sf::Vector2f(350, 500));
    //start
    st.loadFromFile("start.png");
    start.setTexture(&st);
    start.setSize(sf::Vector2f(100, 50));
    start.setPosition(sf::Vector2f(350, 400));
    //cursor
    cr.loadFromFile("cursor.png");
    cursor.setTexture(&cr);
    cursor.setSize(sf::Vector2f(40, 40));
    cursor.setFillColor(sf::Color(sf::Color::Red));

    sf::Mouse mu;
    sf::Vector2i mo;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        mo.y = mu.getPosition().y;
        mo.x = mu.getPosition().x;
        sf::Vector2i wp;
        wp.y = window.getPosition().y;
        wp.x = window.getPosition().x;
        Vector2i sr;
        sr.y = mo.y - wp.y - 45;
        sr.x = mo.x - wp.x - 11;
        //cout << sr.x << "    " << sr.y << endl;
        cursor.setPosition(sr.x - 10, sr.y - 35);
        //startwindow
        if (sr.x > 350 && sr.x < 450 && sr.y>420 && sr.y < 480)
        {
            if (sr.x > 350 && sr.x < 450 && sr.y>420 && sr.y < 480 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                std::srand(static_cast<unsigned>(std::time(0)));

                // Create the window
                sf::RenderWindow window(sf::VideoMode(800, 600), "Plane Shooting Asteroids");
                int lim = 60;
                window.setFramerateLimit(lim);
                sf::RectangleShape back2, plane;
                sf::Texture ba, pl;
                // background
                ba.loadFromFile("back.jpg");
                back2.setTexture(&ba);
                back2.setSize(sf::Vector2f(800, 600));
                // Pause button
                sf::RectangleShape pauseButton(sf::Vector2f(100.f, 50.f));
                pauseButton.setFillColor(sf::Color::Green);
                pauseButton.setPosition(40.f, 40.f);

                sf::Text pauseButtonText;
                pauseButtonText.setFont(font);
                pauseButtonText.setCharacterSize(20);
                pauseButtonText.setFillColor(sf::Color::Black);
                pauseButtonText.setString("Pause");
                pauseButtonText.setPosition(60.f, 50.f);

                //plane
                pl.loadFromFile("plane.png");
                plane.setTexture(&pl);
                plane.setSize(sf::Vector2f(100, 100));
                plane.setPosition(sf::Vector2f(350, 500));

                // Bullets and asteroids
                std::vector<Bullet> bullets;
                std::vector<Asteroid> asteroids;

                // Bullet firing cooldown
                sf::Clock bulletClock;
                float bulletCooldown = 0.2f;

                // Asteroid spawn cooldown
                sf::Clock asteroidClock;
                float asteroidSpawnCooldown = 0.7f;

                // Score and lives tracking
                int score = 0;
                int lives = 3;

                // Fonts for score and lives display
                sf::Font font;
                if (!font.loadFromFile("arial.ttf")) {
                    std::cerr << "Error loading font\n";
                    return -1;
                }

                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(20);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition(10.f, 10.f);

                sf::Text livesText;
                livesText.setFont(font);
                livesText.setCharacterSize(20);
                livesText.setFillColor(sf::Color::White);
                livesText.setPosition(650.f, 10.f);
                bool isPauseButtonClicked = false;
                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }
                    // pause
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        isPauseButtonClicked = true;  // Mark that the button was clicked
                    }

                    if (event.type == sf::Event::MouseButtonReleased && isPauseButtonClicked) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (pauseButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            if (currentState == Playing) {
                                currentState = Paused;
                                pauseButton.setFillColor(sf::Color::Red);
                                pauseButtonText.setString("Play");
                            }
                            else {
                                currentState = Playing;
                                pauseButton.setFillColor(sf::Color::Green);
                                pauseButtonText.setString("Pause");
                            }
                        }
                        isPauseButtonClicked = false;  // Reset the click flag
                    }
                    if (currentState == Playing) {
                        // Plane movement
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && plane.getPosition().x > 0) {
                            plane.move(-6.f, 0.f);
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && plane.getPosition().x + plane.getSize().x < 800) {
                            plane.move(6.f, 0.f);
                        }

                        // Fire bullets
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                            if (bulletClock.getElapsedTime().asSeconds() > bulletCooldown) {
                                bullets.emplace_back(plane.getPosition().x + plane.getSize().x / 2 - 2.5f, plane.getPosition().y);
                                bulletClock.restart();
                            }
                        }

                        // Spawn asteroids
                        if (asteroidClock.getElapsedTime().asSeconds() > asteroidSpawnCooldown) {
                            float x = static_cast<float>(std::rand() % 760); // Random x position
                            asteroids.emplace_back(x, -10.f); // Spawn above the screen
                            asteroidClock.restart();
                        }

                        // Move bullets
                        for (auto& bullet : bullets) {
                            bullet.shape.move(0.f, -10.f);
                        }

                        // Move asteroids
                        for (auto& asteroid : asteroids) {
                            asteroid.shape.move(0.f, 3.f);
                        }

                        // Check for collisions
                        for (size_t i = 0; i < bullets.size();) {
                            bool bulletDestroyed = false;
                            for (size_t j = 0; j < asteroids.size();) {
                                if (bullets[i].shape.getGlobalBounds().intersects(asteroids[j].shape.getGlobalBounds())) {
                                    // Destroy bullet and asteroid, increment score
                                    bullets.erase(bullets.begin() + i);
                                    asteroids.erase(asteroids.begin() + j);
                                    score += 10; // Add 10 points for each collision
                                    lim += 2;
                                    bulletDestroyed = true;
                                    break;
                                }
                                else {
                                    ++j;
                                }
                            }
                            if (!bulletDestroyed) {
                                ++i;
                            }
                        }

                        // Remove bullets that are out of bounds
                        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                            [](const Bullet& bullet) {
                                return bullet.shape.getPosition().y < 0;
                            }),
                            bullets.end());

                        // Check for collisions between the plane and asteroids
                        for (size_t i = 0; i < asteroids.size();) {
                            if (asteroids[i].shape.getGlobalBounds().intersects(plane.getGlobalBounds())) {
                                asteroids.erase(asteroids.begin() + i);
                                lives--;

                                // Check if lives are zero
                                if (lives <= 0) {
                                    window.close(); // Close the main window
                                    string playerName = getPlayerName();
                                    updateLeaderboard(playerName, score);
                                    showGameOverScreen(score);
                                }
                            }
                            else {
                                ++i;
                            }
                        }

                        // Remove asteroids that are out of bounds (without affecting lives)
                        asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
                            [](const Asteroid& asteroid) {
                                return asteroid.shape.getPosition().y > 600; // Out of bounds
                            }),
                            asteroids.end());


                        scoreText.setString(" Score :" + to_string(score));
                        livesText.setString(" Lives left :" + to_string(lives));
                    }   // Render everything
                    window.clear();
                    window.draw(back2);
                    window.draw(plane);
                    for (const auto& bullet : bullets) {
                        window.draw(bullet.shape);
                    }
                    for (const auto& asteroid : asteroids) {
                        window.draw(asteroid.shape);
                    }
                    window.draw(scoreText);
                    window.draw(livesText);
                    window.draw(pauseButton);
                    window.draw(pauseButtonText);
                    window.display();
                }
            }

        }
        else {
            start.setFillColor(sf::Color::White);
        }

        if (sr.x > 350 && sr.x < 450 && sr.y > 520 && sr.y < 580)
        {
            exit.setFillColor(sf::Color(255, 205, 200));
            if (sr.x > 350 && sr.x < 450 && sr.y>520 && sr.y < 580 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                 while (window.pollEvent(event))
                 {
                 sf::Event event;
                    sf::RenderWindow confirmWindow(sf::VideoMode(300, 120), "EXIT SCREEN!");
                    sf::RectangleShape ins;
                    sf::Texture in;

                    // Load the background image
                    if (!in.loadFromFile("inst.png"))
                    {
                        std::cerr << "Error loading image file!" << std::endl;
                    }
                    // load text on exit screen
                    sf::Font font1;
                    if (!font1.loadFromFile("arial.ttf")) {
                        std::cerr << "Error loading font\n";
                        return -1;
                    }
                    sf::Text text1;
                    text1.setFont(font1);
                    text1.setString("Are you want to exit the game!");
                    text1.setCharacterSize(18);
                    text1.setFillColor(sf::Color::Yellow);
                    text1.setPosition(20, 10);


                    ins.setTexture(&in); // Set texture to the rectangle
                    ins.setSize(sf::Vector2f(300, 120));

                    // Create "Yes" and "No" buttons
                    sf::RectangleShape yesButton(sf::Vector2f(60, 30));
                    yesButton.setPosition(50, 60);
                    yesButton.setFillColor(sf::Color::Red);

                    sf::RectangleShape noButton(sf::Vector2f(60, 30));
                    noButton.setPosition(170, 60);
                    noButton.setFillColor(sf::Color::Green);

                    // Create text for "Yes" and "No" buttons
                    sf::Font font;
                    if (!font.loadFromFile("arial.ttf"))
                    {
                        std::cerr << "Error loading font!" << std::endl;
                    }

                    sf::Text yesText("Yes", font, 20);
                    yesText.setPosition(60, 60);
                    yesText.setFillColor(sf::Color::Black);

                    sf::Text noText("No", font, 20);
                    noText.setPosition(190, 60); // Position it in the middle of the button
                    noText.setFillColor(sf::Color::Black);

                    while (confirmWindow.isOpen())
                    {
                        sf::Event confirmEvent;
                        while (confirmWindow.pollEvent(confirmEvent))
                        {
                            if (confirmEvent.type == sf::Event::Closed)
                            {
                                confirmWindow.close();
                            }
                            sf::Vector2i wp;
                            wp.y = window.getPosition().y;
                            wp.x = window.getPosition().x;


                            // Mouse click event
                            if (confirmEvent.type == sf::Event::MouseButtonPressed)
                            {
                                if (confirmEvent.mouseButton.button == sf::Mouse::Left)
                                {
                                    // Check if click is inside the "Yes" button
                                    if (yesButton.getGlobalBounds().contains(confirmEvent.mouseButton.x, confirmEvent.mouseButton.y))
                                    {
                                        std::cout << "User selected Yes. Closing window." << std::endl;
                                        confirmWindow.close();
                                        window.close();
                                    }
                                    // Check if click is inside the "No" button
                                    if (noButton.getGlobalBounds().contains(confirmEvent.mouseButton.x, confirmEvent.mouseButton.y))
                                    {
                                        std::cout << "User selected No. Closing window." << std::endl;
                                        confirmWindow.close();
                                    }
                                }
                            }
                        }

                        confirmWindow.clear();

                        confirmWindow.draw(ins);
                        confirmWindow.draw(text1);
                        confirmWindow.draw(yesButton);
                        confirmWindow.draw(noButton);
                        confirmWindow.draw(yesText);
                        confirmWindow.draw(noText);


                        confirmWindow.display();
                    }
                }
            }
        }

        else {
            exit.setFillColor(sf::Color::White);
        }
        window.clear();
        window.draw(rw);
        window.draw(text);
        window.draw(ship);
        window.draw(start);
        window.draw(buton);
        window.draw(exit);
        window.draw(cursor);
        window.display();
    }

    return 0;
}