
package org.scilab.modules.xcos.simulink.patterns;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GeneralParameters complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GeneralParameters">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="string" type="{}StringParameters" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;element name="integer" type="{}IntegerParameters" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;element name="simple" type="{}SimpleParameter" maxOccurs="unbounded" minOccurs="0"/>
 *       &lt;/sequence>
 *       &lt;attribute name="description" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GeneralParameters", propOrder = {
    "string",
    "integer",
    "simple"
})
public class GeneralParameters {

    protected List<StringParameters> string;
    protected List<IntegerParameters> integer;
    protected List<SimpleParameter> simple;
    @XmlAttribute
    protected String description;

    /**
     * Gets the value of the string property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the string property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getString().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link StringParameters }
     * 
     * 
     */
    public List<StringParameters> getString() {
        if (string == null) {
            string = new ArrayList<StringParameters>();
        }
        return this.string;
    }

    /**
     * Gets the value of the integer property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the integer property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getInteger().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link IntegerParameters }
     * 
     * 
     */
    public List<IntegerParameters> getInteger() {
        if (integer == null) {
            integer = new ArrayList<IntegerParameters>();
        }
        return this.integer;
    }

    /**
     * Gets the value of the simple property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the simple property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getSimple().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link SimpleParameter }
     * 
     * 
     */
    public List<SimpleParameter> getSimple() {
        if (simple == null) {
            simple = new ArrayList<SimpleParameter>();
        }
        return this.simple;
    }

    /**
     * Gets the value of the description property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDescription() {
        return description;
    }

    /**
     * Sets the value of the description property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDescription(String value) {
        this.description = value;
    }

}