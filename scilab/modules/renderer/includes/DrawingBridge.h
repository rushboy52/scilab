/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Functions to create and access a drawer of graphic handles
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWING_BRIDGE_H_
#define _DRAWING_BRIDGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_renderer.h"
#include "ObjectStructure.h"

  /**
   * Create a render for this object
   */
  RENDERER_IMPEXP void createDrawer( sciPointObj * pObj ) ;

  /**
   * Delete the drawer of a graphic handle
   */
  RENDERER_IMPEXP void destroyHandleDrawer( sciPointObj * pObj ) ;

  /**
   * Draw a graphic handle.
   * Should normally not be called directly. Observers should call it automatically.
   */
  RENDERER_IMPEXP void sciDrawObj( sciPointObj * pObj ) ;

  /**
   * Move a graphic handle
   */
  RENDERER_IMPEXP void sciMoveObj(sciPointObj * pObj, const double translation[3]);

  /**
  * Draw a single graphic handle.
  * Should normally not be called directly. Observers should call it automatically.
  */
  RENDERER_IMPEXP void sciDrawSingleObj( sciPointObj * pObj ) ;

  /**
   * Draw only a set of graphic handles.
   * @param pObjs an array of objects lying within the same figure
   */
  RENDERER_IMPEXP void sciDrawSetOfObj(sciPointObj * pObjs[], int nbObjs ) ;

  /**
   * Redraw an object and all its children.
   * To be called on a figure to redraw a whole window
   */
  RENDERER_IMPEXP void redrawHierarchy( sciPointObj * pObj ) ;

  /**
   * Call display function on the children of an object
   */
  RENDERER_IMPEXP void displayChildren( sciPointObj * pObj ) ;

  /**
   * Next call to display will force this object and its children to be drawn.
   */
  RENDERER_IMPEXP void forceHierarchyRedraw( sciPointObj * pObj ) ;

  /**
   * Specify that an object has been modified and need to be redrawn.
   * To be used when several objects are modified in the same time.
   */
  RENDERER_IMPEXP void forceRedraw(sciPointObj * pObj);

  /**
   * Force a move an object.
   * The move is schedule for the next redraw
   */
  RENDERER_IMPEXP void forceMove(sciPointObj * pObj, double tx, double ty, double tz);

#ifdef __cplusplus
}
#endif

#endif /* _DRAWING_BRIDGE_H_  */