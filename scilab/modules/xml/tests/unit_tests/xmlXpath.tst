// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================


xmlFile=xmlRead(SCI+"/modules/xml/tests/unit_tests/w3cExample.xml");
titles=xmlXPath(xmlFile, "/bookstore/book/title");
assert_checkequal(titles.size,5);
assert_checkequal(titles(4).name,"title");
assert_checkequal(titles(4).content,"Learning XML");
assert_checkequal(titles(4).type,"XML_ELEMENT_NODE");
assert_checkequal(titles(5).name,"title");
assert_checkequal(titles(5).content,"Scilab rox");
assert_checkequal(titles(5).type,"XML_ELEMENT_NODE");
attribs=titles(4).attributes;
assert_checkequal(titles(4).attributes.size,[]);
assert_checkequal(attribs.lang,"en");

// Get all the books where the price is more than 35 euros
moreThan35eList=xmlXPath(xmlFile,"/bookstore/book[price>35]/title");
assert_checkequal(moreThan35eList.size,2);
assert_checkequal(size(moreThan35eList),[1,2]); 
assert_checkequal(moreThan35eList(1).content,"XQuery Kick Start");
assert_checkequal(moreThan35eList(2).content,"Learning XML");

// Get all the free books 
freeBooks=xmlXPath(xmlFile,"/bookstore/book[price=0]/title");
assert_checkequal(freeBooks.size,1);
assert_checkequal(size(freeBooks),[1,1]); 
assert_checkequal(freeBooks(1).content,"Scilab rox");

xmlClose(xmlFile);

xmlFile=xmlRead(SCI+"/modules/xml/tests/unit_tests/sep_69_example.xml");
titles=xmlXPath(xmlFile, "/root/hello");
assert_checkequal(titles.size,2);
xmlClose(xmlFile);

xmlFile=xmlRead(SCI+"/etc/modules.xml");
content=xmlDump(xmlFile);
assert_checktrue(length(content)>0);
assert_checktrue(size(content)>=[1,1]);
xmlClose(xmlFile);
// Close the file a second time
assert_checkerror("xmlClose(xmlFile)",gettext("xmlClose: XML document does not exist."));


xmlClasspath=xmlRead(SCI+"/etc/classpath.xml");
jarPath=xmlXPath(xmlClasspath, "//classpaths/path[@load=''onUse'']/load");
assert_checktrue(length(jarPath(1).attributes.on)>0); // Should be Console
// if the order of classpath.xml does not change
assert_checkequal(grep(jarPath(1).parent.attributes.value,".jar"),1);
assert_checkequal(jarPath(1).parent.attributes.load,"onUse");

jarPath=xmlXPath(xmlClasspath, "//classpaths/path[@load!=''onUse'']");
assert_checktrue(length(jarPath(1).attributes.value)>0);
assert_checkequal(jarPath(1).attributes.load,"startup");
assert_checkequal(jarPath(1).parent.name,"classpaths"); // it is the root dir


xmlClose(xmlClasspath);

xmlFile=xmlRead(SCI+"/etc/modules.xml");
xmlFile2=xmlRead(SCI+"/etc/classpath.xml");
content=xmlDump(xmlFile);
content2=xmlDump(xmlFile2);
//assert_checkequal(content, content2);
assert_checktrue(length(content)>0);
assert_checktrue(size(content)>=[1,1]);
assert_checktrue(length(content2)>0);
assert_checktrue(size(content2)>=[1,1]);
xmlClose(xmlFile);
xmlClose(xmlFile2);
// Close the file a second time
assert_checkerror("xmlClose(xmlFile)","xmlClose: XML document does not exist.");
