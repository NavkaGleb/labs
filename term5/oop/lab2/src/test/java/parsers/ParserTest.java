package parsers;

import org.junit.jupiter.api.Test;

import util.Plane;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class ParserTest {
    public static final String      XML_INVALID_PATH = "src/main/resources/invalid_planes.xml";
    public static final String      XML_VALID_PATH = "src/main/resources/planes.xml";
    public static final String      XSD_PATH = "src/main/resources/planes.xsd";
    public static final List<Plane> PLANES = List.of(
        new Plane(
            "model",
            "origin",
            new Plane.Specifications(
                Plane.Type.ESCORT,
                2,
                0
            ),
            false
        ),
        new Plane(
            "new model",
            "new origin",
            new Plane.Specifications(
                Plane.Type.FIGHTER,
                1,
                10
            ),
            true
        ));

    void invalidTest(XMLParser<Plane> parser) {
        assertThrows(RuntimeException.class, () -> parser.parse(XML_INVALID_PATH, XSD_PATH, new PlaneBuilder()));
    }

    void validTest(XMLParser<Plane> parser) {
        try {
            List<Plane> planes = parser.parse(XML_VALID_PATH, XSD_PATH, new PlaneBuilder());

            assertEquals(PLANES.size(), planes.size());

            for (int i = 0; i < PLANES.size(); i++) {
                var expected = PLANES.get(i);
                var actual = planes.get(i);

                assertEquals(expected.getModel(), actual.getModel());
                assertEquals(expected.getOrigin(), actual.getOrigin());
                assertEquals(expected.getSpecifications().getType(), actual.getSpecifications().getType());
                assertEquals(expected.getSpecifications().getSeatCount(), actual.getSpecifications().getSeatCount());
                assertEquals(expected.getSpecifications().getMissileCount(), actual.getSpecifications().getMissileCount());
                assertEquals(expected.hasRadar(), actual.hasRadar());
            }
        } catch (Exception exception) {
            throw new RuntimeException();
        }
    }

    @Test
    void domParserTest() {
        invalidTest(new DOMParser<>());
        validTest(new DOMParser<>());
    }

    @Test
    void saxParserTest() {
        invalidTest(new SAXParser<>());
        validTest(new SAXParser<>());
    }
}
