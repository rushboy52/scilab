/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP void matz_cath(scicos_block *block,int flag)
{
	if ((flag==1) || (flag==6))
	{
		int mu =GetInPortRows(block,1);
		double *yr=GetRealOutPortPtrs(block,1);
		double *yi=GetImagOutPortPtrs(block,1);

		int j = 0;
		for(j=0;j<mu;j++)
		{
			int k=j;
			int bk = 0;
			for (bk=1;bk<GetNin(block)+1;bk++) 
			{
				double *ur=GetRealInPortPtrs(block,bk);
				double *ui=GetImagInPortPtrs(block,bk);
				int nu = GetInPortCols(block,bk);
				int i = 0;
				for(i=0;i<nu;i++)
				{
					int ij=j+i*mu;
					yr[k]=ur[ij];
					yi[k]=ui[ij];
					k+= mu;
				}
			}
		}
	}
}
/*--------------------------------------------------------------------------*/ 
