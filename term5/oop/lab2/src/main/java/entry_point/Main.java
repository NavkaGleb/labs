package entry_point;

import org.xml.sax.SAXException;
import parsers.DOMParser;
import parsers.PlaneBuilder;
import parsers.SAXParser;
import util.Plane;

import javax.xml.parsers.ParserConfigurationException;
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

    public static void main(String[] args) throws ParserConfigurationException, IOException, SAXException {
        var planeBuilder = new PlaneBuilder();

        var domParser = new DOMParser<Plane>();
        var saxParser = new SAXParser<Plane>();

        var domPlanes = domParser.parse(XML_PATH, XSD_PATH, planeBuilder);
        var saxPlanes = saxParser.parse(XML_PATH, XSD_PATH, planeBuilder);

        logPlanes("DOM Parser", domPlanes);
        logPlanes("SAX Parser", saxPlanes);
    }
}
