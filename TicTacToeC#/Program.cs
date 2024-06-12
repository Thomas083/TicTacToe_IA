var game = new TicTacToe.Engine.TicTacToe();

while(game.State is GameStates.InProgress i)
{
    i.History.GetGrid().EmptyCells
    game.Play(GridPosition.BottomCenter);
}

