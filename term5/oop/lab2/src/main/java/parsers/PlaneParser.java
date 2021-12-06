package parsers;

import java.io.IOException;
import java.util.List;

import util.Plane;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.stream.XMLStreamException;

interface PlaneParser {

    List<Plane> parse(String dataFilepath, String schemaFilepath) throws
        ParserConfigurationException,
        IOException,
        SAXException,
        XMLStreamException;
}
