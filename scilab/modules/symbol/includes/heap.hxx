/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef SYMBOL_HEAP_HH
#define SYMBOL_HEAP_HH

#include <iostream>
#include <list>
#include "symbol.hxx"
#include "table.hxx"

#include "bool.hxx"
#include "double.hxx"

#define UNNAMED L"unnamed"

namespace symbol
{

/*-----------------------------------------------------------------------.
| This implements heaps for storing definitions of variables and         |
| functions as a list of dictionnaries.  Each time a scope is opened, a  |
| new dictionnary is added on the top of the heap; the dictionary is     |
| removed when the scope is closed.  Lookup of symbols is donne in the   |
| last added dictionnary first (LIFO).                                   |
`-----------------------------------------------------------------------*/
    class EXTERN_SYMBOL Heap : public Table
    {
    public:

        types::Bool *m_True;
        /**

        */
        Heap()
        {
            scope_begin(UNNAMED);
            m_True = new types::Bool(1);
            m_True->IncreaseRef();
        }

        /** Open a new scope */
        void scope_begin(const std::wstring& name)
        {
            this->l_scope.push_front(new Scope(name));
        }

        /** Open a new scope */
        void scope_begin()
        {
            this->l_scope.push_front(new Scope(UNNAMED));
        }

        /** Close the last scope, forgetting everything since the latest
        **	scope_begin (). */
        void scope_end()
        {
            Scope* scope = this->l_scope.front();
            this->l_scope.pop_front();
            delete scope;
        }

        /*
        ** Global namespace direct access.
        ** {
        */

        /** Associate value to key in the global scope. */
        void put(const symbol::Symbol& key, types::InternalType &value)
        {
            (*this->l_scope.back()).put(key, value);
        }

        /** \brief If key was associated to some Entry_T in
        ** the "global"/last namespace, return the
        ** most recent insertion. Otherwise return the empty pointer.
        ** \param key : the key to look for
        */
        types::InternalType* get(const symbol::Symbol& key) const
        {
            types::InternalType* result = 0;

            result = (*this->l_scope.back()).get(key);
            return result;
        }

        /*
        ** }
        ** END Global Namespace
        */

        /** \brief Associate a value to a key in the given namespace.
        ** If the Namespace does not exists it will be automatically
        ** created.
        ** \param name : the name of the namespace.
        ** \param key : the key.
        ** \param value : the value associated to the key.
        */
        void put(const std::wstring& name, symbol::Symbol& key, types::InternalType &value)
        {
            std::list<Scope*>::iterator it_list_scope;

            for (it_list_scope = this->l_scope.begin() ; it_list_scope != this->l_scope.end(); ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == name)
                {
                    (*it_list_scope)->put(key, value);
                    return ;
                }
            }
            scope_begin(name);
            (*this->l_scope.front()).put(key, value);
        }

        /** \brief Check if key is visible in the current global scope
        ** \param key : the key to look for
        */
        bool isGlobalVisible(const symbol::Symbol& key) const
        {
            std::list<Scope*>::const_iterator it_list_scope;

            for (it_list_scope = this->l_scope.begin() ; it_list_scope != this->l_scope.end() ; ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == UNNAMED)
                {
                    types::InternalType* pIT = (*it_list_scope)->get(key);
                    if(pIT != NULL)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            return false;
        }

        /** \brief Check if key exists in globals
        ** \param key : the key to look for
        */
        bool isGlobalExists(const symbol::Symbol& key) const
        {
            types::InternalType* pIT = getGlobalValue(key);
            if(pIT)
            {
                return true;
            }
            return false;
        }

        /** \brief Return value associated to key in current global scope
        ** \param key : the key to look for
        */
        types::InternalType* getGlobalValue(const symbol::Symbol& key) const
        {
            std::list<Scope*>::const_reverse_iterator it_list_scope;

            for (it_list_scope = this->l_scope.rbegin() ; it_list_scope != this->l_scope.rend() ; ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == UNNAMED)
                {
                    return (*it_list_scope)->get(key);
                }
            }
            return NULL;
        }

        /** \brief associate value to key in current global scope
        ** \param key : the key to look for
        ** \param value : the value to associete
        */
        void setGlobalValue(const symbol::Symbol& key, types::InternalType &value)
        {
            std::list<Scope*>::reverse_iterator it_list_scope;

            for (it_list_scope = this->l_scope.rbegin() ; it_list_scope != this->l_scope.rend() ; ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == UNNAMED)
                {
                    (*it_list_scope)->put(key, value);
                    break;
                }
            }
        }

        /** \brief Create a "empty" global value
        ** \param key : the key to create
        */
        void createEmptyGlobalValue(const symbol::Symbol& key)
        {
            setGlobalValue(key, *types::Double::Empty());
        }


        /** \brief Change visibility of a global variable
        ** \param key : the key to manage
        ** \param bVisible : status ( true to visible, false to hide )
        */
        void setGlobalVisible(const symbol::Symbol& key, bool bVisible)
        {
            std::list<Scope*>::const_iterator it_list_scope;

            for (it_list_scope = this->l_scope.begin() ; it_list_scope != this->l_scope.end() ; ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == UNNAMED)
                {
                    if(bVisible)
                    {
                        (*it_list_scope)->put(key, *m_True);
                    }
                    else
                    {
                        (*it_list_scope)->remove(key);
                    }
                    break;
                }
            }
        }

        /** \brief Remove a variable from global scope
        ** \param key : the key to remove
        */
        void removeGlobal(const symbol::Symbol& key)
        {
            //remove variable and all variable references
            std::list<Scope*>::const_iterator it_list_scope;
            for (it_list_scope = this->l_scope.begin() ; it_list_scope != this->l_scope.end() ; ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == UNNAMED)
                {
                    (*it_list_scope)->remove(key);
                }
            }
        }

        /** \brief Remove all variables from global scope
        */
        void removeGlobalAll()
        {
            std::list<Scope*>::const_iterator it_list_scope;
            for (it_list_scope = this->l_scope.begin() ; it_list_scope != this->l_scope.end() ; ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == UNNAMED)
                {
                    std::map<symbol::Symbol, types::InternalType*>::const_iterator it_scope;
                    std::map<symbol::Symbol, types::InternalType*>* pScope = (*it_list_scope)->getInternalMap();

                    it_scope = pScope->begin();
                    while(it_scope != pScope->end())
                    {
                        (*it_list_scope)->remove(it_scope->first);
                        it_scope = pScope->begin();
                    }
                }
            }
        }

        /** \brief If key was associated to some Entry_T in
        ** the "global"/last namespace, return the
        ** most recent insertion. Otherwise return the empty pointer.
        ** \param key : the key to look for
        */
        types::InternalType* get(const std::wstring& name, const symbol::Symbol& key) const
        {
            std::list<Scope*>::const_iterator it_list_scope;

            for (it_list_scope = this->l_scope.begin() ; it_list_scope != this->l_scope.end() ; ++it_list_scope)
            {
                if ((*it_list_scope)->get_name() == name)
                {
                    return (*it_list_scope)->get(key);
                }
            }
            return NULL;
        }
    };

    inline std::wostream& operator<< (std::wostream& ostr, const Heap &tbl)
    {
        tbl.print (ostr);
        return ostr;
    }
}


#endif /* ! SYMBOL_HEAP_HH */
