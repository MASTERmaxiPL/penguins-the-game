#ifndef PLACEMENT_PHASE_H
#define PLACEMENT_PHASE_H

#include "gameBoard.h"
#include "gameManager.h"

void PlacementPhase_Run(GameManager *gm);
void Player_Place(int playerIndex, int *players, IceFloe *floe, int x, int y);
void Player_Placement_Turn(GameManager *gm, int currentPlayerIndex);


#endif
