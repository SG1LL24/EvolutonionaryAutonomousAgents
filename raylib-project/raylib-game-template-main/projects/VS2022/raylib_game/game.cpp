#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "player.h"

#include <iostream>

using namespace std;

struct AutoAgents
{
    Color luffyTint = ColorBrightness(BLUE, -1.0f);

    int fruitsEaten = 0;
    int typeAgent;

    float angle = 0;
    float desiredAngle;
    float loseRangeTime;
    float loseRangeThreshold = 3.0f;
    float brightness = -1.0f;
    float circleRadius = 40.0f;

    Vector2 target;

    bool loseRangeCheck;
    bool shouldDraw = true;

    Player luffy = Player(rand() % 1551 + 50, rand() % 901 + 30);
} agents[5];

struct DevilFruit
{
    int xPos, yPos, radius;
    Color tint;
} fruits[100];

int main(void)
{
    InitWindow(1600, 950, "Mugiwara no Luffy");
    SetTargetFPS(30);

    Image luffy = LoadImage("C:\\Users\\Simarbir G\\Documents\\raylib-project\\raylib-game-template-main\\projects\\VS2022\\raylib_game\\resources\\monkey-d-luffy-removebg-preview.png");
    Texture2D luffyTexture = LoadTextureFromImage(luffy);
    UnloadImage(luffy);

    //Image ship = LoadImage("\"C:\\Users\\Simarbir G\\Documents\\raylib-project\\raylib-game-template-main\\projects\\VS2022\\raylib_game\\resources\\sunny-removebg-preview-removebg-preview.png\"");
    Image ship = LoadImage("resources/ship-removebg-preview.png");
    Texture2D shipTexture = LoadTextureFromImage(ship);
    UnloadImage(ship);

    for(int i = 0; i < 10; i++)
        if(i < 5)
            agents[i].typeAgent = 1;
        // else
            // agents[i].typeAgent = 2;

    float time = 30.0f;
    float changeLocTime = 3.0f;

    int chosenRandomNumX;
    int chosenRandomNumY;
    int indexRemove = -1;
    int oddsEvolving = 0;

    bool ranOnce = false;

    while (!WindowShouldClose())
    {
        if(time >= 30.0f)
        {
            for(int i = 0; i < size(agents); i++)
            {
                for(int j = i+1; j < size(agents); j++)
                {
                    if(agents[i].fruitsEaten > agents[j].fruitsEaten)
                    {
                        AutoAgents temp = agents[i];
                        agents[i] = agents[j];
                        agents[j] = temp;
                    }
                }
            }

            //for(int i = 0; i < size(agents); i++)
            //    cout << to_string(agents[i].fruitsEaten) + "\n";

            for(int i = 0; i < size(fruits); i++)
            {
                fruits[i].xPos = rand() % 1551 + 50;
                fruits[i].yPos = rand() % 901 + 30;
                fruits[i].radius = 5.0f;
                fruits[i].tint = PURPLE;
            }

            for(int i = 0; i < size(agents); i++)
            {
                chosenRandomNumX = rand() % 1551 + 50;
                chosenRandomNumY = rand() % 901 + 30;
                agents[i].target.x = (float)chosenRandomNumX;
                agents[i].target.y = (float)chosenRandomNumY;

                if(i == indexRemove && ranOnce)
                    agents[i].shouldDraw = false;

                agents[i].fruitsEaten = 0;
                agents[i].brightness = -1.0f;
                agents[i].luffyTint = ColorBrightness(BLUE, agents[i].brightness);
            }

            indexRemove++;

            for(int i = size(agents) - 1; i >= 0; i--)
            {
                int randNum = rand() % 5 + 1;

                if(randNum > oddsEvolving && ranOnce)
                {
                    agents[i].luffy.boostMaxSpeed(0.4f);
                    agents[i].circleRadius += 5.0f;
                    agents[i].loseRangeThreshold -= 0.5f;
                }

                oddsEvolving++;
            }

            time = 0;
            oddsEvolving = 0;
        }

        BeginDrawing();

        time += GetFrameTime();
        changeLocTime += GetFrameTime();

        for(int i = 0; i < size(agents); i++)
        {
            // Check each time if type 1
            if(agents[i].loseRangeCheck)
                agents[i].loseRangeTime += GetFrameTime();
        }

        ClearBackground(GREEN);

        for(int i = 0; i < size(agents); i++)
        {
            agents[i].desiredAngle = atan2(agents[i].luffy.velocity.y, agents[i].luffy.velocity.x);
            agents[i].angle = Lerp(agents[i].angle, agents[i].desiredAngle, 0.1f);
        }

        if(time < 30.0f)
        {
            ranOnce = true;

            for(int i = 0; i < size(fruits); i++)
                DrawCircle(fruits[i].xPos, fruits[i].yPos, fruits[i].radius, fruits[i].tint);

            for(int i = 0; i < size(agents); i++)
            {
                if(agents[i].shouldDraw /* && typeAgent == 1 */)
                {
                    DrawCircle((int)agents[i].luffy.position.x, (int)agents[i].luffy.position.y, agents[i].circleRadius, agents[i].luffyTint);
                    DrawTexturePro(luffyTexture, Rectangle({0,0,(float)(luffyTexture.width),(float)(luffyTexture.height)}),
                                   Rectangle({agents[i].luffy.position.x, agents[i].luffy.position.y, (float)(luffyTexture.width), (float)(luffyTexture.height)}),
                                   Vector2({(float)(luffyTexture.width / 2), (float)(luffyTexture.height / 2)}), (int)(agents[i].angle * (180 / PI)) % 360, WHITE);
                }
                /*
                else if(agents[i].shouldDraw && typeAgent == 2)
                 {
                    How you draw your agent with its circles around it
                 }
                 */
            }

            for(int i = 0; i < size(agents); i++)
            {
                agents[i].luffy.findTarget(agents[i].target);
                agents[i].luffy.movePlayer();
            }

            for(int i = 0; i < size(fruits); i++)
                for(int j = 0; j < size(agents); j++)
                {
                    if(CheckCollisionCircles(Vector2({agents[j].luffy.position.x, agents[j].luffy.position.y}), 40, Vector2({(float)fruits[i].xPos, (float)fruits[i].yPos}), fruits[i].radius) && !agents[j].loseRangeCheck && agents[j].shouldDraw)
                    {
                        fruits[i].tint = BLANK;
                        fruits[i].xPos = 3000;
                        agents[j].luffyTint = BLANK;
                        agents[j].brightness += 0.15f;
                        agents[j].fruitsEaten++;

                        // Only do this next step if it is type 1
                        agents[j].loseRangeCheck = true;
                    }
                }
        }

        if(changeLocTime >= 3.0f)
        {
            for(int i = 0; i < size(agents); i++)
            {
                chosenRandomNumX = rand() % 1551 + 50;
                chosenRandomNumY = rand() % 901 + 30;
                agents[i].target.x = (float)chosenRandomNumX;
                agents[i].target.y = (float)chosenRandomNumY;
            }

            changeLocTime = 0;
        }

        for(int i = 0; i < size(agents); i++)
        {
            // Check each time if type 1

            if(agents[i].loseRangeTime >= agents[i].loseRangeThreshold)
            {
                agents[i].luffyTint = ColorBrightness(BLUE, agents[i].brightness);
                agents[i].loseRangeCheck = false;
                agents[i].loseRangeTime = 0;
            }
        }

        EndDrawing();
    }

    CloseWindow();
}