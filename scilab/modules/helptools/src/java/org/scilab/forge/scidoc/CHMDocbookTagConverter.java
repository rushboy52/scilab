/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.forge.scidoc;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import org.xml.sax.SAXException;

/**
 * Class to convert DocBook to CHM
 * @author Calixte DENIZET
 */
public class CHMDocbookTagConverter extends HTMLDocbookTagConverter {

    private StringBuilder buffer = new StringBuilder(8192);
    private String outName;
    private List<String> filesList = new ArrayList();

    /**
     * Constructor
     * @param inName the name of the input stream
     * @param primConf the file containing the primitives of Scilab
     * @param macroConf the file containing the macros of Scilab
     * @param out the output stream
     */
    public CHMDocbookTagConverter(String inName, String outName, String primConf, String macroConf, String template, String version, String imageDir, boolean checkLast) throws IOException, SAXException {
        super(inName, outName, primConf, macroConf, template, version, imageDir, checkLast);
        this.outName = new File(outName).getCanonicalPath() + File.separator;
    }

    /**
     * {@inheritDoc}
     */
    public void createHTMLFile(String id, String fileName, String subtitle, String contents) {
        super.createHTMLFile(id, fileName, subtitle, contents);
        filesList.add(fileName);
    }

    /**
     * {@inheritDoc}
     */
    public String makePrevious(String id) {
        buffer.setLength(0);
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "<link rel=\"prev\" href=\"\" title=\"\">";
        }
        HTMLDocbookLinkResolver.TreeId prev = leaf.getPrevious();
        if (prev.parent != null) {
            buffer.append("<link rel=\"prev\" href=\"");
            buffer.append(mapId.get(prev.id));
            buffer.append("\" title=\"");
            buffer.append(tocitem.get(prev.id));
            buffer.append("\">");

            return buffer.toString();
        }

        return "<link rel=\"prev\" href=\"\" title=\"\">";
    }

    /**
     * {@inheritDoc}
     */
    public String makeTop(String id) {
        buffer.setLength(0);
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "<link rel=\"up\" href=\"\" title=\"\">";
        }
        HTMLDocbookLinkResolver.TreeId prev = leaf.getPrevious();
        if (prev.parent != null) {
            buffer.append("<link rel=\"up\" href=\"");
            if (!leaf.isRoot()) {
                buffer.append(mapId.get(leaf.id));
                buffer.append("\" title=\"");
                buffer.append(tocitem.get(leaf.id));
            } else {
                buffer.append(indexFilename);
                buffer.append("\" title=\"");
                buffer.append(bookTitle);
            }
            buffer.append("\">");

            return buffer.toString();
        }

        return "<link rel=\"up\" href=\"\" title=\"\">";
    }

    /**
     * {@inheritDoc}
     */
    public String makeNext(String id) {
        buffer.setLength(0);
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "<link rel=\"next\" href=\"\" title=\"\">";
        }
        HTMLDocbookLinkResolver.TreeId next = leaf.getNext();
        if (next != null) {
            buffer.append("<link rel=\"next\" href=\"");
            buffer.append(mapId.get(next.id));
            buffer.append("\" title=\"");
            buffer.append(tocitem.get(next.id));
            buffer.append("\">");

            return buffer.toString();
        }

        return "<link rel=\"next\" href=\"\" title=\"\">";
    }

    /**
     * {@inheritDoc}
     */
    public String makeStart(String id) {
        buffer.setLength(0);
        buffer.append("<link rel=\"start\" href=\"");
        buffer.append(indexFilename);
        buffer.append("\" title=\"");
        buffer.append(bookTitle);
        buffer.append("\">");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public void endDocument() throws SAXException {
        try {
            FileOutputStream outToc = new FileOutputStream("toc.hhc");
            FileOutputStream outFiles = new FileOutputStream("htmlhelp.hhp");
            OutputStreamWriter writerFiles = new OutputStreamWriter(outFiles, Charset.forName("UTF-8"));
            OutputStreamWriter writerToc = new OutputStreamWriter(outToc, Charset.forName("UTF-8"));
            convertTree(writerToc);
            writerToc.flush();
            writerToc.close();
            outToc.flush();
            outToc.close();

            convertFileList(writerFiles);
            writerFiles.flush();
            writerFiles.close();
            outFiles.flush();
            outFiles.close();
        } catch (IOException e) {
            exceptionOccured(e);
        }
    }

    private void convertFileList(Appendable buffer) throws IOException {
        // Allan : si tu penses que l'on peut faire un truc plus configurable, dis-moi je ferais un fichier modele
        // que l'on remplira ensuite avec les bonnes options...
        buffer.append("[OPTIONS]\n");
        buffer.append("Binary TOC=Yes\n");
        buffer.append("Compatibility=1.1 or later\n");
        buffer.append("Compiled file=htmlhelp.chm\n");
        buffer.append("Contents file=");
        buffer.append(outName);
        buffer.append("toc.hhc\n");
        buffer.append("Default Window=Main\n");
        buffer.append("Default topic=");
        buffer.append(outName);
        buffer.append("index.html\n");
        buffer.append("Display compile progress=Yes\n");
        buffer.append("Full-text search=Yes\n");
        buffer.append("Language=");
        buffer.append("0x0409");// Allan : code pr la langue
        buffer.append("\nTitle=");
        buffer.append(bookTitle);
        buffer.append("\nEnhanced decompilation=No\n\n");
        buffer.append("[WINDOWS]\n");
        buffer.append("Main=\"");
        buffer.append(bookTitle);
        buffer.append("\",\",");
        buffer.append(outName);
        buffer.append("toc.hhc\",,\"");
        buffer.append(outName);
        buffer.append("index.html\",\"");
        buffer.append(outName);
        buffer.append("index.html\",,,,,0x2520,,0x603006,,,,,,,,0\n\n[FILES]\n");
        for (String s : filesList) {
            buffer.append(outName);
            buffer.append(s);
            buffer.append("\n");
        }
    }

    private void convertTreeId(HTMLDocbookLinkResolver.TreeId leaf, Appendable buffer) throws IOException {
        if (leaf.children != null) {
            for (HTMLDocbookLinkResolver.TreeId c : leaf.children) {
                buffer.append("<LI><OBJECT type=\"text/sitemap\">\n<param name=\"Name\" value=\"");
                buffer.append(tocitem.get(c.id));
                buffer.append("\">\n<param name=\"Local\" value=\"");
                buffer.append(outName);
                buffer.append(mapId.get(c.id));
                buffer.append("\">\n</OBJECT>\n</LI>\n");
                if (c.children != null) {
                    buffer.append("<UL>\n");
                    convertTreeId(c, buffer);
                    buffer.append("</UL>\n");
                }
            }
        }
    }

    private void convertTree(Appendable buffer) throws IOException {
        buffer.append("<HTML>\n<HEAD>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n</HEAD>\n<BODY>\n<OBJECT type=\"text/site properties\">\n<param name=\"ImageType\" value=\"Folder\">\n</OBJECT>\n");
        buffer.append("<UL>\n<LI><OBJECT type=\"text/sitemap\">\n<param name=\"Name\" value=\"");
        buffer.append(bookTitle);
        buffer.append("\">\n<param name=\"Local\" value=\"");
        buffer.append(outName);
        buffer.append("index.html\">\n</OBJECT>\n</LI>\n<UL>\n");
        convertTreeId(tree, buffer);
        buffer.append("</UL>\n</UL>\n</BODY>\n</HTML>");
    }
}