package parsers;

import java.io.IOException;
import java.util.List;

import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.stream.XMLStreamException;

interface XMLParser<T> {

    List<T> parse(String dataFilepath, String schemaFilepath, XMLBuilder<T> builder) throws
        ParserConfigurationException,
        IOException,
        SAXException,
        XMLStreamException;
}
