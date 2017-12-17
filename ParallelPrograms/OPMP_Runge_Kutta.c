
#include <string.h>
#include <stdlib.h>
#include <math.h>

//inputs Mass, Velocity, Position of each particle

#define DIM 2
typedef double vect_t[DIM];

void compute_forces(int part,vect_t* forces);
double runge_kutta_4(double(*f)(double, double), double dx, double x, double y);

//Main
int main ()
{
    for each timestep {
        if (timestep == output){
            //Then output
        }

        for each particle q {
            vect_t* forces = malloc(n*sizeof(vect_t));   
            for (int step = 1; step <= n_steps; step++){
                forces = memset(forces, 0, n*sizeof(vect_t));
                for (int part = 0; part < n-1; part++){
                    compute_force(part, forces);
            }
    }
        }
        //Compute force on Q
        for each particle q{
            pos[q][X] += runge_kutta_4(function,delta_t, vel[q][X];
            pos[q][Y] += runge_kutta_4(function,delta_t, vel[q][Y]);
            vel[q][X] += runge_kutta_4(function, delta_t, vel[q][X]);
            vel[q][Y] += runge_kutta_4(function, delta_t, vel[q][Y]);
        }
    }
}



//output Position and Velocity after a period of time
void compute_forces(int part,vect_t* forces)
{
    for each particle q;
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
            forces[k][Y] -= force_qk[Y];
        }
    }
}

double runge_kutta_4(double(*f)(double, double), double dx, double x, double y)
{
    double	k1 = dx * f(x, y),
            k2 = dx * f(x + dx / 2, y + k1 / 2),
            k3 = dx * f(x + dx / 2, y + k2 / 2),
            k4 = dx * f(x + dx, y + k3);
    return y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}