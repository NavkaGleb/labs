package parsers;

import org.w3c.dom.Element;
import org.w3c.dom.Node;

import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class DOMParser<T> implements XMLParser<T> {

    @Override
    public List<T> parse(String xmlPath, String xsdPath, XMLBuilder<T> builder) throws ParserConfigurationException, IOException, SAXException {
        XMLValidator.validateAgainstXSD(xmlPath, xsdPath);

        var builderFactory = DocumentBuilderFactory.newInstance();
        var documentBuilder = builderFactory.newDocumentBuilder();
        var document = documentBuilder.parse(new File(xmlPath));

        var nodes = document.getDocumentElement().getChildNodes();

        List<T> result = new ArrayList<>();

        for (int i = 0; i < nodes.getLength(); ++i) {
            Node node = nodes.item(i);

            if (node instanceof Element element) {
                result.add(builder.buildByDOM(element));
            }
        }

        return result;
    }
}
