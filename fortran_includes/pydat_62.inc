* PYHTIA 6.2xxx common
      integer         MSTU,               MSTJ
      double precision          PARU,               PARJ     
      COMMON /PYDAT1/ MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      save /pydat1/
* PYHTIA 6.2xxx common
      integer         KCHG
      double precision            PMAS,       PARF,      VCKM
      COMMON /PYDAT2/ KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      save   /PYDAT2/
* PYHTIA 6.2xxx common
      integer         MDCY,       MDME,                   KFDP
      double precision                         BRAT
      COMMON /PYDAT3/ MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      save   /PYDAT3/
