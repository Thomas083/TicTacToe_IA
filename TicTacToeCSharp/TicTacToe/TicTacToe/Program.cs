using TicTacToe.Engine;

var game = new TicTacToe.Engine.TicTacToe();

while (game.State is GameState.InProgress i)
{
    IEnumerable<GridPosition> emptyCells = i.History.GetGrid().EmptyCells;
    game.Play(GridPosition.BottomCenter);
}