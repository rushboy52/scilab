c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$
      subroutine intdoldsvd(tol,fname)

c     [U,S,V,rk]=svd(A,tol)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      double precision tol,eps,dlamch
      parameter ( ZERO=0.0D0 )

      minrhs=1
      maxrhs=2
      minlhs=1
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            return
         else if(lhs.eq.4) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            if(.not.createvar(4,'d', 1, 1, lrk)) return
            stk(lrk)=0
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            lhsvar(4) = 4
            return
         endif
      elseif(N.eq.-1.or.M.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
      k = 3              
      if(lhs.gt.1) then
         if(.not.createvar(3,'d', M, M, lU)) return
         if(.not.createvar(4,'d', M, N, lS)) return
         if(.not.createvar(5,'d', N, N, lV)) return
         if(.not.createvar(6,'d', N, N, lVT)) return
         if(.not.createvar(7,'d', 1, 1, lrk)) return
         k = 8
      endif
      LWORKMIN = max(3*min(M,N)+max(M,N),5*min(M,N)-4)
      LWORK=maxvol(k,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
         JOBU = 'A'
         JOBVT = 'A'
      endif
      if(lhs.eq.1) then
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lA),
     $        M, stk(lA), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
c     next lines to patch an error of DGESVD
         do 01 ii = 0,min(M,N)-1
            stk(lSV+ii)=abs(stk(lSV+ii))
 01      continue
         call DLASRT('D', min(M,N), stk(lSV), INFO )
      else
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lU),
     $        M, stk(lVT), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
      endif  
      if(info.ne.0) then
         if(info.gt.0) call error(24)
         return
      endif

c     calculating rk = # of sv's > tol 

      eps=dlamch('eps')
      if(tol.eq.0.0d0) tol=dble(max(M,N))*eps*stk(lSV)
      irang=0
      do 51 i = 1, min(M,N)
         if(stk(lSV+i-1).gt.tol) irang=i
 51   continue
      stk(lrk)=dble(irang)
      if(lhs.gt.1) then
         call DLASET( 'F', M, N, ZERO, ZERO, stk(lS), M )
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, min(M,N)
            ii = i+(i-1)*M
            stk(lS+ii-1) = stk(lSV+i-1)
 10      continue        
         do 30 j = 1, N
            do 20 i = j, N
               ij = i+(j-1)*N
               ji = j+(i-1)*N
               stk(lV+ij-1) = stk(lVT+ji-1)
               stk(lV+ji-1) = stk(lVT+ij-1)
 20         continue
 30      continue     
      endif

      if(lhs.eq.1) then
         lhsvar(1) = 2
      else 
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=5
         lhsvar(4)=7
      endif
c     
      end

