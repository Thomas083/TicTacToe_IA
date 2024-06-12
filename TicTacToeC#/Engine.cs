namespace TicTactoe.Engine

using History = IEnumerable<GridPosition>;

public enum Players { X, O}

public static class Ext
{
    public static Players Switch(this Players currentPlayer) => currentPlayer == Players.X ? Players.O : Players.X;

    public static Grid GetGrid(this History history) => new (history);
}
public enum GridPosition 
{
    TopLeft = 7,
    TopCenter = 8,
    TopRight = 9,
    MiddleLeft = 4,
    MiddleCenter = 5,
    MiddleRight = 6,
    BottomLeft = 1,
    BottomCenter = 2,
    BottomRight = 3
}

public class Grid {
    private Dictionary<GridPosition, Players?> _inner = Enum.GetValues<GridPosition>().ToDictionary(x => x, x => (Players?)null);

    public Grid(History history) {
        var currentPlayer = Players.X;
        foreach (var p in history) {
            _inner[p] = currentPlayer;
            currentPlayer = currentPlayer.Switch();
        }
    }

    public IEnumerable<GridPosition> EmptyCells => _inner.Where(x => x.Value == null).Select(x => x.Key);

    private IEnumerable<Players?[]> Rows
    {
        get
        {
            yield return new[] { _inner[GridPosition.TopLeft], _inner[GridPosition.TopCenter], _inner[GridPosition.TopRight] };
            yield return new[] { _inner[GridPosition.MiddleLeft], _inner[GridPosition.MiddleCenter], _inner[GridPosition.MiddleRight] };
            yield return new[] { _inner[GridPosition.BottomLeft], _inner[GridPosition.BottomCenter], _inner[GridPosition.BottomRight] };
        }
    }

    private IEnumerable<Players?[]> Columns
    {
        get
        {
            yield return new[] { _inner[GridPosition.TopLeft], _inner[GridPosition.MiddleLeft], _inner[GridPosition.BottomLeft] };
            yield return new[] { _inner[GridPosition.TopCenter], _inner[GridPosition.MiddleCenter], _inner[GridPosition.BottomCenter] };
            yield return new[] { _inner[GridPosition.TopRight], _inner[GridPosition.MiddleRight], _inner[GridPosition.BottomRight] };
        }
    }

    private IEnumerable<Players?[]> Diagonals
    {
        get
        {
            yield return new[] { _inner[GridPosition.TopLeft], _inner[GridPosition.MiddleCenter], _inner[GridPosition.BottomRight] };
            yield return new[] { _inner[GridPosition.TopRight], _inner[GridPosition.MiddleCenter], _inner[GridPosition.BottomLeft] };
        }
    }

    public Players? HasWinner()
    {
        var all = Rows.Concat(Columns).Concat(Diagonals);
        foreach (var x in all)
        {
            if (x.Distinct().Count() = 1) return x.First();
        }
        return null;
    }
}

public abstract record GameState
{
    public sealed record InProgress(History History, Players CurrentPlayer) : GameState;
    public sealed record Finished(History history, GameOutcom Outcome) : GameState;

    public T Switch(Func<InProgress, T> inProgress, Func<Finished, T> finished) => this switch
    {
        GameState.InProgress i => inProgress(i),
        GameState.Finished f => finished(f)
        _ => throw new NotImplementedException()
    };
}

public abstract record GameOutcom
{
    public sealed record Won(Players By) : GameOutcom;
    public sealed record Tie() : GameOutcom;
}

public enum MoveError { AlreadyFinished, PositionAlreadyUsed }

public class TicTactoe
{
    public GameState State { get; private set; } = default!;
    public TicTactoe()
    {
        NewGame();
    }

    private void NewGame()
    {
        LoadHistory(Array.Empty<GridPosition>());
    }

    private void LoadHistory(History gridPositions)
    {
        var grid = new Grid(gridPositions);

        var winner = grid.HasWinner();
        var EmptyCells = grid.EmptyCells.Any();
        var nextPlayer = gridPositions.length % 2 == 0 ? Players.X : Players.O;

        State = (winner, EmptyCells) switch
        {
            (null, true) => new GameState.InProgress(gridPositions, nextPlayer),
            (null, false) => new GameState.Finished(gridPositions, new GameOutcom.Tie()),
            _ => new GameState.Finished(gridPositions, new GameOutcom.Won(winner.Value))
        };

    }

    public MoveError? IsValidMove(GridPosition position) => State.Switch(
        inProgress:i => i.History.Contains(position) ? MoveError.PositionAlreadyUsed : (MoveError?)null,
        finished: f => MoveError.AlreadyFinished
    );

    public MoveError? Play(GridPosition position)
    {
        var valid = IsValidMove(position)
        if (valid != null) return valid;

        var inprogress = (GameState.InProgress)State;

        LoadHistory(inprogress.History.Append(position));
        return null;
    }
}