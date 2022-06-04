#include <raylib.h>
#include <string>
#include <iostream>
using namespace std;

struct AnimData
{
    Rectangle Rec;
    Vector2 Pos;
    int Frame;
    float UpdateTime;
    float RunningTime;
};

bool GroundCheck(AnimData data, int windowHeight)
{
    return data.Pos.y >= windowHeight - data.Rec.height;
}

int main(){

    int WindowDimensions[2]{1080, 720};
    int FreeSpace{0};
    
    InitWindow(WindowDimensions[0], WindowDimensions[1], "dASH gAMe");

    //dimensions
    int velocity{0};
    const int gravity{1000};
    const int jumpVel{-600};
    bool inAIR{false};

    //Obstacles
    Texture2D Crystal = LoadTexture("textures/12_nebula_spritesheet.png");

    /*
    AnimData CrystalData{
                        {0, 0, Crystal.width/8, Crystal.height/8},       //REC
                        {WindowDimensions[0], WindowDimensions[1] - Crystal.height/8},  //POS
                        0,                                               //FRAME
                        1.0/12.0,                                        //UPDATE TIME
                        0                                                //RUNNING TIME
                        };

    */

    const int sizeOfCrystal{10};
    AnimData CrystalsArray[sizeOfCrystal]{};

    for( int i = 0; i <sizeOfCrystal; i++)
    {
        CrystalsArray[i].Rec.x = 0.0;
        CrystalsArray[i].Rec.y = 0.0;
        CrystalsArray[i].Rec.width = Crystal.width/8;
        CrystalsArray[i].Rec.height = Crystal.height/8;
        CrystalsArray[i].Pos.y = WindowDimensions[1] - Crystal.height/8;
        CrystalsArray[i].Frame = 0;
        CrystalsArray[i].RunningTime = 0.0;
        CrystalsArray[i].UpdateTime = 0.0;
    }

    float finishLine{CrystalsArray[sizeOfCrystal - 1].Pos.x};

    for( int i = 0; i <sizeOfCrystal; i++)
    {
        CrystalsArray[i].Pos.x = WindowDimensions[0] + FreeSpace;
        FreeSpace += 350;
    }
    
    int CrystalVel{-300};

    //Player
    Texture2D Player = LoadTexture("textures/scarfy.png");
    AnimData PLayerData;
    PLayerData.Rec.width = Player.width/6;
    PLayerData.Rec.height = Player.height;
    PLayerData.Rec.x = 0;
    PLayerData.Rec.y = 0;
    PLayerData.Pos.x = WindowDimensions[0]/2 - PLayerData.Rec.width/2;
    PLayerData.Pos.y = WindowDimensions[1] - PLayerData.Rec.height;

    //animation
    PLayerData.Frame = 0;
    PLayerData.UpdateTime = 1.0/12.0;
    PLayerData.RunningTime = 0.0;
    int jumps{0};
    Texture2D BackGroundImage = LoadTexture("textures/far-buildings.png");
    Texture2D BuildingImage = LoadTexture("textures/back-buildings.png");
    Texture2D ForeGroundImage = LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};

    bool collisionPlayer{false};
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float dT{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);
    
        bgX -= 20* dT;
        mgX -= 40* dT;
        fgX -= 80* dT;

        if( bgX <= -BackGroundImage.width * 2)
        {
            bgX = 0.0;
        }

        if( mgX <= -BuildingImage.width * 2)
        {
            mgX = 0.0;
        }

        if( fgX <= -ForeGroundImage.width * 2)
        {
            fgX = 0.0;
        }


        //env
        Vector2 bgPos1{bgX, 0.0};
        Vector2 bgPos2{bgX + BackGroundImage.width * 2, 0.0};
        DrawTextureEx( BackGroundImage, bgPos1, 0.0, 4.2, WHITE);
        DrawTextureEx( BackGroundImage, bgPos2, 0.0, 4.2, WHITE);
        Vector2 mgPos1{mgX, 0.0};
        Vector2 mgPos2{mgX + BuildingImage.width * 2, 0.0};
        DrawTextureEx( BuildingImage, mgPos1, 0.0, 4.2, WHITE);
        DrawTextureEx( BuildingImage, mgPos2, 0.0, 4.2, WHITE);
        Vector2 fgPos1{fgX, 0.0};
        Vector2 fgPos2{fgX + ForeGroundImage.width * 2, 0.0};
        DrawTextureEx( ForeGroundImage, fgPos1, 0.0, 4, WHITE);
        DrawTextureEx( ForeGroundImage, fgPos2, 0.0, 4, WHITE);

        //ground check
        if (GroundCheck(PLayerData, WindowDimensions[1])){
            velocity = 0;
            inAIR = false;
            jumps = 2;
        }
        else{
            velocity += gravity * dT;
            inAIR = true;
        }
        
        if (IsKeyPressed(KEY_SPACE) && jumps > 0)
        {
            velocity = jumpVel;
            jumps -= 1;
        }
        
        //update posistions
        PLayerData.Pos.y += velocity * dT;
        PLayerData.RunningTime += dT;

        for (int i = 0; i < sizeOfCrystal; i++)
        {
            CrystalsArray[i].Pos.x += CrystalVel * dT;
            CrystalsArray[i].RunningTime += dT;
        }

        finishLine += CrystalVel * dT;

        if(PLayerData.RunningTime >= PLayerData.UpdateTime && !inAIR)
        {
            PLayerData.RunningTime = 0.0;

            //ani
            PLayerData.Rec.x = PLayerData.Frame * PLayerData.Rec.width;
            PLayerData.Frame++;
            if(PLayerData.Frame > 7)
            {
                PLayerData.Frame = 0;
            }
        }

        for (int i = 0; i < sizeOfCrystal; i++)
        {
            if(CrystalsArray[i].RunningTime >= CrystalsArray[i].UpdateTime)
            {
                CrystalsArray[i].RunningTime = 0.0;
                CrystalsArray[i].Rec.x = CrystalsArray[i].Frame * CrystalsArray[i].Rec.width;
                //cout << CrystalsArray[0].Rec.x; 
                CrystalsArray[i].Frame++;

                if(CrystalsArray[i].Frame > 7)
                {
                CrystalsArray[i].Frame = 0;
                }
            }
        }

        
        for(AnimData Crystal : CrystalsArray)
        {
            float pad{40};
            Rectangle crystalRec{
                Crystal.Pos.x + pad,
                Crystal.Pos.y + pad,
                Crystal.Rec.width - 2 * pad,
                Crystal.Rec.height - 2 * pad
            };
            Rectangle playerRec{
                PLayerData.Pos.x,
                PLayerData.Pos.y,
                PLayerData.Rec.width,
                PLayerData.Rec.height
            };
            if(CheckCollisionRecs(crystalRec, playerRec))
            {
                collisionPlayer = true;
            }
        }

        if(collisionPlayer)
        {
            DrawText("Game Over", WindowDimensions[0]/4, WindowDimensions[1]/2, 40, GREEN);
        }
        else if(PLayerData.Pos.x <= finishLine)
        {
            DrawText("Level clear", WindowDimensions[0]/4, WindowDimensions[1]/2, 40, GREEN);
        }
        else
        {
            for (int i = 0; i < sizeOfCrystal; i++)
            {
                DrawTextureRec(Crystal,CrystalsArray[i].Rec, CrystalsArray[i].Pos, BLUE);
            }
       
            DrawTextureRec(Player, PLayerData.Rec, PLayerData.Pos, WHITE);
        }

        
        EndDrawing();
    }

    UnloadTexture(Player);
    UnloadTexture(Crystal);
    UnloadTexture(BackGroundImage);
    UnloadTexture(BuildingImage);
    UnloadTexture(ForeGroundImage);
    CloseWindow();
}