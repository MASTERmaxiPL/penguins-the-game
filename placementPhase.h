#ifndef PLACEMENT_PHASE_H
#define PLACEMENT_PHASE_H

#include "gameBoard.h"
#include "gameManager.h"
#include "player.h"


void PlacementPhase_Run(GameManager *gm);
bool Can_Player_Place(Player *p);
void Player_Place(GameBoard *gb, Player *p, int x, int y);


#endif