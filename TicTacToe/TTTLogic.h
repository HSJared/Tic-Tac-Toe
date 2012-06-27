#ifndef TTTLOGIC_H
#define TTTLOGIC_H

#include <iostream>
#include "set.h"

using namespace std;

typedef Set<9> Player;

int nextMove(Player Xmoves, Player Omoves, int level);
int winningConfig(Player player);
Set<9> getWinningMoves(Player player, Player opponent);
Set<9> getCreatesWinningMoves(Player player, Player opponent);
bool isOccupied(Player player, int cell);
bool isBoardFilled(Player player, Player opponent);
bool hasWon(Player player);

#endif