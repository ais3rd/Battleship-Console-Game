#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

// Cell representations
#define AIR '*'
#define HIT '#'
#define MISS 'O'

// Color codes
#define COLOR_DEFAULT 7
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_RED 12
#define COLOR_YELLOW 14

// Defines the properties of a single ship
struct Ship{
    char name[20];
    int length;
    char symbol;
};

// Defines all the ships used in the game
struct Ship SHIPS[5] =
{
    {"5-parts-ship", 5, '5'},
    {"4-parts-ship", 4, '4'},
    {"alpha-3-parts-ship", 3, '3'},
    {"beta-3-parts-ship", 3, '8'},
    {"2-parts-ship", 2, '2'}
};


// --- Function Prototypes ---
void initializeBoard(char board[10][10]);
void printBoards(char playerBoard[10][10], char opposingBoard[10][10], int playerParts, int opposingParts);
void placeShipsRandomly(char board[10][10]);
void getPlayerMove(int *row, int *col);
int isShipSunk(char board[10][10], char shipSymbol);

// -- Main Game Execution ---
int main()
{
    // === 1. Game Setup ===
    char playerBoard[10][10];
    char opposingBoard[10][10];
    char displayOpposingBoard[10][10];

    int playerParts = 17;
    int opposingParts = 17;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));

    initializeBoard(playerBoard);
    initializeBoard(opposingBoard);
    initializeBoard(displayOpposingBoard);

    placeShipsRandomly(playerBoard);
    placeShipsRandomly(opposingBoard);

    // === 2. Main Game Loop ===
    while (playerParts > 0 && opposingParts > 0)
    {
        printBoards(playerBoard, displayOpposingBoard, playerParts, opposingParts);

        // Player's Turn
        int row, col;
        getPlayerMove(&row, &col);

        if (opposingBoard[row][col] != AIR && opposingBoard[row][col] != HIT && opposingBoard[row][col] != MISS)
        {
            SetConsoleTextAttribute(hConsole, COLOR_RED);
            printf("It's a HIT!\n");
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);

            char shipSymbol = opposingBoard[row][col];
            opposingBoard[row][col] = HIT;
            displayOpposingBoard[row][col] = HIT;
            opposingParts--;

            if (isShipSunk(opposingBoard, shipSymbol))
            {
                for(int i = 0; i < 5; ++i)
                {
                    if(SHIPS[i].symbol == shipSymbol)
                    {
                        SetConsoleTextAttribute(hConsole, COLOR_YELLOW);
                        printf("That %s is sunk!\n", SHIPS[i].name);
                        SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
                        break;
                    }
                }
            }
        }
        else if (displayOpposingBoard[row][col] == HIT || displayOpposingBoard[row][col] == MISS)
        {
            printf("You already shot there!\n");
        }
        else
        {
            SetConsoleTextAttribute(hConsole, COLOR_BLUE);
            printf("It's a MISS!\n");
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
            displayOpposingBoard[row][col] = MISS;
        }

        // AI's Turn
        if (opposingParts > 0)
        {
            do
            {
                row = rand() % 10;
                col = rand() % 10;
            }
            while (playerBoard[row][col] == HIT || playerBoard[row][col] == MISS);

            printf("The enemy shot at %c%d\n", 'A' + col, row + 1);

            if (playerBoard[row][col] != AIR)
            {
                SetConsoleTextAttribute(hConsole, COLOR_RED);
                printf("The enemy HIT!\n");
                SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
                char shipSymbol = playerBoard[row][col];
                playerBoard[row][col] = HIT;
                playerParts--;

                if (isShipSunk(playerBoard, shipSymbol))
                {
                    for(int i = 0; i < 5; ++i)
                    {
                        if(SHIPS[i].symbol == shipSymbol)
                        {
                            SetConsoleTextAttribute(hConsole, COLOR_YELLOW);
                            printf("Your %s is sunk!\n", SHIPS[i].name);
                            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
                            break;
                        }
                    }
                }
            }
            else
            {
                SetConsoleTextAttribute(hConsole, COLOR_BLUE);
                printf("The enemy MISS!\n");
                SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
                playerBoard[row][col] = MISS;
            }
        }
        printf("\nPress Enter to continue...");
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Delete Buffer // Pause
        printf("\e[1;1H]\e[2J"); // Scroll up
    }

    // === 3. Game Over ===
    printBoards(playerBoard, displayOpposingBoard, playerParts, opposingParts);
    if (opposingParts == 0)
    {
        SetConsoleTextAttribute(hConsole, COLOR_GREEN);
        printf("\n---You WIN!---\n");
    }
    else
    {
        SetConsoleTextAttribute(hConsole, COLOR_RED);
        printf("\n---The enemy WINS---\n");
    }
    SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);

    return 0;
}


