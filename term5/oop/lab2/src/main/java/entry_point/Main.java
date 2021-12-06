package entry_point;

import org.xml.sax.SAXException;
import parsers.DOMPlaneParser;
import parsers.SAXPlaneParser;
import parsers.StAXPlaneParser;
import util.Plane;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.stream.XMLStreamException;
import java.io.IOException;
import java.util.List;

public class Main {

    public static final String XML_PATH = "src/main/resources/planes.xml";
    public static final String XSD_PATH = "src/main/resources/planes.xsd";

    public static void logPlanes(String parserName, List<Plane> planes) {
        System.out.println(parserName);

        for (var plane : planes) {
            System.out.println(plane);
            System.out.println();
        }

        System.out.println("-------------------------------");
    }

    public static void main(String[] args) throws ParserConfigurationException, IOException, SAXException, XMLStreamException {
        var domParser = new DOMPlaneParser();
        var saxParser = new SAXPlaneParser();
        var staxParser = new StAXPlaneParser();

        var domPlanes = domParser.parse(XML_PATH, XSD_PATH);
        var saxPlanes = saxParser.parse(XML_PATH, XSD_PATH);
        var staxPlanes = staxParser.parse(XML_PATH, XSD_PATH);

        logPlanes("DOM Parser", domPlanes);
        logPlanes("SAX Parser", saxPlanes);
        logPlanes("StAX Parser", staxPlanes);
    }
}
