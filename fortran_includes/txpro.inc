
*----------------------------------------------------------------------------*
      integer  maxpro 
      parameter (maxpro=500)  ! max.num. of TopReX process
*
      integer         txmup, txsel, txpro,         txext
      common /trxpro/ txmup, txsel, txpro(maxpro), txext(maxpro)
      save   /trxpro/
*............
* rxsig(I,1) : sum of differential X-section   
* rxsig(I,2) : total X-section value
* rxsig(I,3) : max of differential X-section 
* rxsig(I,4) : sum of differential X-section squared
      integer         NXev
      double precision              rxsig
      common /trxsig/ NXev(maxpro), rxsig(maxpro,4)
      save   /trxsig/
