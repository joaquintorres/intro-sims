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
