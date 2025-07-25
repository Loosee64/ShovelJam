#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include "Supply.h"
#include "Building.h"

#include "Button.h"

#include "Bunks.h"
#include "Barracks.h"
#include "SupplyShed.h"
#include "Saloon.h"
#include "SleepingBag.h"

#include <vector>
#include <string>

enum class DAYCYCLE {DAY, NIGHT};
enum class GameState {TITLE, CREDITS, GAMEPLAY, GAMEOVER, ENDOFDAY, TUTORIAL};

class Game
{
public:
    Game();
    ~Game();
    void init();
    void reset();
    void draw();
    void update();

    void titleUpdate();
    void titleDraw();

    void creditsDraw();
    void creditsUpdate();

    void gameplayUpdate();
    void gameplayDraw();

    void gameOverDraw();
    void gameOverUpdate();

    void endOfDayDraw();
    void endOfDayUpdate();

    void tutorialDraw();
    void tutorialUpdate();

    void screenScrolling();

    void collisionCheck();

    void bunksInteract(Building& t_bunks);
    void buildingInteract(Building& t_building);

    Vector2 findNearestTarget(Enemy& t_enemy, std::vector<std::shared_ptr<NPC>>& t_targets);

    void enemySpawning(int t_num);
    void waveSpawning();
    void swarmSpawning();
    void supplySpawning(int t_amount);
    void fieldNPCSpawning();
    NPC generateNPC();

    void findNPCTarget();
    void moveCell();

    void dayCycle(bool t_forcedChange);
    void dailySupplies();
    void raidStarting(bool t_trigger);
    void checkBuildingStatus();
    void buildingProgress(Building& t_building);
    void disableSleepingBag();

private:
    GameState m_gameState;
    Vector2 m_scrolling1;
    Vector2 m_scrolling2;

    std::vector<std::shared_ptr<Button>> buttons;

    int m_dailySuppliesGained;
    int m_dailySuppliesConsumed;
    int m_dailyVillagers;
    int m_dailyHappiness;

    int m_happiness;

    static const int MAX_ENEMIES = 10;
    static const int MAX_NPCS = 50;
    static const int MAX_PARTY = 3;
    static const int MAX_SUPPLIES = 1;
    static const int MAX_BUILDINGS = 5;

    int m_effectiveMaxBuildings = MAX_BUILDINGS;

    int m_lastName;
    int m_lastSprite;

    Player player;
    Enemy enemies[MAX_ENEMIES];
    int enemyKill;
    float wavedt;
    bool waveComplete;
    bool canInteract;
    std::vector<std::shared_ptr<NPC>> party;
    std::vector<std::shared_ptr<NPC>> village;
    std::vector < std::shared_ptr<NPC>> barracks;
    std::shared_ptr<NPC> m_fieldNPC;
    int m_numInVillage;

    std::vector<std::shared_ptr<Supply>> supplies;
    std::vector<std::shared_ptr<Building>> buildings;

    Vector2 m_targetsArray[MAX_ENEMIES];
    int m_numTargets;

    Cell currentCell;

    Rectangle m_healthBar;
    Rectangle m_healthOutline;

    float dt;
    float timedt;
    const float DAY_LENGTH = 60.0f;

    DAYCYCLE time;
    int m_dayCount;
    std::string m_dayText;
    std::string m_timeText;
    const int RAID_DAY = 7;

    Rectangle m_swarmArea;
    Rectangle m_waveSource;
    Cell swarmSpot;
    Cell m_lastSwarm;

    Rectangle m_backSource;
    Rectangle m_backArea;
    Texture2D m_backToVillage;

    bool m_raid;

    Texture2D m_ground;
    Texture2D m_waveArrow;

    bool m_interactText;

    Texture2D m_logo;

    Music m_music;
};

#endif // GAME_H
