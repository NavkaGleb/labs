import java.util.ArrayList;
import java.util.List;

public class Team {

    public String               name;
    private final List<Player>  players = new ArrayList<>();

    public void addPlayer(Player player) {
        players.add(player);
    }

    public void removePlayer(Player player) {
        players.remove(player);
    }

    public Player getPlayerByIndex(int index) {
        return players.get(index);
    }

    public int getPlayerCount() {
        return players.size();
    }

    @Override
    public String toString() {
        return name;
    }
}
