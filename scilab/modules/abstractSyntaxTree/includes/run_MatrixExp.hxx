/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This code is separated in run_MatrixExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

/*
  [1,2;3,4] with/without special character $ and :
*/
void visitprivate(const MatrixExp &e)
{
    try
    {
//			int iRows = e.lines_get().size();
//			int iCols	= -1;
        int iRows = 0;
        int iCols	= -1;
        int iCurRow = -1;
        int iCurCol = 0;
        InternalType *poResult = NULL;

        std::list<MatrixLineExp *>::const_iterator	row;
        std::list<Exp *>::const_iterator	col;
        //store all element after evaluation
        if(e.lines_get().size() == 0)
        {
            poResult = Double::Empty();
        }
        else
        {
            list<list<InternalType*> > MatrixList;
            for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
            {
                list<InternalType*> RowList;
                for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; ++col)
                {
                    T* execMe = new T();
                    CommentExp* pComment = dynamic_cast<CommentExp*>((*col));
                    if(pComment != NULL)
                    {
                        execMe->result_set(Double::Empty());
                    }
                    else
                    {
                        (*col)->accept (*execMe);
                    }

                    if(execMe->result_get()->isImplicitList())
                    {
                        if(execMe->result_get()->getAsImplicitList()->isComputable() == true)
                        {
                            execMe->result_set(execMe->result_get()->getAsImplicitList()->extractFullMatrix());
                            iCurCol += ((GenericType*)execMe->result_get())->getCols();
                        }
                        else
                        {
                            iCurCol++;
                        }
                    }
                    else
                    {
                        iCurCol += ((GenericType*)execMe->result_get())->getCols();
                    }

                    if(iCurRow == -1)
                    {
                        iCurRow = ((GenericType*)execMe->result_get())->getRows();
                        if(iCurRow == 0)
                        {//to manage [] in MatrixExp
                            iCurRow = -1;
                        }
                    }
                    else if(iCurRow != ((GenericType*)execMe->result_get())->getRows() && ((GenericType*)execMe->result_get())->getRows() != 0)
                    {
                        std::wostringstream os;
                        os << L"inconsistent row/column dimensions\n";
                        //os << ((Location)(*row)->location_get()).location_getString() << std::endl;
                        throw ScilabError(os.str(), 999, (*row)->location_get());
                    }

                    InternalType *pResult = execMe->result_get();
                    RowList.push_back(pResult);

                    //tips to delete only execvisitor but not data
                    pResult->IncreaseRef();
                    delete execMe;
                    pResult->DecreaseRef();
                }

                //if empty row, ignore it
                if(iCurCol != 0)
                {
                    if(iCols == -1)
                    {
                        iCols = iCurCol;
                        if(iCols == 0)
                        {//manage []
                            iCols = -1;
                        }
                    }
                    else if(iCols != 0 && iCols != iCurCol)
                    {
                        std::wostringstream os;
                        os << L"inconsistent row/column dimensions\n";
                        //os << ((Location)(*row)->location_get()).location_getString() << std::endl;
                        throw ScilabError(os.str(), 999, (*row)->location_get());
                    }

                    iRows += iCurRow;
                    if(iRows == -1)
                    {//manage  []
                        iRows = 0;
                    }
                    iCurCol = 0;
                    iCurRow = -1;
                    MatrixList.push_back(RowList);
                }
            }

            list<list<InternalType*> >::const_iterator it_ML;
            list<InternalType*>::const_iterator it_RL;

            int iAddRow = 0;
            iCurRow			= 0;
            iCurCol			= 0;
            for(it_ML = MatrixList.begin() ; it_ML != MatrixList.end() ; it_ML++)
            {
                int iAddCol = 0;
                for(it_RL = (*it_ML).begin() ; it_RL != (*it_ML).end() ; it_RL++)
                {
                    if((*it_RL)->isDouble() && (*it_RL)->getAs<Double>()->isEmpty())
                    {//manage []
                        continue;
                    }

                    if(poResult == NULL)
                    {
                        poResult = AddElementToVariable(poResult, *it_RL, iRows, iCols, &iAddRow, &iAddCol);

                        if((*it_RL)->isDeletable() == true)
                        {
                            delete (*it_RL);
                        }
                        iCurCol += iAddCol;
                    }
                    else
                    {
                        poResult = AddElementToVariable(poResult, *it_RL, iCurRow, iCurCol, &iAddRow, &iAddCol);
                        if((*it_RL)->isDeletable() == true)
                        {
                            delete (*it_RL);
                        }
                        iCurCol += iAddCol;
                    }
                }
                iCurRow += iAddRow;
                iCurCol = 0;
            }
        }

        if (poResult == NULL)
        {
            poResult = Double::Empty();
        }

        result_set(poResult);
    }
    catch(ScilabError error)
    {
        throw error;
    }
}