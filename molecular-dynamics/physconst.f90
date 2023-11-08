module physconst
    use kinds
    implicit none
    real(kind=dbl), parameter :: kboltz =    0.0019872067_dbl   ! boltzman constant in kcal/mol/k
    real(kind=dbl), parameter :: mvsq2e = 2390.05736153349_dbl  ! m*v^2 in kcal/mol
    private
    public :: kboltz, mvsq2e
end module physconst
