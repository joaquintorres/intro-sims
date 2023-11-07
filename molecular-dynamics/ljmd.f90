! 
! simple lennard-jones potential md code with velocity verlet.
! units: length=angstrom, mass=amu, energy=kcal
!
! baseline f95 version.
!

module kinds
    implicit none
    integer, parameter :: dbl = selected_real_kind(14,200)  ! double precision floating point
    integer, parameter :: sgl = selected_real_kind(6,30)    ! single precision floating point
    integer, parameter :: sln = 200                         ! length of i/o input line
    private
    public :: sgl, dbl, sln
end module kinds

module physconst
    use kinds
    implicit none
    real(kind=dbl), parameter :: kboltz =    0.0019872067_dbl   ! boltzman constant in kcal/mol/k
    real(kind=dbl), parameter :: mvsq2e = 2390.05736153349_dbl  ! m*v^2 in kcal/mol
    private
    public :: kboltz, mvsq2e
end module physconst

! module to hold the complete system information 
module mdsys
    use kinds
    implicit none
    integer :: natoms,nfi,nsteps
    real(kind=dbl) dt, mass, epsilon, sigma, box, rcut
    real(kind=dbl) ekin, epot, temp
    real(kind=dbl), pointer, dimension (:) :: rx, ry, rz
    real(kind=dbl), pointer, dimension (:) :: vx, vy, vz
    real(kind=dbl), pointer, dimension (:) :: fx, fy, fz
end module mdsys

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

! compute kinetic energy
subroutine getekin
    use kinds
    use mdsys, only: natoms, mass, temp, ekin, vx, vy, vz
    use physconst
    implicit none

    integer :: i

    ekin = 0.0_dbl
    do i=1, natoms
        ekin = ekin + 0.5_dbl * mvsq2e * mass * (vx(i)*vx(i) + vy(i)*vy(i) + vz(i)*vz(i))
    end do

    temp = 2.0_dbl * ekin/(3.0_dbl*dble(natoms-1))/kboltz
end subroutine getekin

! compute forces 
subroutine force
    use kinds
    use utils
    use mdsys
    implicit none

    real(kind=dbl) :: r, ffac, dx, dy, dz
    integer :: i, j

    epot=0.0_dbl
    call azzero(fx,natoms)
    call azzero(fy,natoms)
    call azzero(fz,natoms)

    do i=1, natoms
         do j=1, natoms
        ! particles have no interactions with themselves 
            if (i==j) cycle
                
            ! get distance between particle i and j 
            !        delta = delta - box*(anint(delta/box))
            dx=pbc(rx(i) - rx(j), box)
            dy=pbc(ry(i) - ry(j), box)
            dz=pbc(rz(i) - rz(j), box)
            r = sqrt(dx*dx + dy*dy + dz*dz)

            ! compute force and energy if within cutoff */
            if (r < rcut) then
                ffac = -4.0_dbl*epsilon*(-12.0_dbl*((sigma/r)**12)/r   &
                    +6.0_dbl*(sigma/r)**6/r)
                    
                epot = epot + 0.5_dbl*4.0_dbl*epsilon*((sigma/r)**12 &
                    -(sigma/r)**6.0)

                fx(i) = fx(i) + dx/r*ffac
                fy(i) = fy(i) + dy/r*ffac
                fz(i) = fz(i) + dz/r*ffac
            end if
        end do
    end do
end subroutine force


! velocity verlet
subroutine velverlet
    use kinds
    use mdsys
    use physconst
    implicit none

    integer :: i


    ! first part: propagate velocities by half and positions by full step
    do i=1, natoms
        vx(i) = vx(i) + 0.5_dbl * dt / mvsq2e * fx(i) / mass
        vy(i) = vy(i) + 0.5_dbl * dt / mvsq2e * fy(i) / mass
        vz(i) = vz(i) + 0.5_dbl * dt / mvsq2e * fz(i) / mass
        rx(i) = rx(i) + dt*vx(i)
        ry(i) = ry(i) + dt*vy(i)
        rz(i) = rz(i) + dt*vz(i)
    end do

    ! compute forces and potential energy 
    call force

  ! second part: propagate velocities by another half step */
    do i=1, natoms
         vx(i) = vx(i) + 0.5_dbl * dt / mvsq2e * fx(i) / mass
         vy(i) = vy(i) + 0.5_dbl * dt / mvsq2e * fy(i) / mass
         vz(i) = vz(i) + 0.5_dbl * dt / mvsq2e * fz(i) / mass
    end do
end subroutine velverlet


program ljmd
    use kinds
    use io
    use utils
    use mdsys
    implicit none

    integer :: nprint, i
    character(len=sln) :: restfile, trajfile, ergfile

    read(stdin,*) natoms
    read(stdin,*) mass
    read(stdin,*) epsilon
    read(stdin,*) sigma
    read(stdin,*) rcut
    read(stdin,*) box
    call getline(stdin,restfile)
    call getline(stdin,trajfile)
    call getline(stdin,ergfile)
    read(stdin,*) nsteps
    read(stdin,*) dt
    read(stdin,*) nprint

    ! allocate storage for simulation data.
    allocate(rx(natoms),ry(natoms),rz(natoms),&
       vx(natoms),vy(natoms),vz(natoms), &
       fx(natoms),fy(natoms),fz(natoms))

    ! read restart 
    open(unit=33, file=restfile, form='formatted', status='old')
    do i=1,natoms
        read(33,*) rx(i), ry(i), rz(i)
    end do
    do i=1,natoms
        read(33,*) vx(i), vy(i), vz(i)
    end do
    close(33)

    call azzero(fx,natoms)
    call azzero(fy,natoms)
    call azzero(fz,natoms)

    ! initialize forces and energies
    nfi=0
    call force
    call getekin

    call ioopen(ergfile, trajfile)

    write(stdout, *) 'starting simulation with ', natoms, ' atoms for', nsteps, ' steps'
    write(stdout, *) '    nfi           temp                 ekin                  epot&
       &                etot'
    call output

    ! main md loop 
    do nfi=1, nsteps
    ! write output, if requested
    if (mod(nfi,nprint) == 0) then
        call output
    end if

        ! propagate system and recompute energies
        call velverlet
        call getekin
    end do

    ! clean up: close files, free memory
    write(stdout,'(a)') 'simulation done.'
    call ioclose

    deallocate(rx,ry,rz,vx,vy,vz,fx,fy,fz)
end program ljmd
