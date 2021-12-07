package parsers;

import org.w3c.dom.Element;

interface XMLBuilder<T> {

    T buildByDOM(Element element);
    T buildBySAX(String data, String tag, String qName);
}
