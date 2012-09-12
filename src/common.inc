c dummy generator common
      IMPLICIT NONE
c comment
      REAL RTAB(10)
      DOUBLE PRECISION DTAB(-2:2,1:5,7), D1, D2
      INTEGER I1, I2, ITAB(5,2)   
      CHARACTER*3 DAY(7) 
      CHARACTER PLANET*10

      COMMON /GLOBAL/ D1, D2, DTAB, RTAB, I1, I2, ITAB, DAY, PLANET