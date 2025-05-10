#include <iostream>
#include "raylib.h"
#include <ctime>
using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int size = 10;
const int cellSize = 50;
const int mayinSayisi = 20;

bool mayinKutu[size][size];
bool acikKutu[size][size];
bool oyunBitti = false;
float timer = 0.0f;

enum GameState { MENU, OYUN, OYUN_BITTI };
GameState gameState = MENU;


// Yeni oyun başlat
void YeniOyunBaslat() {
    oyunBitti = false;
    timer = 0.0f;

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            mayinKutu[i][j] = false;
            acikKutu[i][j] = false;
        }

    int yerlestirilenMayin = 0;
    while (yerlestirilenMayin < mayinSayisi) {
        int satir = rand() % size;
        int sutun = rand() % size;
        if (!mayinKutu[satir][sutun]) {
            mayinKutu[satir][sutun] = true;
            yerlestirilenMayin++;
        }
    }
}

int main() {
       srand(time(0));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mayin Tarlasi");
    SetTargetFPS(60);

    Rectangle baslaButonu = { SCREEN_WIDTH / 2.0f - 75, SCREEN_HEIGHT / 2.0f, 150, 50 };
    Rectangle yenidenButon = { SCREEN_WIDTH/2 - 75, SCREEN_HEIGHT/2 - 30, 150, 40 };
    Rectangle cikisButon = { SCREEN_WIDTH/2 - 75, SCREEN_HEIGHT/2 + 40, 150, 40 };


     while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameState == MENU) {
            // Ana ekran
            DrawText("Mayin Tarlası", SCREEN_WIDTH / 2 - MeasureText("Mayin Tarlasi", 30) / 2, 150, 30, DARKBLUE);
            DrawRectangleRec(baslaButonu, LIGHTGRAY);
            DrawText("Basla", SCREEN_WIDTH / 2 - MeasureText("Basla", 20) / 2, SCREEN_HEIGHT / 2.0f + 10, 20, BLACK);

            // Tıklama kontrolü
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, baslaButonu)) {
                    YeniOyunBaslat();
                    gameState = OYUN;
                }
            }

        } else if (gameState == OYUN) {
            // Oyun ekranı
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !oyunBitti) {
                int mouseX = GetMouseX();
                int mouseY = GetMouseY();
                int row = mouseY / cellSize;
                int col = mouseX / cellSize;

                if (row >= 0 && row < size && col >= 0 && col < size) {
                    acikKutu[row][col] = true;
                    if (mayinKutu[row][col]) {
                        oyunBitti = true;
                        gameState = OYUN_BITTI;

                        for (int i = 0; i < size; i++)
                            for (int j = 0; j < size; j++)
                                acikKutu[i][j] = true;
                    }
                }
            }

            // Ekran çizimi
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (acikKutu[i][j]) {
                        if (mayinKutu[i][j]) {
                            DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, RED);
                        } else {
                            DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, LIGHTGRAY);
                        }
                    } else {
                        DrawRectangleLines(j * cellSize, i * cellSize, cellSize, cellSize, DARKGRAY);
                    }
                }
            }

        }else if (gameState == OYUN_BITTI) {
            // Oyun bitti ekranı
            DrawText("Mayina Bastiniz!", SCREEN_WIDTH/2 - MeasureText("Mayina Bastiniz!", 30)/2, 150, 30, RED);
            
            // Yeniden başlat butonu
            DrawRectangleRec(yenidenButon, LIGHTGRAY);
            DrawText("Yeniden Baslat", SCREEN_WIDTH/2 - MeasureText("Yeniden Baslat", 20)/2, SCREEN_HEIGHT/2 - 20, 20, BLACK);
            
            // Çıkış butonu
            DrawRectangleRec(cikisButon, LIGHTGRAY);
            DrawText("Cikis", SCREEN_WIDTH/2 - MeasureText("Cikis", 20)/2, SCREEN_HEIGHT/2 + 50, 20, BLACK);

            // Buton kontrolü
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, yenidenButon)) {
                    YeniOyunBaslat();
                    gameState = OYUN;
                } else if (CheckCollisionPointRec(mousePos, cikisButon)) {
                    CloseWindow();
                    return 0;
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
