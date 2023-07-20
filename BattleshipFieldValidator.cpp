#include <iostream>
#include <vector>
#include <map>

class PlayerField {
private:

    class Ship;

public:

    PlayerField(std::vector<std::vector<int>> field) {
        foundShips(field);
        сheckQuantity();
    }

    bool validation() {
        return isValidate;
    }

private:

    class Ship {
    public:

        Ship(bool isVertical_, std::pair<int, int> startPositionBegin, std::pair<int, int> startPositionEnd)
            : isVertical(isVertical_), positionShip{ startPositionBegin, startPositionEnd }, sizeShip(1) {}

        void setPositionBegin(std::pair<int, int> positionBegin) {
            positionShip[0] = positionBegin;
        }

        void setPositionEnd(std::pair<int, int> positionEnd) {
            positionShip[1] = positionEnd;
            defineSize();
        }

        std::pair<int, int> begin() {
            return positionShip[0];
            defineSize();
        }

        std::pair<int, int> end() {
            return positionShip[1];
        }

        bool orientaton() {
            return isVertical;
        }

        int type() {
            return sizeShip;
        }

    private:

        int sizeShip;

        bool isVertical;
        // isVertical == true (duration = vertical), isVertical == false (duration = horizontal)

        std::pair<int, int> positionShip[2]; 
        // positionShip[0] - begin position, positionShip[1] - end position

        void defineSize() {
            if (isVertical) {
                sizeShip = positionShip[1].first - positionShip[0].first;
            }
            else {
                sizeShip = positionShip[1].second - positionShip[0].second;
            }
            ++sizeShip;
        }

    };

    std::vector<Ship> shipsPlayer; 
    // must be: submarines (1s) - 4, destroyers (2s) - 3, cruisers (3s) - 2, battleship (4s) - 1
    
    bool isValidate = true;

    void foundShips(std::vector<std::vector<int>> field) {
        bool isDurationVertical = false;
        bool isDurationHorisont = false;
        for (int i = 0; i < 10; ++i) {       // i - rows
            for (int j = 0; j < 10; ++j) {   // j - collums
                //  checking to see if stumbled upon a ship
                bool isShipPartFound = field[i][j] == 1; 
                if (isShipPartFound) {
                    //  the ship be stretched vertically
                    isDurationVertical = (i + 1 < 10 ? field[i + 1][j] == 1 : false) || (i - 1 >= 0 ? field[i - 1][j] == 1 : false);
                    //  the ship be stretched horizontally
                    isDurationHorisont = (j + 1 < 10 ? field[i][j + 1] == 1 : false) || (j - 1 >= 0 ? field[i][j - 1] == 1 : false); 
                    // if it does not occupy a straight line - is an error
                    if (isDurationVertical && isDurationHorisont) {
                        isValidate = false;
                        return;
                    } 
                    // if there is not additional 1 around - is a single
                    if (!isDurationVertical && !isDurationHorisont) {
                        shipsPlayer.push_back(Ship{true, std::pair<int,int>{i, j}, std::pair<int,int>{i, j} });                
                    }
                    // if ship on a vertical line
                    if (isDurationVertical) {
                        shipsPlayer.push_back(Ship{ isDurationVertical, std::pair<int,int>{i, j}, std::pair<int,int>{i, j} });
                        // go up
                        for (int u = i - 1; u >= 0; --u) {
                            bool isLeftElement = j - 1 >= 0 ? field[u][j - 1] == 1 : false;
                            bool isRightElement = j + 1 <= 10 ? field[u][j + 1] == 1 : false;
                            if (isLeftElement || isRightElement) {
                                isValidate = false;
                                return;
                            }
                            bool isUpperElementPartShip = field[u][j] == 1;
                            if (isUpperElementPartShip) 
                                shipsPlayer.back().setPositionBegin(std::pair<int, int>{u, j});
                            else 
                                break;
                        }
                        // go down
                        for (int d = i + 1; d < 10; ++d) {
                            bool isLeftElement = j - 1 >= 0 ? field[d][j - 1] == 1 : false;
                            bool isRightElement = j + 1 <= 10 ? field[d][j + 1] == 1 : false;
                            if (isLeftElement || isRightElement) {
                                isValidate = false;
                                return;
                            }
                            bool isDownerElementPartShip = field[d][j] == 1;
                            if (isDownerElementPartShip)
                                shipsPlayer.back().setPositionEnd(std::pair<int, int>{d, j});
                            else
                                break;
                        }

                    }
                    // if ship on a horizont line
                    if (isDurationHorisont) {
                        shipsPlayer.push_back(Ship{ isDurationVertical, std::pair<int,int>{i, j}, std::pair<int,int>{i, j} });
                        // go left
                        for (int l = j - 1; l >= 0; --l) {
                            bool isUpperElement = i - 1 >= 0 ? field[i - 1][l] == 1 : false;
                            bool isDownerElement = i + 1 <= 10 ? field[i + 1][l] == 1 : false;
                            if (isUpperElement || isDownerElement) {
                                isValidate = false;
                                return;
                            }
                            bool isLefterElementPartShip = field[i][l] == 1;
                            if (isLefterElementPartShip) 
                                shipsPlayer.back().setPositionBegin(std::pair<int, int>{i, l});
                            else
                                break;
                        }
                        // go right
                        for (int r = j + 1; r < 10; ++r) {
                            bool isUpperElement = i - 1 >= 0 ? field[i - 1][r] == 1 : false;
                            bool isDownerElement = i + 1 <= 10 ? field[i + 1][r] == 1 : false;
                            if (isUpperElement || isDownerElement) {
                                isValidate = false;
                                return;
                            }
                            bool isRighterElementPartShip = field[i][r] == 1;
                            if (isRighterElementPartShip) 
                                shipsPlayer.back().setPositionEnd(std::pair<int, int>{i, r});
                            else
                                break;
                        }

                    }
                    erasing(field);
                }
            }
        }
    }

    void erasing(std::vector<std::vector<int>>& field) {
        if (shipsPlayer.back().orientaton()) {
            // if vertical
            int j = shipsPlayer.back().begin().second;
            for (int i = shipsPlayer.back().begin().first; i <= shipsPlayer.back().end().first; ++i) {
                field[i][j] = 0;
            }
        }
        else {
            // if horizontal
            int i = shipsPlayer.back().begin().first;
            for (int j = shipsPlayer.back().begin().second; j <= shipsPlayer.back().end().second; ++j) {
                field[i][j] = 0;
            }
        }
    }

    void сheckQuantity() {
        if (shipsPlayer.size() != 10) {
            isValidate = false;
            return;
        }
        std::map<int, int> shipsNumber;
        for (int i = 0; i < shipsPlayer.size(); ++i) {
            bool isSizeEmplaced = shipsNumber.count(shipsPlayer[i].type()) > 0;
            if (isSizeEmplaced)
                ++shipsNumber[shipsPlayer[i].type()];
            else
                shipsNumber.emplace(shipsPlayer[i].type(), 1);
        }
        for (int i = 1; i <= 4; ++i) {
            if (shipsNumber[i] != 5 - i) {
                isValidate = false;
                return;
            }
        }
    }
};


bool validate_battlefield(std::vector< std::vector<int>> field) {
    PlayerField playerField(field);
    return playerField.validation();
}
