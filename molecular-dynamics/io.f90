module io
    use kinds
    implicit none
    private 
    integer, parameter :: stdin=5, stdout=6, log=30, xyz=31
    public :: ioopen, ioclose, output, stdin, stdout, getline

contains
    subroutine getline(chan, line)
    integer, intent(in) :: chan
    character(len=sln), intent(out) :: line
    integer :: idx, i

    read(chan, '(a)') line
    ! delete comment
    idx=index(line,'#')
    if (idx > 0) then
        do i=idx,sln
            line(i:i) = ' '
        end do
    end if
    end subroutine getline

    subroutine ioopen(logname, xyzname)
        character(len=sln) :: logname, xyzname
        open(unit=log, file=trim(logname), status='unknown', form='formatted')
        open(unit=xyz, file=trim(xyzname), status='unknown', form='formatted')
    end subroutine ioopen

    subroutine ioclose
        close(unit=log)
        close(unit=xyz)
    end subroutine ioclose
  
  ! append data to output.
    subroutine output
        use mdsys
        implicit none
        integer :: i
        write(log, '(i8,1x,f20.8,1x,f20.8,1x,f20.8,1x,f20.8)') &
             nfi, temp, ekin, epot, ekin+epot
        write(stdout, '(i8,1x,f20.8,1x,f20.8,1x,f20.8,1x,f20.8)') &
             nfi, temp, ekin, epot, ekin+epot
        write(xyz, '(i8)') natoms
        write(xyz, '(a,i8,1x,a,f20.8)') 'nfi=', nfi, 'etot=', ekin+epot
        do i=1, natoms
            write(xyz, '(a,1x,f20.8,1x,f20.8,1x,f20.8)') &
                'ar ', rx(i), ry(i), rz(i)
        end do
    end subroutine output
end module io
