package parsers;

import util.Plane;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParserFactory;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

class PlaneSAXHandler extends DefaultHandler {

    private final List<Plane>   planes = new ArrayList<>();
    private StringBuilder       value = null;
    private Plane               currentPlane = null;
    private String              tag = "";

    public List<Plane> getPlanes() {
        return planes;
    }

    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) {
        tag = qName;
        value = new StringBuilder();

        if (qName.equalsIgnoreCase("plane")) {
            currentPlane = new Plane();
        }
    }

    @Override
    public void characters(char[] ch, int start, int length) {
        value.append(new String(ch, start, length));
    }

    @Override
    public void endElement(String uri, String localName, String qName) {
        switch (tag) {
            case "model" -> currentPlane.setModel(value.toString().trim());
            case "origin" -> currentPlane.setOrigin(value.toString().trim());
            case "type" -> currentPlane.getSpecifications().setType(Plane.Type.valueOf(value.toString().trim()));
            case "seatCount" -> currentPlane.getSpecifications().setSeatCount(Integer.parseInt(value.toString().trim()));
            case "missileCount" -> currentPlane.getSpecifications().setMissileCount(Integer.parseInt(value.toString().trim()));
            case "hasRadar" -> currentPlane.setRadar(Boolean.parseBoolean(value.toString().trim()));
        }

        if (qName.equalsIgnoreCase("plane")) {
            planes.add(currentPlane);
        }
    }
}

public class SAXPlaneParser implements PlaneParser {

    @Override
    public List<Plane> parse(String xmlPath, String xsdPath) throws ParserConfigurationException, SAXException, IOException {
        XMLValidator.validateAgainstXSD(xmlPath, xsdPath);

        var saxParserFactory = SAXParserFactory.newInstance();
        var saxParser = saxParserFactory.newSAXParser();

        var handler = new PlaneSAXHandler();
        saxParser.parse(new File(xmlPath), handler);

        return handler.getPlanes();
    }
}
