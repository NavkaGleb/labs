package parsers;

import util.Plane;

import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamConstants;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.XMLEvent;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;

public class StAXPlaneParser implements PlaneParser {

    @Override
    public List<Plane> parse(String xmlPath, String xsdPath) throws XMLStreamException, FileNotFoundException {
        XMLValidator.validateAgainstXSD(xmlPath, xsdPath);

        List<Plane> planes = new ArrayList<>();

        var eventReader = XMLInputFactory.newInstance().createXMLEventReader(new FileReader(xmlPath));
        var currentPlane = new Plane();
        var tag = "";

        while (eventReader.hasNext()) {
            XMLEvent event = eventReader.nextEvent();

            switch (event.getEventType()) {
                case XMLStreamConstants.START_ELEMENT -> {
                    String qName = event.asStartElement().getName().getLocalPart();
                    tag = qName;

                    if (qName.equalsIgnoreCase("plane")) {
                        currentPlane = new Plane();
                    }
                }

                case XMLStreamConstants.CHARACTERS -> {
                    String value = event.asCharacters().toString().trim();
                    if ("".equals(value)) {
                        continue;
                    }

                    switch (tag) {
                        case "model" -> currentPlane.setModel(value);
                        case "origin" -> currentPlane.setOrigin(value);
                        case "type" -> currentPlane.getSpecifications().setType(Plane.Type.valueOf(value));
                        case "seatCount" -> currentPlane.getSpecifications().setSeatCount(Integer.parseInt(value));
                        case "missileCount" -> currentPlane.getSpecifications().setMissileCount(Integer.parseInt(value));
                        case "hasRadar" -> currentPlane.setRadar(Boolean.parseBoolean(value));
                    }
                }

                case XMLStreamConstants.END_ELEMENT -> {
                    String qName = event.asEndElement().getName().getLocalPart();

                    if (qName.equalsIgnoreCase("plane")) {
                        planes.add(currentPlane);
                    }
                }
            }
        }

        return planes;
    }
}
