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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mayın Tarlası");
    SetTargetFPS(60);

    YeniOyunBaslat();

    while (!WindowShouldClose()) {
        // Tıklama işlemi
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !oyunBitti) {
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();
            int row = mouseY / cellSize;
            int col = mouseX / cellSize;

            if (row >= 0 && row < size && col >= 0 && col < size) {
                acikKutu[row][col] = true;
                if (mayinKutu[row][col]) {
                    oyunBitti = true;

                    // Tüm kutuları aç
                    for (int i = 0; i < size; i++)
                        for (int j = 0; j < size; j++)
                            acikKutu[i][j] = true;
                }
            }
        }

        // Ekran çizimi
        BeginDrawing();
        ClearBackground(RAYWHITE);

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

        // Mayına basıldıysa mesaj ve zamanlayıcı
        if (oyunBitti) {
            DrawText("Mayina bastin! 2 saniye sonra yeniden...", 40, SCREEN_HEIGHT - 30, 20, RED);
            timer += GetFrameTime();
            if (timer >= 2.0f) {
                YeniOyunBaslat();
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
