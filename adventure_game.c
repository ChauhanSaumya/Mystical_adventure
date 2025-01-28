#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_NAME_LENGTH 50
#define MAX_INVENTORY_SIZE 10
#define MAX_STORY_TEXT 500
#define TOTAL_LOCATIONS 32

//game state structure
typedef struct{
	char playerName[MAX_NAME_LENGTH];
	int health;
	int inventory[MAX_INVENTORY_SIZE];
	int currentLocation;
	int storyProgress;
	int gameOver;
	int score;
}GameState;

//Location and story progression structures
typedef struct{
	int locationId;
	char description[MAX_STORY_TEXT];
	int choices[3];
	int consequences[3];
}Location;

//Fuction prototypes
void initializeGame(GameState *game);
void displayLocation(Location *locations,GameState *game);
int processPlayerChoice(Location *locations, GameState *game, int choice);
void saveGame(GameState *game);
int loadGame(GameState *game);
void displayInventory(GameState *game);
void displayGameStats(GameState *game);

//game locations and story progression
Location locations[] = {
    {0, "You find yourself in a dark, misty forest. Ancient trees loom around you. A worn path splits into three directions.", {1, 2, 3}, {10, 20, 30}},
    {1, "A mysterious cave entrance beckons. Strange symbols are carved into the rocky walls. Faint echoes of distant water can be heard.", {4, 5, 6}, {40, 50, 60}},
    {2, "A steep mountain path winds through rocky terrain. Jagged cliffs surround you, with a narrow bridge ahead.", {7, 8, 9}, {70, 80, 90}},
    {3, "You discover an abandoned village. Dilapidated houses stand silent. A well in the center seems unusually well-maintained.", {11, 12, 13}, {110, 120, 130}},
    {4, "The cave narrows. Phosphorescent fungi provide dim light. Ancient artifacts are scattered on the ground.", {14, 15, 16}, {140, 150, 160}},
    {5, "An underground river flows swiftly. Wooden rafts are tied to a rocky bank. Strange markings suggest previous travelers.", {17, 18, 19}, {170, 180, 190}},
    {6, "You reach a mountain summit. An ancient stone altar stands prominently. Winds howl around you, carrying whispers of forgotten legends.", {20, 21, 22}, {200, 210, 220}},
    {7, "A magical clearing appears. Luminescent flowers bloom in impossible colors. Magical energy tingles in the air.", {23, 24, 25}, {230, 240, 250}},
    {8, "An overgrown temple emerges from the wilderness. Intricate carvings hint at a complex history.", {26, 27, 28}, {260, 270, 280}},
    {9, "A pristine lake reflects the sky like a mirror. An old boat with mysterious runes sits by the shore.", {29, 30, 31}, {290, 300, 310}},
    // Add more locations as needed (up to TOTAL_LOCATIONS)
};

int main()
{
	GameState game;
	int choice;
	srand(time(NULL));
	
	printf("Welcome to Mystical Real Adventure!\n");
	printf("1. New Game\n2. Load Game\nEnter choice: ");
	scanf("%d",&choice);
	
	if(choice==2)
	{
		if(!loadGame(&game))
		{
			printf("No saved game found. Starting new game.\n");
			initializeGame(&game);
		}
	}
	else
	{
		initializeGame(&game);
	}
	while(!game.gameOver){
		displayLocation(locations,&game);
		displayGameStats(&game);
		
		printf("\nChoose your action:\n");
		printf("1-3.Make a choice\n");
		printf("4.Check Inventory\n");
		printf("5.Save Game\n");
		printf("6.Quit Game\n");
		printf("Enter your choice: ");
		scanf("%d",&choice);
		
		switch(choice)
		{
			case 1:
			case 2:
			case 3:
				if(processPlayerChoice(locations,&game,choice -1)== -1)
				{
					printf("Invalid choice.Try again.\n");
				}
				break;
			case 4:
				displayInventory(&game);
				break;
			case 5:
				saveGame(&game);
				printf("Game saved successfully!\n");
				break;
			case 6:
				game.gameOver = 1;
				break;
			default:
				printf("Invalid option.Try again.\n");
		}
	}
	printf("Game Over! Final Score:%d\n",game.score);
	return 0;
}

void initializeGame(GameState *game)
{
	printf("Enter your character name: ");
	scanf("%s",game->playerName);
	
	game->health=100;
	game->currentLocation=0;
	game->storyProgress=0;
	game->gameOver=0;
	game->score=0;
	
	// Initialize inventory to zero
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        game->inventory[i] = 0;
    }

}

void displayLocation(Location *locations, GameState *game) {
    Location current = locations[game->currentLocation];
    
    printf("\n--- Location Description ---\n");
    printf("%s\n", current.description);
}

void displayGameStats(GameState *game) {
    printf("\n--- Character Stats ---\n");
    printf("Name: %s\n", game->playerName);
    printf("Health: %d\n", game->health);
    printf("Current Location: %d\n", game->currentLocation);
    printf("Score: %d\n", game->score);
}
int processPlayerChoice(Location *locations, GameState *game, int choice) {
    if (choice < 0 || choice >= 3) return -1;

    // Simulate consequences of choice
    game->health -= rand() % 10;
    game->score += rand() % 50;
    game->currentLocation = locations[game->currentLocation].choices[choice];
    game->storyProgress++;

    // Simple game over conditions
    if (game->health <= 0) {
        printf("You have died. Game Over!\n");
        game->gameOver = 1;
    }

    if (game->storyProgress >= 20) {
        printf("Congratulations! You completed the adventure!\n");
        game->gameOver = 1;
    }

    return 0;
}

void saveGame(GameState *game) {
    FILE *file = fopen("mystical_realms_save.dat", "wb");
    if (file == NULL) {
        printf("Error saving game!\n");
        return;
    }
    fwrite(game, sizeof(GameState), 1, file);
    fclose(file);
}

int loadGame(GameState *game) {
    FILE *file = fopen("mystical_realms_save.dat", "rb");
    if (file == NULL) return 0;

    fread(game, sizeof(GameState), 1, file);
    fclose(file);
    return 1;
}

void displayInventory(GameState *game) {
    printf("\n--- Inventory ---\n");
    int hasItems = 0;
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        if (game->inventory[i] != 0) {
            printf("Item %d: %d\n", i+1, game->inventory[i]);
            hasItems = 1;
        }
    }
    if (!hasItems) {
        printf("Your inventory is empty.\n");
    }
}
