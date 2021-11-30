import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;
import java.io.IOException;

public class Main {

    public static void main(String[] args) throws ParserConfigurationException,
                                                  SAXException,
                                                  IOException,
                                                  TransformerException {
        // Variant 8
        var competition = new FootballCompetition();

        competition.loadFromFile("assets/in.xml");
        competition.saveToFile("assets/out.xml");

        for (int i = 0; i < competition.getTeamCount(); ++i) {
            var team = competition.getTeamByIndex(i);

            System.out.println(team);

            for (int j = 0; j < team.getPlayerCount(); ++j) {
                System.out.println("\t" + team.getPlayerByIndex(j));
            }
        }
    }
}
