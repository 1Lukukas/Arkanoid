// Arkanoid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include <thread>
#include <chrono>

class Boxes {
private:
    std::vector <std::string>& map;
    std::vector <int> boxCoord;
    std::vector <std::pair<int, int>> storedCoords{ 0 };
    int boxCount = 0;
public:
    Boxes(std::vector <std::string>& map_, std::vector <int> boxCoord_) : map(map_), boxCoord(boxCoord_) {
        drawBoxes();
    }

    void drawBoxes() {
        for (int i = 0; i < boxCoord.size() - 1; i += 2) {
           
            int x = boxCoord[i];
            int y = boxCoord[i + 1];
            //storeBoxCoordinates(x, y);
            drawBox(x,y);
            
        }
    }

    /*void storeBoxCoordinates(int x, int y) {
        std::pair<int, int> temp = std::make_pair(x, y);
        storedCoords.push_back(temp);
    }*/

    void drawBox(int x, int y) {
        map[y][x] = '[';
        map[y][x + 1] = ']';
        ++boxCount;
    }
    
    void deleteBox(int x, int y) {
        map[y][x] = ' ';
        map[y][x + 1] = ' ';
        --boxCount;
    }

    bool clearedBoxes() {
        if (boxCount > 0) { return false; }
        else return true;
    }
};

class Platform {
    
private:
    std::vector <std::string> &map;
    int xLeft, xRight, y;
    
public:
    Platform(std::vector <std::string>& map_, int xLeft_, int xRight_, int y_) : map(map_), xLeft(xLeft_), xRight(xRight_), y(y_){}

    void moveLeft() {
        if (xLeft >= 2) {
            --xLeft;
            --xRight;
        }
    }

    void moveRight() {
        if (xRight <= 25) {
            ++xLeft;
            ++xRight;
        }
    }

    void drawPlatform() {
        map[y] = "#                          #";
        map[y][xLeft] = '=';
        map[y][xLeft+1] = '=';
        map[y][xLeft+2] = '=';
        map[y][xLeft+3] = '=';
    }

    int getxLeft() {
        return xLeft;
    }
    int getxRight() {
        return xLeft;
    }
};

class Ball {
private:
    std::vector <std::string>& map;
    int x, y, yTrajectory, xTrajectory;
    Boxes& boxes;
    Platform& platform;
public:
    Ball(std::vector <std::string>& map_, int x_, int y_, Boxes& boxes_, Platform& platform_) : map(map_), x(x_), y(y_), boxes(boxes_), platform(platform_) {}

    void checkBallCollision() {
        int tempy = y + yTrajectory;
        int tempx = x + xTrajectory;

        if ((map[tempy][x] != ' ') && (map[tempy][tempx] != ' ') && (map[y][tempx] != ' ')) {
            setBallTrajectory(yTrajectory * -1, xTrajectory * -1);
        }
        else if (map[tempy][x] != ' ') {
            setBallTrajectory(yTrajectory * -1, xTrajectory);
        }
        else if (map[y][tempx] != ' ') {
            setBallTrajectory(yTrajectory, xTrajectory * -1);
        }

        else if (map[tempy][tempx] != ' ') {
            setBallTrajectory(yTrajectory, xTrajectory * -1);
        }

        if (map[tempy][tempx] == '[') {
            boxes.deleteBox(tempx, tempy);
        }

        if (map[y][tempx] == '[') {
            boxes.deleteBox(tempx, y);
        }

        if (map[tempy][x] == '[') {
            boxes.deleteBox(x, tempy);
        }
        if (map[tempy][tempx] == ']') {
            boxes.deleteBox(tempx - 1, tempy);
        }
        if (map[y][tempx] == ']') {
            boxes.deleteBox(tempx - 1, y);
        }

        if (map[tempy][x] == ']') {
            boxes.deleteBox(x - 1, tempy);
        }

        if (map[tempy][tempx] == '=') {
            if (tempx == platform.getxLeft()|| tempx == platform.getxLeft()+1) {
                setBallTrajectory(-1, -1);
            }
            else { setBallTrajectory(-1, 1); }
        }

    }

    void moveBall() {
        map[y][x] = ' ';
        checkBallCollision();
        y = y + yTrajectory;
        x = x + xTrajectory;
    }

    void drawBall() {
        map[y][x] = 'O';
    }

    void deleteBall() {
        map[y][x] = ' ';
    }

    int getY() {
        return y;
    }

    void setBallTrajectory(int updown, int leftright) {
        yTrajectory = updown;
        xTrajectory = leftright;
    }
};

class Game {
private:
    std::vector <std::string> map;
public:
    Game(std::vector <std::string> map_) : map(map_){}

    void drawMap() {
        for (int i = 0; i < 31; i++) {
            std::cout << "      " << map[i] << '\n';
        }
    }

    void playGame() {
        HWND console = GetConsoleWindow();
        RECT r;
        bool gameOver = false;
        bool gameStart = true;
        GetWindowRect(console, &r);

        MoveWindow(console, r.left, r.top, 350, 600, TRUE);

        Platform platform(map, 11, 14, 28);
        Boxes boxes(map,
            { 1,4,3,4,5,4,7,4,9,4,11,4,13,4,15,4,17,4,19,4,21,4,23,4,25,4,
             1,5,3,5,5,5,7,5,9,5,11,5,13,5,15,5,17,5,19,5,21,5,23,5,25,5,
             1,6,3,6,5,6,7,6,9,6,11,6,13,6,15,6,17,6,19,6,21,6,23,6,25,6,
             1,7,3,7,5,7,7,7,9,7,11,7,13,7,15,7,17,7,19,7,21,7,23,7,25,7,
             1,8,3,8,5,8,7,8,9,8,11,8,13,8,15,8,17,8,19,8,21,8,23,8,25,8,
             1,9,3,9,5,9,7,9,9,9,11,9,13,9,15,9,17,9,19,9,21,9,23,9,25,9
            });
        Ball ball(map, 13, 27, boxes, platform);

        while (gameOver == false) {

            COORD cursorPosition;	cursorPosition.X = 0;	cursorPosition.Y = 0;	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
            std::this_thread::sleep_for(std::chrono::milliseconds(75));
            platform.drawPlatform();
            if (gameStart == false) ball.moveBall();
            ball.drawBall();
            if (ball.getY() == 29) { gameOver = true; ball.deleteBall(); }
            for (int i = 0; i < 31; i++) {
                std::cout << "      " << map[i] << '\n';
            }
            if (GetAsyncKeyState(VK_LEFT)) {
                if (gameStart == true) {
                    ball.setBallTrajectory(-1, -1);
                    gameStart = false;
                }
                platform.moveLeft();
                platform.drawPlatform();
            }
            if (GetAsyncKeyState(VK_RIGHT)) {
                if (gameStart == true) {
                    ball.setBallTrajectory(-1, 1);
                    gameStart = false;
                }
                platform.moveRight();
                platform.drawPlatform();
            }
        }

    }

};

int main()
{
    std::vector <std::string> map{
    "############################",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "#                          #",
    "############################"
    };


    
    Game newGame(map);
    while (true) {
        newGame.playGame();
        std::cout << "Press R to restart, press anything else to quit";
        system("pause");
        
        if(!GetAsyncKeyState(0x52)){return 0; }
    }
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
