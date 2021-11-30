import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.ErrorHandler;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.validation.SchemaFactory;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class FootballCompetition {

    private final List<Team> teams = new ArrayList<>();

    public void addTeam(Team team) {
        teams.add(team);
    }

    public void removeTeam(Team team) {
        teams.remove(team);
    }

    public Team getTeamByIndex(int index) {
        return teams.get(index);
    }

    public int getTeamCount() {
        return teams.size();
    }

    public void loadFromFile(String filepath) throws ParserConfigurationException, SAXException, IOException {
        class SimpleErrorHandler implements ErrorHandler {

            @Override
            public void warning(SAXParseException e) {
                System.out.println("Line " +e.getLineNumber() + ":");
                System.out.println(e.getMessage());
            }

            @Override
            public void error(SAXParseException e) {
                System.out.println("Line " +e.getLineNumber() + ":");
                System.out.println(e.getMessage());
            }

            @Override
            public void fatalError(SAXParseException e) {
                System.out.println("Line " +e.getLineNumber() + ":");
                System.out.println(e.getMessage());
            }
        }

        SchemaFactory schemaFactory = SchemaFactory.newInstance("http://www.w3.org/2001/XMLSchema");

        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        factory.setValidating(false);
        factory.setSchema(schemaFactory.newSchema(new File("assets/in.xsd")));

        DocumentBuilder builder = factory.newDocumentBuilder();
        builder.setErrorHandler(new SimpleErrorHandler());

        Document document = builder.parse(new File(filepath));
        document.getDocumentElement().normalize();

        NodeList teamNodes = document.getElementsByTagName("Team");

        teams.clear();

        for (int i = 0; i < teamNodes.getLength(); ++i) {
            var teamNode = (Element)teamNodes.item(i);
            var team = new Team();

            team.name = teamNode.getAttribute("name");

            var playerNodes = teamNode.getElementsByTagName("Player");

            for (int j = 0; j < playerNodes.getLength(); ++j) {
                var playerNode = (Element)playerNodes.item(j);
                var player = new Player();

                player.firstName = playerNode.getAttribute("firstName");
                player.lastName = playerNode.getAttribute("lastName");
                player.age = Integer.parseInt(playerNode.getAttribute("age"));

                team.addPlayer(player);
            }

            teams.add(team);
        }
    }

    public void saveToFile(String filepath) throws ParserConfigurationException, TransformerException {
        DocumentBuilderFactory documentFactory = DocumentBuilderFactory.newInstance();
        documentFactory.setValidating(true);

        DocumentBuilder builder = documentFactory.newDocumentBuilder();
        Document document = builder.newDocument();
        Element root = document.createElement("FootballCompetition");

        document.appendChild(root);

        for (var team : teams) {
            var teamNode = document.createElement("Team");

            teamNode.setAttribute("name", team.name);

            for (int i = 0; i < team.getPlayerCount(); ++i) {
                var player = team.getPlayerByIndex(i);
                var playerNode = document.createElement("Player");

                playerNode.setAttribute("firstName", player.firstName);
                playerNode.setAttribute("lastName", player.lastName);
                playerNode.setAttribute("age", String.valueOf(player.age));

                teamNode.appendChild(playerNode);
            }

            root.appendChild(teamNode);
        }

        Source domSource = new DOMSource(document);
        Result fileResult = new StreamResult(new File(filepath));
        TransformerFactory transformerFactory = TransformerFactory.newInstance();
        Transformer transformer = transformerFactory.newTransformer();

        transformer.setOutputProperty(OutputKeys.ENCODING,"UTF-8");
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");
        transformer.transform(domSource, fileResult);
    }
}
