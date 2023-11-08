module kinds
    implicit none
    integer, parameter :: dbl = selected_real_kind(14,200)  ! double precision floating point
    integer, parameter :: sgl = selected_real_kind(6,30)    ! single precision floating point
    integer, parameter :: sln = 200                         ! length of i/o input line
    private
    public :: sgl, dbl, sln
end module kinds
