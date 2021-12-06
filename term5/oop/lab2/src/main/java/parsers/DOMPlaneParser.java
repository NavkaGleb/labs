package parsers;

import util.Plane;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class DOMPlaneParser implements PlaneParser {

    private static String getFieldFromElement(Element element, String key) {
        return element.getElementsByTagName(key).item(0).getTextContent();
    }

    @Override
    public List<Plane> parse(String xmlPath, String xsdPath) throws ParserConfigurationException, IOException, SAXException {
        XMLValidator.validateAgainstXSD(xmlPath, xsdPath);

        var builderFactory = DocumentBuilderFactory.newInstance();
        var builder = builderFactory.newDocumentBuilder();
        var document = builder.parse(new File(xmlPath));

        var planeNodes = document.getDocumentElement().getChildNodes();

        List<Plane> planes = new ArrayList<>();

        for (int i = 0; i < planeNodes.getLength(); ++i) {
            Node node = planeNodes.item(i);

            if (node instanceof Element element) {
                planes.add(new Plane(
                    getFieldFromElement(element, "model"),
                    getFieldFromElement(element, "origin"),
                    new Plane.Specifications(
                        Plane.Type.valueOf(getFieldFromElement(element, "type")),
                        Integer.parseInt(getFieldFromElement(element, "seatCount")),
                        Integer.parseInt(getFieldFromElement(element, "missileCount"))
                    ),
                    Boolean.parseBoolean(getFieldFromElement(element, "hasRadar"))
                ));
            }
        }

        return planes;
    }
}
