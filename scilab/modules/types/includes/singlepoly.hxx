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

#ifndef __POLY_HH__
#define __POLY_HH__

#include <list>
#include "types.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP SinglePoly : public GenericType
    {
    public :
                                SinglePoly();
                                SinglePoly(double** _pdblCoefR, int _iRank);
                                SinglePoly(double** _pdblCoefR, double** _pdblcoefI, int _iRank);
                                SinglePoly(Double** _poCoefR, int _iRank);
        virtual                 ~SinglePoly();

        // FIXME : Should not return NULL;
        SinglePoly*             clone();

        SinglePoly*             getAsSinglePoly(void);
        bool                    isSinglePoly() { return true; }
        /*Config management*/
        void                    whoAmI();
        bool                    isComplex();
        void                    setComplex(bool _bComplex);

        int                     getRank();
        bool                    setRank(int _iRank, bool bSave = false);
        Double*                 getCoef();
        double*                 getCoefReal();
        double*                 getCoefImg();
        bool                    setCoef(Double *_poPow);
        bool                    setCoef(double *_pdblCoefR, double *_pdblCoefI);
        bool                    evaluate(double _dblInR, double _dblInI, double *_pdblOutR, double *_pdblOutI);
        void                    updateRank(void);

        GenericType*            getColumnValues(int _iPos);
        void                    createPoly(double**_pdblCoefR, double**_pdblCoefI, int _iRank);
        void                    toStringReal(int _iPrecision, int _iLineLen, std::wstring _szVar, std::list<std::wstring>* _pListExp , std::list<std::wstring>* _pListCoef);
        void                    toStringImg(int _iPrecision, int _iLineLen, std::wstring _szVar, std::list<std::wstring>* _pListExp , std::list<std::wstring>* _pListCoef);

        std::wstring            toString(int _iPrecision, int _iLineLen);

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"poly";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"p";}
    protected :
        RealType                getType(void);

    private :
        void                    toStringInternal(double *_pdblVal, int _iPrecision, int _iLineLen, std::wstring _szVar, std::list<std::wstring>* _pListExp , std::list<std::wstring>* _pListCoef);


    private :
        bool                    m_bComplex;
        Double*                 m_pdblCoef;
        int                     m_iRank;
    };
}

#endif /* !__POLY_HH__ */