package parsers;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParserFactory;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

class SAXHandler<T> extends DefaultHandler {

    private final List<T>       result = new ArrayList<>();
    private final XMLBuilder<T> builder;
    private StringBuilder       value = null;
    private String              tag = "";

    public SAXHandler(XMLBuilder<T> builder) {
        super();

        this.builder = builder;
    }

    public List<T> getResult() {
        return result;
    }

    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) {
        tag = qName;
        value = new StringBuilder();
    }

    @Override
    public void characters(char[] ch, int start, int length) {
        value.append(new String(ch, start, length));
    }

    @Override
    public void endElement(String uri, String localName, String qName) {
        T object = builder.buildBySAX(value.toString().trim(), tag, qName);

        if (object != null) {
            result.add(object);
        }
    }
}

public class SAXParser<T> implements XMLParser<T> {

    @Override
    public List<T> parse(String xmlPath, String xsdPath, XMLBuilder<T> builder) throws ParserConfigurationException, SAXException, IOException {
        XMLValidator.validateAgainstXSD(xmlPath, xsdPath);

        var saxParserFactory = SAXParserFactory.newInstance();
        var saxParser = saxParserFactory.newSAXParser();

        var handler = new SAXHandler<T>(builder);
        saxParser.parse(new File(xmlPath), handler);

        return handler.getResult();
    }
}
