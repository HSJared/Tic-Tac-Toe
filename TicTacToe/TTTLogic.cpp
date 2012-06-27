/****************************************************************************
*																			*
*	File:		TTTLogic.cpp												*
*																			*
*	Author:		Robb T. Koether												*
*																			*
*	Date:		Aug 19, 2011												*
*																			*
*	Purpose:	This contains various utility functions needed to play		*
*				Tic Tac Toe.												*
*																			*
****************************************************************************/

//	Include headers

#include "TTTLogic.h"

//	Define the winning configurations

Set<9> wins[8] = {Player(0007), Player(0070), Player(0700), Player(0111), Player(0222), Player(0444), Player(0124), Player(0421)};

/****************************************************************************
*																			*
*	Function:	nextMove													*
*																			*
*	Purpose:	To compute the next move that O should take, depending on	*
*				the level of play											*
*																			*
****************************************************************************/

int nextMove(Player Xmoves, Player Omoves, int level)
{
	Set<9> moves;

	if (level >= 2)
	{
	//	Check for a winning move

		moves = getWinningMoves(Omoves, Xmoves);
		if (!moves.isEmpty())
			return moves.randomMember();
	}
	
	if (level >= 3)
	{
	//	Block a winning move

		moves = getWinningMoves(Xmoves, Omoves);
		if (!moves.isEmpty())
			return moves.randomMember();
	}

	if (level >= 4)
	{
	//	Check for a move that creates a winning move

		moves = getCreatesWinningMoves(Omoves, Xmoves);
		if (!moves.isEmpty())
			return moves.randomMember();
	}

	if (level >= 5)
	{
	//	Block a move that creates a winning move

		moves = getCreatesWinningMoves(Xmoves, Omoves);
		if (!moves.isEmpty())
			return moves.randomMember();
	}

	return (-(Xmoves + Omoves)).randomMember();
}

/****************************************************************************
*																			*
*	Function:	getWinningMoves												*
*																			*
*	Purpose:	To return a set of moves any one of which will win the		*
*				game														*
*																			*
****************************************************************************/

Player getWinningMoves(Player player, Player opponent)
{
	Set<9> winMoves;

//	Get set of all unoccupied cells

	Set<9> available = -(player + opponent);
	for (int i = 0; i < 8; i++)
	{
	//	Look for winning configurations with two occupied cells and
	//	one empty cell

		Set<9> s = available * wins[i];
		Set<9> t = player * wins[i];
		if (s.size() == 1 && t.size() == 2)
			winMoves += s;
	}
	return winMoves;
}

/****************************************************************************
*																			*
*	Function:	getCreatesWinningMoves										*
*																			*
*	Purpose:	To return a set of moves any one of which will create a		*
*				winning move												*
*																			*
****************************************************************************/

Set<9> getCreatesWinningMoves(Player player, Player opponent)
{
	Set<9> createWinMoves;

//	Get set of all unoccupied cells

	Set<9> available = -(player + opponent);
	for (int i = 0; i < 8; i++)
	{
	//	Look for winning configurations with one occupied cell and
	//	two empty cells

		Set<9> s = available * wins[i];
		Set<9> t = player * wins[i];
		if (s.size() == 2 && t.size() == 1)
			createWinMoves += s;
	}
	return createWinMoves;
}

/****************************************************************************
*																			*
*	Function:	isOccupied													*
*																			*
*	Purpose:	To return true if the cell is occupied by the player		*
*																			*
****************************************************************************/

bool isOccupied(Player player, int cell)
{
	return player.isMember(cell);
}

/****************************************************************************
*																			*
*	Function:	isBoardFilled												*
*																			*
*	Purpose:	To return true if all the cells are occupied				*
*																			*
****************************************************************************/

bool isBoardFilled(Player player, Player opponent)
{
	return (player + opponent).isUniversal();
}

/****************************************************************************
*																			*
*	Function:	winningConfig												*
*																			*
*	Purpose:	To return the index of the winning configuration, if the	*
*				the player has one.  Otherwise, return -1.					*
*																			*
****************************************************************************/

int winningConfig(Player player)
{
	for (int i = 0; i < 8; i++)
		if (wins[i].isSubsetOf(player))
			return i;
	return -1;
}

/****************************************************************************
*																			*
*	Function:	hasWon														*
*																			*
*	Purpose:	To return true if the player has won the game				*
*																			*
****************************************************************************/

bool hasWon(Player player)
{
	return winningConfig(player) >= 0;
}