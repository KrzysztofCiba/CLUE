*............................................................................*
*..... GLOBAL     TopReX    parameters                                       *
*. . . . . .  INTEGER parametrs . . . . . . . . . . . . . . . . . . . . . .  *
*..Ipar(1)      ! internal TopReX code of the process                        *
*  -1 : PYTHIA process                                                        *
*  Ipar(1) =  1  :  EW top : W g(2->2)                                       *
*  Ipar(1) =  2  :  EW top : W g(2->3)                                       *
*  Ipar(1) =  3  :  EW top : t W                                             *
*  Ipar(1) =  4  :  EW top : W* -> t bbar                                    *
*  Ipar(1) =  5  :  W Q Qbar                                                 *
*  Ipar(1) =  6  :  W+-  -> tau nu                                           *
*  Ipar(1) =  7  :  W+- (-> tau nu) JET                                      *
*  Ipar(1) =  8  :  H+-  -> t bbar                                           *
*  Ipar(1) =  9  :  H+-  -> tau nu                                           *
*  Ipar(1) = 10  :  H+- (-> tau nu) JET                                      *
*  Ipar(1) = 11  :  EW top : W g(2->2) and W g(2->3)                         *
*  Ipar(1) = 12  :  FCNC top : g u -> top                                    *
*  Ipar(1) = 20  :  gg+qq~ -> tt~,  'on-shel ',  S. Slabospitsky             *
*  Ipar(1) = 21  :  gg     -> tt~,  'on-shel ',  S. Slabospitsky             *
*  Ipar(1) = 22  :     qq~ -> tt~,  'on-shel ',  S. Slabospitsky             *
*  Ipar(1) = 31  :  q q~   -> H+ H-                                          *
*  Ipar(1) = 50  :  gg+qq~ -> tt~,  'on-shel ',  W. Bernreuther et al        *
*  Ipar(1) = 51  :  gg     -> tt~,  'on-shel ',  W. Bernreuther et al        *
*  Ipar(1) = 52  :     qq~ -> tt~,  'on-shel ',  W. Bernreuther et al        *
*............................................................................*
*................                                                            *
*  Ipar(2)      : num. of current event                                      *
*..Ipar(3)      : evolution scale type for PDF                               *
*..Ipar(4)      : evolution scale type for alpha_S                           *
*..Ipar(5)      : num. of events used for estimation of SigMax               *
*..Ipar(6)      : num. of events used for estimation of M**2                 *
*..Ipar(7)      : KF code of Q-quark for WQQbar                              *
*..Ipar(8)      : num. of MC entries  for estimates  of H+- and T decays     *
*..Ipar(9)      : W-exchange: > 0 tb, <0 tau nu, 1 W, 2 H, 3 (W+H+Int)       *
*..Ipar(10)     : 1(0) (no)print  sigma for each q, qbar contribution        *
*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
*  Ipar(11)     : 0(1) quark masses are taken from PYTHIA(PDG)               *
*  Ipar(12)     : 0(1) s,c,b,t mass value are QCD-evaluated(fixed)           *
*               : Ipar(12) is used only for evaluation of H+- couplings      *
*  Ipar(13)     : 1(0) running (fixed) top-mass according top width          *
*  Ipar(14)     : 1(0) take (not) into account spin of produced top quarks   *
*  Ipar(15)     : number of current color                                    *
*  Ipar(16)     : MIN M(QQ), 0 : 2M_Q, 1 - user's input                      *
*  Ipar(17)     : MAX M(QQ), 0 : LIPS, 1 - user's input                      *
*..Ipar(18)     : flag for first/second order in alpha-s evaluation          *
*..Ipar(20)     : continuation flag                                          *
*..Ipar(21)     : INIT flag                                                  *
*. . . . . . . . . . . . . .  . . . . . . . . . . . . . . . . . . . . . . .  *
*. . . . . .  DOUBLE PRECISION parameters . . . . . . . . . . . . . . . . .  *
*..Rpar(1)      : Ecm                                                        *
*..Rpar(2)      : S = Ecm**2                                                 *
*..Rpar(3)      : Evolution scale for PDF                                    *
*..Rpar(4)      : Evolution scale for alpha_s                                *
*..Rpar(5)      : parameter for Q-scale evaluation for PDF                   *
*..Rpar(6)      : parameter for Q-scale evaluation for alpha_s               *
*
*.............. couplings ...................................................*
*..Rpar(10)     : 0.38939d0     transformation 1/GeV**2 to mb                *
*..Rpar(11)     : alpha_QED = (1/128) is electromagnetic coupling            *
*..Rpar(12)     : 4.*pi*rpar(11)  = (electric charge)**2                     *
*..Rpar(13)     : sqrt(4.*pi*rpar(11)) is electric charge                    *
*..Rpar(14)     : G_Fermi                                                    *
*..Rpar(15)     : alpha_QCD = QCD coupling used for event                    *
*..Rpar(16)     : 4*pi * Rpar(15) = (QCD charge)**2                          *
*..Rpar(17)     : sqrt(4*pi * Rpar(15)) = QCD charge                         *
*.......
*..Rpar(21)     : sin(theta_W)                                               *
*..Rpar(22)     : cos(theta_W)                                               *
*..Rpar(23)     : sin(theta_W)**2                                            *
*..Rpar(24)     : cos(theta_W)**2                                            *
*..Rpar(25)     : sin(2*theta_W) = 2 * sin(theta_W) * cos(theta_W)           *
*..Rpar(26)     : g = g_weak  = e / sin(theta_W)                             *
*..Rpar(27)     : g_z         = e / sin(2*theta_W)                           *
*
*.............. top quark parameters ........................................*
*..Rpar(30)     : width of t-> d  W decay at tree level (SM case)            *
*..Rpar(31)     : width of t-> s  W decay at tree level (SM case)            *
*..Rpar(32)     : width of t-> b  W decay at tree level (SM case)            *
*..Rpar(33)     : width of t-> b4 W decay at tree level (SM case)            *
*..Rpar(34)     : total t-quark decay width                                  *
*..Rpar(35)     : width of t -> b H+- decay channel                          *
*..Rpar(36)     : M(top) * width(total top)                                  *
*..rpar(37)     : current value of M(top) generated with B.W.                *
*.............. W-boson and Z-boson parameters  .............................*
*..Rpar(41)     : W-boson mass                                               *
*..Rpar(42)     : Rpar(41)**2                                                *
*..Rpar(43)     : W-boson width                                              *
*..Rpar(44)     : M(W) * Width(W)                                            *
*...                                                                         *
*..Rpar(45)     : Z-boson mass                                               *
*..Rpar(46)     : Rpar(45)**2                                                *
*..Rpar(47)     : Z-boson width                                              *
*..Rpar(48)     : M(Z) * Width(Z)                                            *
*.............. charged Higgs parameters  ...................................*
*..Rpar(51)     : charged Higgs mass                                         *
*..Rpar(52)     : Rpar(51)**2                                                *
*..Rpar(53)     : total charged Higgs decay width                            *
*..Rpar(54)     : M(H+-) * Width(H+-)                                        *
*..Rpar(55)     : tan(beta)                                                  *
*..Rpar(56)     : (1/tan(beta))                                              *
*............................................................................*
*..Rpar(60)     : New Physics scale (1 TeV), FCNC                            *
*............................................................................*
*..Rpar(81:83) are for processes Ipar(1) = 50, 51, 52 only                   *
*..Rpar(81)    : gVV, relative coupling of Higgs boson(H0) to W and Z bosons *
*..Rpar(82)    : a, scalar Yukawa coupling of Higgs boson(H0) to top quarks  *
*..Rpar(83)    : a~, pseudoscalar Yukawa coupling of H0 boson to top quarks  *
*............................................................................* 
*..Rpar(100)    : differential cross section (in PB) for current event       *
*............................................................................* 
*..Rpar(101)   : CKIN(1) = sqrt(Shat_MIN) for hard process                   *
*..Rpar(102)   : CKIN(2) = sqrt(Shat_MAX) for hard process                   *
*..Rpar(103)   : CKIN(3) = PThat_MIN                                         *
*..Rpar(104)   : CKIN(4) = PThat_MAX                                         *
*..Rpar(105)   : MW_min  for virtual W in heavy particle decay               *
*..Rpar(106)   : M(QQbar)_MIN  for W Q Qbar production                       *
*..Rpar(107)   : M(QQbar)_MAX  for W Q Qbar production                       *
*..Rpar(108)   : min PThat for IPAR(1) = 2;  T-channel                       *
*..Rpar(109)   : M(QQbar)_MIN  for Q Qbar productionmin (future)             *
*............................................................................*
*..Rpar(111)   : sqrt(Shat) for hard process                                 *
*..Rpar(112)   : Shat       for hard process                                 *
*..Rpar(113)   : PThat      for hard process                                 *
*..Rpar(114)   : PThat**2   for hard process                                 *
*............................................................................*
*...........  Graviton* production,  ADD scenario                            *
*..Rspar(121)     : M_exdim,                         ADD scenario             *
*..Rspar(122)     : n_exdim,  Graviton* production,  ADD scenario             *
*..Rspar(123)     : mKK mass of KK-excitation,       ADD scenario             *
*..Rspar(124)     : recommended PThat_MIN            ADD scenario             *
*............................................................................*
*...........  Black Hole (BH) production                                     *
*  Rspar(131)     : Planck scale, GeV                                         *
*  Rspar(132)     : n extra dimensions                                        *
*  Rspar(133)     : BR(BH -> q qbar X)                                        *
*  Rspar(134)     : BR(BH -> g g    X) / 2                                    *
*  Rspar(135)     : BR(BH -> l l'   X)                                        *
*  Rspar(136)     : BR(BH -> gamma,W,Z X) + BR(BH -> Higgs  X)                *
*  Rspar(137)     : BR(BH -> Higgs  X)                                        *
*............................................................................*
      Integer           Ipar     ! global TopRex integer parameters  
      double precision  Rpar     ! global TopRex real    parameters
      common /TXPAR/Ipar(200), Rpar(200)
      save   /TXPAR/
**********
      integer ndch   ! maximum number of decay channels
      parameter (ndch=50)
      integer        MID(0:ndch,4),             FID(4,ndch,5) 
      double precision   wid(0:ndch,4),brf(ndch,4),     BRS(2,4)
      common /TXdec/ MID, WID,         BRF,     FID,    brs
      save   /TXdec/
*...........
