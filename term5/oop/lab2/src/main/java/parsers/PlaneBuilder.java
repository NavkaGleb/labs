package parsers;

import org.w3c.dom.Element;
import util.Plane;

public class PlaneBuilder implements XMLBuilder<Plane> {

    private Plane current = new Plane();

    @Override
    public Plane buildByDOM(Element element) {
        return new Plane(
            getFieldFromElement(element, "model"),
            getFieldFromElement(element, "origin"),
            new Plane.Specifications(
                Plane.Type.valueOf(getFieldFromElement(element, "type")),
                Integer.parseInt(getFieldFromElement(element, "seatCount")),
                Integer.parseInt(getFieldFromElement(element, "missileCount"))
            ),
            Boolean.parseBoolean(getFieldFromElement(element, "hasRadar"))
        );
    }

    @Override
    public Plane buildBySAX(String data, String tag, String qName) {
        switch (tag) {
            case "model" -> current.setModel(data);
            case "origin" -> current.setOrigin(data);
            case "type" -> current.getSpecifications().setType(Plane.Type.valueOf(data));
            case "seatCount" -> current.getSpecifications().setSeatCount(Integer.parseInt(data));
            case "missileCount" -> current.getSpecifications().setMissileCount(Integer.parseInt(data));
            case "hasRadar" -> current.setRadar(Boolean.parseBoolean(data));
        }

        if (qName.equalsIgnoreCase("plane")) {
            var result = current;
            current = new Plane();

            return result;
        }

        return null;
    }

    private static String getFieldFromElement(Element element, String key) {
        return element.getElementsByTagName(key).item(0).getTextContent();
    }
}
