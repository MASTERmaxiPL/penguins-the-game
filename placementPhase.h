#ifndef PLACEMENT_PHASE_H
#define PLACEMENT_PHASE_H

#include "gameBoard.h"
#include "gameManager.h"
#include "player.h"


void PlacementPhase_Run(GameManager *gm);
bool Can_Player_Place(const Player *p);
void Player_Place(Player *p, IceFloe *floe, int x, int y);


#endif