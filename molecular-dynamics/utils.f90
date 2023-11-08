module utils
    use kinds
    implicit none

    private
    public :: azzero, pbc

contains
! helper function: zero out an array 
    subroutine azzero(d, n)
    real(kind=dbl), dimension(:), intent(inout) :: d
    integer, intent(in) :: n
    integer :: i

    do i=1, n
        d(i) = 0.0_dbl
    end do
    end subroutine azzero

! helper function: apply minimum image convention 
    function pbc(x, box)
        real(kind=dbl), intent(in)  :: x, box
        real(kind=dbl) :: pbc

        pbc = x - box*(anint(x/box))
    end function pbc
end module utils
