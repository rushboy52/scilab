/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <vector>

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "xml_constants.h"
#include "localization.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/

/**
 * Creates a string on stack
 * @param fname the function name
 * @param str the string to put
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createStringOnStack(char * fname, const char * str, int pos)
{
    SciErr err;

    if (!str)
    {
        str = "";
    }

    if (strchr(str, '\n'))
    {
        char * tok = strdup(str);
        char * stok = tok;
        std::vector<char *> vector = std::vector<char *>();

        tok = strtok(tok, "\n");
        while (tok)
        {
            vector.push_back(tok);
            tok = strtok(0, "\n");
        }

        if (vector.size())
        {
            err = createMatrixOfString(pvApiCtx, pos, (int)vector.size(), 1, const_cast<const char * const *>(&(vector[0])));
        }
        else
        {
            err = createMatrixOfDouble(pvApiCtx, pos, 0, 0, 0);
        }

        free(stok);
    }
    else
    {
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, const_cast<const char * const *>(&str));
    }

    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
/**
 * Creates a new variable on stack according to the requested field
 * @param fname the function name
 * @param doc the document
 * @param field the field name
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createVariableOnStack(char * fname, org_modules_xml::XMLDocument & doc, const char * field, int pos)
{
    if (!strcmp("root", field))
    {
        return doc.getRoot()->createOnStack(pos);
    }
    else if (!strcmp("url", field))
    {
        return createStringOnStack(fname, doc.getDocumentURL(), pos);
    }
    else
    {
        Scierror(999, gettext("%s: Unknown field: %s\n"), fname, field);
        return 0;
    }
}
/*--------------------------------------------------------------------------*/

/**
 * Creates a new variable on stack according to the requested field
 * @param fname the function name
 * @param elem the element
 * @param field the field name
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createVariableOnStack(char * fname, XMLElement & elem, const char * field, int pos)
{
    if (!strcmp("name", field))
    {
        return createStringOnStack(fname, elem.getNodeName(), pos);
    }
    else if (!strcmp("namespace", field))
    {
        const XMLNs * ns = elem.getNodeNameSpace();
        if (ns)
        {
            return ns->createOnStack(pos);
        }
        else
        {
            createMatrixOfDouble(pvApiCtx, pos, 0, 0, 0);
            return 1;
        }
    }
    else if (!strcmp("content", field))
    {
        const char * content = elem.getNodeContent();
        int ret = createStringOnStack(fname, content, pos);
        xmlFree(const_cast<char *>(content));
        return ret;
    }
    else if (!strcmp("type", field))
    {
        return createStringOnStack(fname, nodes_type[elem.getNodeType() - 1], pos);
    }
    else if (!strcmp("parent", field))
    {
        const XMLElement * parent = elem.getParentElement();
        if (parent)
        {
            return parent->createOnStack(pos);
        }
        else
        {
            createMatrixOfDouble(pvApiCtx, pos, 0, 0, 0);
            return 1;
        }
    }
    else if (!strcmp("attributes", field))
    {
        return elem.getAttributes()->createOnStack(pos);
    }
    else if (!strcmp("children", field))
    {
        return elem.getChildren()->createOnStack(pos);
    }
    else
    {
        Scierror(999, gettext("%s: Unknown field: %s\n"), fname, field);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

/**
 * Creates a new variable on stack according to the requested field
 * @param fname the function name
 * @param ns the namespace
 * @param field the field name
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createVariableOnStack(char * fname, XMLNs & ns, const char * field, int pos)
{
    if (!strcmp("href", field))
    {
        return createStringOnStack(fname, ns.getHref(), pos);
    }
    else if (!strcmp("prefix", field))
    {
        return createStringOnStack(fname, ns.getPrefix(), pos);
    }
    else
    {
        Scierror(999, gettext("%s: Unknown field: %s\n"), fname, field);
        return 0;
    }
}
/*--------------------------------------------------------------------------*/

/**
 * Function to handle extraction in different XMLObjects
 * @param fname the function name
 * @param fname_len the function name length
 */
template<class T>
int sci_extraction(char * fname, unsigned long fname_len)
{
    T * t;
    int id;
    SciErr err;
    int * fieldaddr = 0;
    int * mlistaddr = 0;
    char * field = 0;
    int ret;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &fieldaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, fieldaddr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    getAllocatedSingleString(pvApiCtx, fieldaddr, &field);
    id = getXMLObjectId(mlistaddr);

    t = XMLObject::getFromId<T>(id);
    if (!t)
    {
        freeAllocatedSingleString(field);
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    ret = createVariableOnStack(fname, *t, const_cast<char *>(field), Rhs + 1);
    freeAllocatedSingleString(field);
    if (ret)
    {
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        LhsVar(1) = 0;
    }
    PutLhsVar();

    return 0;
}