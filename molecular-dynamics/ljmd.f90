! 
! simple lennard-jones potential md code with velocity verlet.
! units: length=angstrom, mass=amu, energy=kcal
!
! baseline f95 version.
!
module ljmd

contains
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

        real(kind=dbl) :: rsq, rcutsq, rinv, r6, c6, c12, ffac, dx, dy, dz
        integer :: i, j

        epot=0.0_dbl
        call azzero(fx,natoms)
        call azzero(fy,natoms)
        call azzero(fz,natoms)

        ! Avoid repetition and sqrts
        rcutsq = rcut*rcut
        c6  = 4.0_dbl*epsilon*sigma**6
        c12 = 4.0_dbl*epsilon*sigma**12

        do i=1, natoms
             do j=1, natoms
            ! particles have no interactions with themselves 
                if (i==j) cycle
                    
                ! get distance between particle i and j 
                !        delta = delta - box*(anint(delta/box))
                dx=pbc(rx(i) - rx(j), box)
                dy=pbc(ry(i) - ry(j), box)
                dz=pbc(rz(i) - rz(j), box)

                rsq = dx*dx + dy*dy + dz*dz

                ! compute force and energy if within cutoff */
                if (rsq < rcutsq) then
                    rinv = 1.0_dbl / rsq
                    r6   = rinv**3
                    ffac = (12.0_dbl*c12*r6 - 6.0_dbl*c6)*r6*rinv 
                    epot = epot + r6*(c12*r6 - c6) / 2.0_dbl

                    fx(i) = fx(i) + dx*ffac
                    fy(i) = fy(i) + dy*ffac
                    fz(i) = fz(i) + dz*ffac
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
end module ljmd
