
package org.scilab.modules.xcos.simulink.patterns;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for SimpleParameter complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="SimpleParameter">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="sim" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="xcos" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="description" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "SimpleParameter")
public class SimpleParameter {

    @XmlAttribute(required = true)
    protected String sim;
    @XmlAttribute(required = true)
    protected String xcos;
    @XmlAttribute
    protected String description;

    /**
     * Gets the value of the sim property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSim() {
        return sim;
    }

    /**
     * Sets the value of the sim property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSim(String value) {
        this.sim = value;
    }

    /**
     * Gets the value of the xcos property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getXcos() {
        return xcos;
    }

    /**
     * Sets the value of the xcos property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setXcos(String value) {
        this.xcos = value;
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