// --- Function Implementations ---
void initializeBoard(char board[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            board[i][j] = AIR;
        }
    }
}

void printBoards(char playerBoard[10][10], char opposingBoard[10][10], int playerParts, int opposingParts)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("\nYOUR BOARD (parts left: %d)          OPPOSING BOARD (parts left: %d)\n", playerParts, opposingParts);
    printf("    A B C D E F G H I J                      A B C D E F G H I J\n");
    printf("  -----------------------                  -----------------------\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%2d|", i + 1);
        for (int j = 0; j < 10; j++)
        {
            char cell = playerBoard[i][j];
            int color = COLOR_DEFAULT;

            if (cell == HIT) color = COLOR_RED;
            else if (cell == MISS) color = COLOR_BLUE;
            else if (cell != AIR) color = COLOR_GREEN;

            SetConsoleTextAttribute(hConsole, color);
            printf(" %c", cell);
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
        }
        printf(" |");

        printf("                %2d|", i + 1);

        for (int j = 0; j < 10; j++)
        {
            char cell = opposingBoard[i][j];
            switch (cell)
            {
            case HIT:
                SetConsoleTextAttribute(hConsole, COLOR_RED);
                break;
            case MISS:
                SetConsoleTextAttribute(hConsole, COLOR_BLUE);
                break;
            default:
                SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
                break;
            }
            printf(" %c", cell);
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
        }
        printf(" |\n");
    }
    printf("  -----------------------                  -----------------------\n");
}

void placeShipsRandomly(char board[10][10])
{
    for (int i = 0; i < 5; i++)
    {
        int shipLength = SHIPS[i].length;
        char shipSymbol = SHIPS[i].symbol;
        int placed = 0;

        while (!placed)
        {
            int row = rand() % 10;
            int col = rand() % 10;
            int direction = rand() % 2;

            if (direction == 0)   // Horizontal
            {
                if (col + shipLength <= 10)
                {
                    int valid = 1;
                    for (int j = 0; j < shipLength; j++)
                    {
                        if (board[row][col + j] != AIR)
                        {
                            valid = 0;
                            break;
                        }
                    }
                    if (valid)
                    {
                        for (int j = 0; j < shipLength; j++)
                        {
                            board[row][col + j] = shipSymbol;
                        }
                        placed = 1;
                    }
                }
            }
            else   // Vertical
            {
                if (row + shipLength <= 10)
                {
                    int valid = 1;
                    for (int j = 0; j < shipLength; j++)
                    {
                        if (board[row + j][col] != AIR)
                        {
                            valid = 0;
                            break;
                        }
                    }
                    if (valid)
                    {
                        for (int j = 0; j < shipLength; j++)
                        {
                            board[row + j][col] = shipSymbol;
                        }
                        placed = 1;
                    }
                }
            }
        }
    }
}

void getPlayerMove(int *row, int *col)
{
    int valid = 0;
    while (!valid)
    {
        printf("Enter your move (format: A1 to J10): ");
        char colChar;
        int rowNum;
        int itemsRead = scanf(" %c%d", &colChar, &rowNum);

        // int c;
        // while ((c = getchar()) != '\n' && c != EOF);

        if (itemsRead == 2)
        {
            colChar = toupper(colChar);
            if (colChar >= 'A' && colChar <= 'J' && rowNum >= 1 && rowNum <= 10)
            {
                *col = colChar - 'A';
                *row = rowNum - 1;
                valid = 1;
            }
        }

        if (!valid)
        {
            printf("Invalid input. Please use format like A1, D5, or J10.\n");
        }
    }
}

int isShipSunk(char board[10][10], char shipSymbol)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (board[i][j] == shipSymbol)
            {
                return 0; // The ship is not sunk
            }
        }
    }
    return 1; // The ship is sunk
}
