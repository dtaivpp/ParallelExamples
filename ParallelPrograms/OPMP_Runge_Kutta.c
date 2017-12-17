
#include <string.h>

//inputs Mass, Velocity, Position of each particle

//Main 
for each timestep {
if (timestep == output)
    output;
    for each particle q
        compute force on q
    for each particle q
        compute positon and Velocity
}
print positions and velocities


//output Position and Velocity after a period of time

vect_t* forces = malloc(n*sizeof(vect_t));
for (int step = 1; step <= n_steps; step++){
    forces = memset(forces, 0, n*sizeof(vect_t))
    for (int part =0; part < n-1; part++){
        compute_force(part, forces)
    }

}

compute forces
for each particle q
    forces[q] = 0;
for each particle q{
    for each particle k > q{ 
        x_diff = pos[q][X] - pos[k][X];
        y_diff = pos[q][Y] - pos[k][Y];
        dist = sqrt(x_diff * x_diff + y_diff * y_diff);
        dist_cubed = dist*dist*dist;
        forces[q][X] -= G*masses[q]*masses[k] / dist_cubed * x_diff;
        forces[q][Y] -= G*masses[q]*masses[k] / dist_cubed * y_diff;

        forces[q][X] += force_qk[X];
        forces[q][Y] += force_qk[Y];
        forces[k][X] -= force_qk[X];
        forces[k][Y] -= force_qk[Y]
    }
}