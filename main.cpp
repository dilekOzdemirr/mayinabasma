#include <iostream>
#include "raylib.h"
#include <ctime>
Texture2D karakterSprite;
Rectangle spriteKare;
Sound patlamaSes;  

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

// Oyuncu değişkenleri
Vector2 oyuncuPozisyon = {0, size - 1};  // Sol alt köşe
int can = 5;
bool oyuncuKazandi = false;


// Yeni oyun başlat
void YeniOyunBaslat() {
   oyunBitti = false;
    timer = 0.0f;
    can = 5;
    oyuncuKazandi = false;
    oyuncuPozisyon = {0, size - 1};

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
    InitAudioDevice();  

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mayin Tarlasi");
    SetTargetFPS(60);
    karakterSprite = LoadTexture("george.png");
    patlamaSes = LoadSound("SFX_Explosion_01.wav");



     
    spriteKare = { 0, 0, (float)(karakterSprite.width / 3), (float)(karakterSprite.height / 4) };

    Rectangle baslaButonu = { SCREEN_WIDTH / 2.0f - 75, SCREEN_HEIGHT / 2.0f, 150, 50 };
    Rectangle yenidenButon = { SCREEN_WIDTH/2 - 75, SCREEN_HEIGHT/2 - 30, 150, 40 };
    Rectangle cikisButon = { SCREEN_WIDTH/2 - 75, SCREEN_HEIGHT/2 + 40, 150, 40 };


     while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameState == MENU) {
            // Ana ekran
            DrawText("Mayin Tarlasi", SCREEN_WIDTH / 2 - MeasureText("Mayin Tarlasi", 30) / 2, 150, 30, DARKBLUE);
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
            // Oyuncu hareketi ve mayın kontrolü
            if (!oyunBitti && !oyuncuKazandi) {
                if (IsKeyPressed(KEY_UP) && oyuncuPozisyon.y > 0)
                    oyuncuPozisyon.y--;
                else if (IsKeyPressed(KEY_DOWN) && oyuncuPozisyon.y < size - 1)
                    oyuncuPozisyon.y++;
                else if (IsKeyPressed(KEY_LEFT) && oyuncuPozisyon.x > 0)
                    oyuncuPozisyon.x--;
                else if (IsKeyPressed(KEY_RIGHT) && oyuncuPozisyon.x < size - 1)
                    oyuncuPozisyon.x++;

                int row = (int)oyuncuPozisyon.y;
                int col = (int)oyuncuPozisyon.x;

                acikKutu[row][col] = true;

                if (mayinKutu[row][col]) {
                    mayinKutu[row][col] = false;
                    can--;
                    PlaySound(patlamaSes);
                    if (can <= 0) {
                        oyunBitti = true;
                        gameState = OYUN_BITTI;
                        for (int i = 0; i < size; i++)
                            for (int j = 0; j < size; j++)
                                acikKutu[i][j] = true;
                    }
                }
                
                if (oyuncuPozisyon.x == size - 1 && oyuncuPozisyon.y == 0) {
                    oyuncuKazandi = true;
                    gameState = OYUN_BITTI;
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

           //
            DrawTextureRec(karakterSprite, spriteKare, { oyuncuPozisyon.x * cellSize, oyuncuPozisyon.y * cellSize }, WHITE);


            // Can bilgisi
            DrawText(TextFormat("Can: %d", can), 10, 10, 20, BLACK);






        }else if (gameState == OYUN_BITTI) {
           if (oyuncuKazandi) {
                DrawText("Tebrikler, Kazandiniz!", SCREEN_WIDTH/2 - MeasureText("Tebrikler, Kazandiniz!", 30)/2, 100, 30, DARKGREEN);
            } else {
                DrawText("CAN BITTI !", SCREEN_WIDTH/2 - MeasureText("Mayina Bastiniz!", 30)/2, 100, 30, RED);
            }

            DrawRectangleRec(yenidenButon, LIGHTGRAY);
            DrawText("Yeniden Baslat", SCREEN_WIDTH/2 - MeasureText("Yeniden Baslat", 20)/2, SCREEN_HEIGHT/2 - 20, 20, BLACK);

            DrawRectangleRec(cikisButon, LIGHTGRAY);
            DrawText("Cikis", SCREEN_WIDTH/2 - MeasureText("Cikis", 20)/2, SCREEN_HEIGHT/2 + 50, 20, BLACK);

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
