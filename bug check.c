main() {
void initShips(theShip ships[numShips]) {
    const char *names[numShips] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
    const int sizes[numShips] = {5,4,3,3,2};
    for (int i = 0; i < numShips; i++) {
        strcpy(ships[i].name, names[i], sizeof(ships[i].name)-1);
        ships[i].name[sizeof(ships[i].name)-1] = 0;
        ships[i].size = sizes[i];
        ships[i].health = sizes[i];
    }
}

return 0;}
