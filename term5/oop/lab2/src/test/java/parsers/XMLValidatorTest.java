package parsers;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class ValidatorXMLTest {
    public static final String XML_INVALID_PATH = "src/main/resources/invalid_planes.xml";
    public static final String XML_VALID_PATH = "src/main/resources/planes.xml";
    public static final String XSD_PATH = "src/main/resources/planes.xsd";

    @Test
    void invalidTest() {
        Assertions.assertThrows(
            RuntimeException.class, () -> XMLValidator.validateAgainstXSD(XML_INVALID_PATH, XSD_PATH)
        );
    }

    @Test
    void validTest() {
        XMLValidator.validateAgainstXSD(XML_VALID_PATH, XSD_PATH);
    }
}
