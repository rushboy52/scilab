/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "scinotes_gw.hxx"
#include "funcmanager.hxx"
#include "context.hxx"

#define MODULE_NAME L"scinotes"

extern "C"
{
#include "gw_scinotes.h"
#include "loadOnUseClassPath.h"
}

bool ScinotesModule::loadedDep = false;

void ScinotesModule::LoadDeps(void)
{
    if (loadedDep == false)
    {
        loadOnUseClassPath("SciNotes");
        loadedDep = true;
    }
}

int ScinotesModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"editor", &sci_scinotes, &ScinotesModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"closeEditor", &sci_closeSciNotesFromScilab, &ScinotesModule::LoadDeps, MODULE_NAME));
    
    return 1;
}