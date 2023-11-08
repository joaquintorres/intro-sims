
program molecular_dynamics
    use kinds
    use io
    use utils
    use mdsys
    use ljmd

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
end program molecular_dynamics
