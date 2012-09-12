
      SUBROUTINE INIT

      include 'common.inc'
      INTEGER I,J,K
      INTEGER II,JJ
      INTEGER III
      INTEGER JJJ
      DOUBLE PRECISION A

      WRITE (*,*) " *** JUMP INSIDE INIT SUBROUTINE ***"

      PLANET = "EARTH"
      
      DO III = 1,10,1
         RTAB(III) = III
      ENDDO

      D1 = 1.0
      D2 = 2.0
      A = -189.0D0
      DO I = -2,2,1
         DO J = 1,5,1
            DO K = 1,7,1
               A = A + 1.0D0
               DTAB(I,J,K) = A
            ENDDO
         ENDDO
      ENDDO
      
      I1 = 1
      I2 = 2
      
      DO II = 1,5,1
         DO JJ = 1,2,1
            ITAB(II,JJ) = II*10 + JJ
         ENDDO
      ENDDO

      DAY(1) = "SUN"
      DAY(2) = "MON"
      DAY(3) = "TUE"
      DAY(4) = "WED"
      DAY(5) = "THU"
      DAY(6) = "FRI"
      DAY(7) = "SAT"


      WRITE (*,*) " *** QUIT FORM INIT SUBROUTINE   ***"

      RETURN
      END

      SUBROUTINE PRINT_I1
      INCLUDE 'common.inc'
      WRITE (*,*) "F77 I1 FROM COMMON /GLOBAL/ = ", I1
      RETURN 
      END
      
      SUBROUTINE PRINT_I2
      INCLUDE 'common.inc'
      WRITE (*,*) "F77 I2 FROM COMMON /GLOBAL/ = ", I2
      RETURN 
      END

      SUBROUTINE PRINT_D1
      INCLUDE 'common.inc'
      WRITE (*,*) "F77 D1 FROM COMMON /GLOBAL/ = ", D1
      RETURN 
      END

      SUBROUTINE PRINT_D2
      INCLUDE 'common.inc'
      WRITE (*,*) "F77 D2 FROM COMMON /GLOBAL/ = ", D2
      RETURN 
      END

      SUBROUTINE PRINT_PLANET
      INCLUDE 'common.inc'
      WRITE (*,*) "F77 PLANET FROM COMMON /GLOBAL/ = ", PLANET
      RETURN 
      END
      
      SUBROUTINE PRINT_DAY
      INCLUDE 'common.inc'
      INTEGER I
      WRITE (*,*) "F77 DAY FROM COMMON /GLOBAL/ IS "
      DO I = 1, 7, 1
         WRITE (*,*) "F77 DAY(", I, ") = ", DAY(I) 
      ENDDO
      RETURN 
      END
      

      SUBROUTINE PRINT_RTAB
      INCLUDE 'common.inc'
      INTEGER I
      WRITE (*,*) "F77 RTAB FROM COMMON /GLOBAL/ IS "
      DO I = 1,10,1
         WRITE(*,*) 'F77 RTAB(',I,') = ', RTAB(I) 
      ENDDO
      RETURN 
      END
      

      SUBROUTINE PRINT_ITAB
      INCLUDE 'common.inc'
      INTEGER I,J
      WRITE (*,*) "F77 ITAB FROM COMMON /GLOBAL/ IS "
      DO I = 1,5,1
         DO J = 1,2,1
            WRITE (*,*) 'F77 ITAB(',I,',',J,') = ', ITAB(I,J)
         ENDDO
      ENDDO
      RETURN
      END

      SUBROUTINE PRINT_DTAB
      INCLUDE 'common.inc'
      INTEGER I,J,K
      WRITE (*,*) "F77 DTAB FROM COMMON /GLOBAL/ IS "
      
      DO I = -2,2,1
         DO J = 1,5,1
            DO K = 1,7,1
               WRITE(*,*) 'F77 DTAB(',I,',',J,',',K,') =', DTAB(I,J,K)
            ENDDO
         ENDDO
      ENDDO
      
      RETURN
      END


      SUBROUTINE WRITEOUT

      INCLUDE 'common.inc'
      INTEGER I,J,K
      INTEGER II,JJ
      INTEGER III, JJJ

      WRITE (*,*) " *** JUMP INSIDE WRITEOUT SUBROUTINE ***"
      WRITE (*,*) "/GLOBAL/ common variables are:"
      WRITE (*,*) "****************************"
      WRITE (*,*) "I1        = ", I1
      WRITE (*,*) "I2        = ", I2
      WRITE (*,*) "D1        = ", D1
      WRITE (*,*) "D2        = ", D2
      WRITE (*,*) "PLANET    = ", PLANET

      WRITE (*,*) "****************************"
      DO III = 1,10,1
         WRITE(*,*) 'RTAB(',III,') = ', RTAB(III) 
      ENDDO
      WRITE (*,*) "****************************"
      DO II = 1,5,1
         DO JJ = 1,2,1
            WRITE (*,*) 'ITAB(',II,',',JJ,') = ', ITAB(II,JJ)
         ENDDO
      ENDDO
      WRITE (*,*) "****************************"
      DO I = -2,2,1
         DO J = 1,5,1
            DO K = 1,7,1
               WRITE(*,*) 'DTAB(',I,',',J,',',K,') =', DTAB(I,J,K)
            ENDDO
         ENDDO
      ENDDO
      WRITE (*,*) "****************************"
      DO JJJ = 1,7,1
         WRITE(*,*) 'DAY(',JJJ,') = ', DAY(JJJ)
      ENDDO
    
      WRITE (*,*) " *** QUIT FROM WRITEOUT SUBROUTINE   ***"
      RETURN
      END

  
