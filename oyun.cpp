#include <cstdlib>
#include "raylib.h"
#include <vector>

using namespace std;


const int GENISLIK = 450;
const int YUKSEKLIK = 800;

int skor = 0;
char text[80];

class GameObject {
    public:
    virtual void update() = 0;
    virtual void draw() = 0;
    bool active=true;
    int x,y;
};

class Mermi : public GameObject {
    public:
    Mermi(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void update() {
        if (active)
            y -= 5;
        if(y < 0)
            active = false;
    }

    void draw() {
        DrawRectangle(x, y, 3, 8, WHITE);
    }
};

vector<Mermi> mermiler;

class Ucak : public GameObject {
    public:
    Ucak() {
        x = (GENISLIK-30) / 2;
        y = YUKSEKLIK-50;
    }
    void update() {
        if (IsKeyDown(KEY_LEFT)) {
            x -= 5;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            x += 5;
        }
        if (x <= 0)
            x = 0;
        if(x>GENISLIK-30)
            x=GENISLIK-30;

        if(IsKeyPressed(KEY_SPACE)) {
            Mermi m(x+14,y);
            mermiler.push_back(m);
        }
    }

    void draw() {
        DrawRectangle(x, y, 30, 50, YELLOW);
    }
};

class Dusman : public GameObject {
    public:
    Dusman() {
        x=rand()%GENISLIK;
        y=-50;
    }
    void update() {
        if (active)
            y += 5;
        if (y > YUKSEKLIK)
            active = false;
    }

    void draw() {
        if(active)
            DrawRectangle(x, y, 30, 50, PURPLE);
    }
};


vector<Dusman> dusmanlar;
Ucak ucak;


void dusmanOlustur() {
    if(rand()%100 == 0) {
        Dusman d;
        dusmanlar.push_back(d);
    }
}

void oyunuSifirla() {
    skor = 0;
    dusmanlar.clear();
    mermiler.clear();
}

void carpismaAlgila() {
    // Düşman-mermi etkileşimi
    for (size_t i = 0; i < dusmanlar.size(); i++)
    {
        if(dusmanlar[i].active) {
            for (size_t j = 0; j < mermiler.size(); j++)
            {
                if (mermiler[j].active)
                {
                    Rectangle r1 = {dusmanlar[i].x, dusmanlar[i].y, 30, 50};
                    Rectangle r2 = {mermiler[j].x, mermiler[j].y, 3, 5};
                    if (CheckCollisionRecs(r1, r2)) {
                        dusmanlar[i].active = false;
                        mermiler[j].active = false;
                        skor++;
                    }
                }
                
            }
            
        }
    }
    //Düşman-uçak etkileşimi
    for (size_t i = 0; i < dusmanlar.size(); i++)
    {
        if(dusmanlar[i].active) {
            Rectangle r1 = {dusmanlar[i].x, dusmanlar[i].y, 30, 50};
            Rectangle r2 = {ucak.x, ucak.y, 30, 50};
            if(CheckCollisionRecs(r1, r2)) {
                oyunuSifirla();
            }
        }
    }
}

void vectorTemizle() {
    for(auto it=mermiler.begin(); it != mermiler.end();) {
        if(!it->active)
             it = mermiler.erase(it);
        else
            ++it;
    }
    for(auto it=dusmanlar.begin(); it != dusmanlar.end();) {
        if(!it->active)
             it = dusmanlar.erase(it);
        else
            ++it;
    }
}

int main()
{
    InitWindow(GENISLIK, YUKSEKLIK, "Uçak Oyunu");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        dusmanOlustur();
        ucak.update();
        for(size_t i=0;i<dusmanlar.size();i++) {
            dusmanlar[i].update();
        }
        for (size_t i = 0; i < mermiler.size(); i++) {
            mermiler[i].update();
        }
        carpismaAlgila();
        vectorTemizle();
        BeginDrawing();
        ClearBackground(BLUE);
        DrawText(TextFormat("Skor: %d", skor), 0, 0, 20, YELLOW);
        ucak.draw();
        for(size_t i=0;i<dusmanlar.size();i++) {
            if(dusmanlar[i].active)
                dusmanlar[i].draw();
        }
        for (size_t i = 0; i < mermiler.size(); i++) {
            if(mermiler[i].active)
                mermiler[i].draw();
        }
        EndDrawing();
    }
    

    CloseWindow();
    return 0;
}
