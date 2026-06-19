#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

// ================= CONSTANTS =================
const int WIDTH = 800;
const int HEIGHT = 600;

const float GRAVITY = 750.f;
const float JUMP_FORCE = -320.f;

const float PIPE_SPEED = 220.f;
const float PIPE_WIDTH = 90.f;

const float HITBOX_MARGIN = 8.f;

// ================= GAME STATE =================
enum GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER
};

GameState state = MENU;

// ================= DIFFICULTY =================
int difficulty = 1;

float currentGap = 180.f;

float oscillationAmplitude = 0.f;
float oscillationSpeed = 0.f;

string difficultyName = "MEDIUM";

// ================= APPLY DIFFICULTY =================
void applyDifficulty(int level) {

    if (level == 0) {

        currentGap = 230.f;

        oscillationAmplitude = 0.f;
        oscillationSpeed = 0.f;

        difficultyName = "EASY";
    }

    else if (level == 1) {

        currentGap = 180.f;

        oscillationAmplitude = 30.f;
        oscillationSpeed = 2.f;

        difficultyName = "MEDIUM";
    }

    else {

        currentGap = 140.f;

        oscillationAmplitude = 60.f;
        oscillationSpeed = 4.f;

        difficultyName = "HARD";
    }
}

// ================= BIRD CLASS =================
class Bird {

private:

    Sprite sprite;
    float velocity;

public:

    Bird(Texture& tex) {

        sprite.setTexture(tex);
        sprite.setScale(0.5f, 0.5f);

        reset();
    }

    void reset() {

        sprite.setPosition(180, HEIGHT / 2);

        velocity = 0;
    }

    void flap() {

        velocity = JUMP_FORCE;
    }

    void update(float dt) {

        velocity += GRAVITY * dt;

        sprite.move(0, velocity * dt);
    }

    void draw(RenderWindow& window) {

        window.draw(sprite);
    }

    FloatRect getBounds() {

        FloatRect b = sprite.getGlobalBounds();

        b.left += HITBOX_MARGIN;
        b.top += HITBOX_MARGIN;

        b.width -= HITBOX_MARGIN * 2;
        b.height -= HITBOX_MARGIN * 2;

        return b;
    }

    float getY() {

        return sprite.getPosition().y;
    }

    float getX() {

        return sprite.getPosition().x;
    }
};

// ================= PIPE CLASS =================
class Pipe {

public:

    float x;

    float gapY;
    float baseGapY;

    bool passed;

    float phase;
    float amplitude;
    float speed;

    Pipe() {

        x = WIDTH + 100;

        baseGapY = 120 + rand() % (HEIGHT - 240);

        gapY = baseGapY;

        passed = false;

        phase = (rand() % 360) * 3.14159f / 180.f;

        amplitude = oscillationAmplitude;
        speed = oscillationSpeed;
    }

    void update(float dt, float time) {

        x -= PIPE_SPEED * dt;

        if (amplitude > 0) {

            gapY =
                baseGapY +
                sin(time * speed + phase) * amplitude;
        }
    }

    void draw(RenderWindow& window, Texture& pipeTexture) {

        Sprite topPipe(pipeTexture);
        Sprite bottomPipe(pipeTexture);

        float texW = pipeTexture.getSize().x;
        float texH = pipeTexture.getSize().y;

        float topHeight = gapY - currentGap / 2;

        float bottomY = gapY + currentGap / 2;

        float bottomHeight = HEIGHT - bottomY;

        // TOP PIPE
        topPipe.setPosition(x, topHeight);

        topPipe.setScale(
            PIPE_WIDTH / texW,
            -topHeight / texH
        );

        // BOTTOM PIPE
        bottomPipe.setPosition(x, bottomY);

        bottomPipe.setScale(
            PIPE_WIDTH / texW,
            bottomHeight / texH
        );

        window.draw(topPipe);
        window.draw(bottomPipe);
    }

    bool collision(FloatRect birdBounds) {

        float topHeight = gapY - currentGap / 2;

        float bottomY = gapY + currentGap / 2;

        float bottomHeight = HEIGHT - bottomY;

        FloatRect topRect(
            x + HITBOX_MARGIN,
            0,
            PIPE_WIDTH - HITBOX_MARGIN * 2,
            topHeight
        );

        FloatRect bottomRect(
            x + HITBOX_MARGIN,
            bottomY,
            PIPE_WIDTH - HITBOX_MARGIN * 2,
            bottomHeight
        );

        return
            birdBounds.intersects(topRect) ||
            birdBounds.intersects(bottomRect);
    }
};

