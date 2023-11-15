#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <thread>
#include <chrono>


using namespace std;

struct Position {
    int x;
    int y;
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class SnakeGame {
private:
    const int width = 40;
    const int height = 20;
    bool gameOver;
    Position apple;
    Position head;
    vector<Position> snake;
    Direction dir;
    int score;

public:
    SnakeGame() {
        gameOver = false;
        head = { width / 2, height / 2 };
        apple = GenerateApple();
        dir = RIGHT;
        score = 0;
        snake.push_back(head);
    }

    Position GenerateApple() {
        Position newApple;
        newApple.x = rand() % width;
        newApple.y = rand() % height;
        return newApple;
    }

    void Draw() {
        // Clear the console output
        cout << "\033[2J\033[H";

        for (int i = 0; i < width + 2; i++) {
            cout << "#";
        }
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) {
                    cout << "#";
                }
                if (i == head.y && j == head.x) {
                    cout << "O";
                }
                else if (i == apple.y && j == apple.x) {
                    cout << "A";
                }
                else {
                    bool isBodyPart = false;
                    for (int k = 1; k < snake.size(); k++) {
                        if (snake[k].x == j && snake[k].y == i) {
                            cout << "o";
                            isBodyPart = true;
                            break;
                        }
                    }
                    if (!isBodyPart) {
                        cout << " ";
                    }
                }
                if (j == width - 1) {
                    cout << "#";
                }
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) {
            cout << "#";
        }
        cout << endl;

        cout << "Score: " << score << endl;
    }


    void Input() {
        string input;
        if (cin >> input) {
            if (input == "w" && dir != DOWN) {
                dir = UP;
            }
            else if (input == "s" && dir != UP) {
                dir = DOWN;
            }
            else if (input == "a" && dir != RIGHT) {
                dir = LEFT;
            }
            else if (input == "d" && dir != LEFT) {
                dir = RIGHT;
            }
            else if (input == "x") {
                gameOver = true;
            }
        }
    }

    void Logic() {
        Position prevTail = snake.empty() ? Position{ 0, 0 } : snake.back();
        Position prevHead = head;
        switch (dir) {
        case UP:
            head.y--;
            break;
        case DOWN:
            head.y++;
            break;
        case LEFT:
            head.x--;
            break;
        case RIGHT:
            head.x++;
            break;
        }

        if (head.x >= width || head.x < 0 || head.y >= height || head.y < 0) {
            gameOver = true;  // Snake collided with the wall
            return;
        }

        if (head.x == apple.x && head.y == apple.y) {
            score++;
            apple = GenerateApple();
        }
        else {
            if (!snake.empty()) {
                snake.pop_back();
            }
        }

        for (int i = 1; i < snake.size(); i++) {
            if (head.x == snake[i].x && head.y == snake[i].y) {
                gameOver = true;
                break;
            }
        }

        snake.insert(snake.begin(), head);
        if (prevTail.x != 0 || prevTail.y != 0) {
            snake.push_back(prevTail);
        }
    }

    void Run() {
        while (!gameOver) {
            Draw();
            Input();
            if (!gameOver) {
                Logic();
            }
            this_thread::sleep_for(chrono::milliseconds(400));
        }
    }
};

int main() {
        srand(static_cast<unsigned>(time(nullptr)));

        SnakeGame game;
        game.Run();

        return 0;
    }