// ================= MAIN =================
int main() {

    srand(time(0));

    // ================= WINDOW =================
    RenderWindow window(
        VideoMode(WIDTH, HEIGHT),
        "Flappy Bird SFML"
    );

    window.setFramerateLimit(60);

    applyDifficulty(difficulty);

    // ================= LOAD TEXTURES =================
    Texture birdTexture;
    Texture pipeTexture;

    if (!birdTexture.loadFromFile("graphics/bird.png")) {

        cout << "Bird texture failed!\n";

        return -1;
    }

    if (!pipeTexture.loadFromFile("graphics/pipes.png")) {

        cout << "Pipe texture failed!\n";

        return -1;
    }

    // ================= LOAD FONT =================
    Font font;

    if (!font.loadFromFile("graphics/KOMIKAP_.ttf")) {

        cout << "Font failed!\n";

        return -1;
    }

    // ================= OBJECTS =================
    Bird bird(birdTexture);

    vector<Pipe> pipes;

    // ================= SCORE =================
    int score = 0;

    int highScore = 0;

    ifstream readFile("highscore.txt");

    if (readFile.is_open()) {

        readFile >> highScore;

        readFile.close();
    }

    // ================= TEXT =================
    Text scoreText;

    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setPosition(20, 20);

    Text menuText;

    menuText.setFont(font);
    menuText.setCharacterSize(38);
    menuText.setPosition(120, 90);

    Text infoText;

    infoText.setFont(font);
    infoText.setCharacterSize(42);
    infoText.setPosition(140, 220);

    // ================= CLOCKS =================
    Clock deltaClock;
    Clock gameClock;

    float spawnTimer = 0;

    // ================= MAIN LOOP =================
    while (window.isOpen()) {

        float dt = deltaClock.restart().asSeconds();

        float gameTime =
            gameClock.getElapsedTime().asSeconds();

        // ================= EVENTS =================
        Event event;

        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {

                window.close();
            }

            if (event.type == Event::KeyPressed) {

                // ================= MENU =================
                if (state == MENU) {

                    // EASY
                    if (
                        event.key.code == Keyboard::Num1 ||
                        event.key.code == Keyboard::Numpad1
                    ) {

                        difficulty = 0;

                        applyDifficulty(difficulty);
                    }

                    // MEDIUM
                    if (
                        event.key.code == Keyboard::Num2 ||
                        event.key.code == Keyboard::Numpad2
                    ) {

                        difficulty = 1;

                        applyDifficulty(difficulty);
                    }

                    // HARD
                    if (
                        event.key.code == Keyboard::Num3 ||
                        event.key.code == Keyboard::Numpad3
                    ) {

                        difficulty = 2;

                        applyDifficulty(difficulty);
                    }

                    // START GAME
                    if (event.key.code == Keyboard::Enter) {

                        pipes.clear();

                        bird.reset();

                        score = 0;

                        spawnTimer = 0;

                        state = PLAYING;
                    }
                }

                // ================= PLAYING =================
                else if (state == PLAYING) {

                    // FLAP
                    if (event.key.code == Keyboard::Space) {

                        cout << "SPACE PRESSED" << endl;

                        bird.flap();
                    }

                    // PAUSE
                    if (event.key.code == Keyboard::P) {

                        state = PAUSED;
                    }
                }

                // ================= PAUSED =================
                else if (state == PAUSED) {

                    if (event.key.code == Keyboard::P) {

                        state = PLAYING;
                    }
                }

                // ================= GAME OVER =================
                else if (state == GAMEOVER) {

                    if (event.key.code == Keyboard::R) {

                        state = MENU;
                    }
                }
            }
        }

        // ================= UPDATE =================
        if (state == PLAYING) {

            // BIRD
            bird.update(dt);

            // SPAWN PIPES
            spawnTimer += dt;

            if (spawnTimer > 2.f) {

                spawnTimer = 0;

                pipes.push_back(Pipe());
            }

            // UPDATE PIPES
            for (auto& p : pipes) {

                p.update(dt, gameTime);

                // SCORE
                if (!p.passed && p.x < bird.getX()) {

                    p.passed = true;

                    score++;
                }

                // COLLISION
                if (p.collision(bird.getBounds())) {

                    state = GAMEOVER;
                }
            }

            // TOP/BOTTOM COLLISION
            if (
                bird.getY() < 0 ||
                bird.getY() > HEIGHT
            ) {

                state = GAMEOVER;
            }

            // HIGH SCORE
            if (score > highScore) {

                highScore = score;

                ofstream writeFile("highscore.txt");

                if (writeFile.is_open()) {

                    writeFile << highScore;

                    writeFile.close();
                }
            }
        }

        // ================= SCORE TEXT =================
        stringstream ss;

        ss << "Score: "
           << score
           << " High Score: "
           << highScore
           << " "
           << difficultyName;

        scoreText.setString(ss.str());

        // ================= DRAW =================
        window.clear(Color(100, 200, 255));

        // ================= MENU SCREEN =================
        if (state == MENU) {

            menuText.setString(
                "FLAPPY BIRD\n\n"
                "SELECT MODE\n\n"
                "1 - EASY\n"
                "2 - MEDIUM\n"
                "3 - HARD\n\n"
                "CURRENT MODE : " +
                difficultyName +
                "\n\nPRESS ENTER TO START"
            );

            window.draw(menuText);
        }

        // ================= GAME =================
        else {

            // DRAW PIPES
            for (auto& p : pipes) {

                p.draw(window, pipeTexture);
            }

            // DRAW BIRD
            bird.draw(window);

            // DRAW SCORE
            window.draw(scoreText);

            // ================= PAUSED =================
            if (state == PAUSED) {

                infoText.setString(
                    "PAUSED\n\nP - RESUME"
                );

                window.draw(infoText);
            }

            // ================= GAME OVER =================
            if (state == GAMEOVER) {

                infoText.setString(
                    "GAME OVER\n\nR - MENU"
                );

                window.draw(infoText);
            }
        }

        window.display();
    }

    return 0;
}