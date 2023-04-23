# HPC Homework 4

**Problem 1**

I modified the given ***pingpong.cpp*** file a little bit to add a piece of code that gets and prints the current processor name. I run it with job.sbatch options:

```
#!/bin/bash
#SBATCH --job-name=HPC_pingpong
#SBATCH --nodes=4
#SBATCH --cpus-per-task=1
#SBATCH --tasks-per-node=1
#SBATCH --mem=2GB 
#SBATCH --time=01:00:00 
#SBATCH --verbose
module purge 
module load  amber/openmpi/intel/20.06 
mpiexec -np 4 ./pingpong 0 1
```

Here I run with 4 processes and force them to be on different physical nodes. The result is:

```
Rank 0/4 running on cs011.hpc.nyu.edu.
Rank 3/4 running on cs014.hpc.nyu.edu.
Rank 2/4 running on cs013.hpc.nyu.edu.
Rank 1/4 running on cs012.hpc.nyu.edu.
pingpong latency: 2.508956e-03 ms
pingpong bandwidth: 1.227394e+01 GB/s
```

Then I also try to change it to 8 processes and add `--subscribe` flag to the compiler to allocate more slots in MPI. The result is:

```
Rank 0/8 running on cs339.hpc.nyu.edu.
Rank 3/8 running on cs342.hpc.nyu.edu.
Rank 4/8 running on cs343.hpc.nyu.edu.
Rank 2/8 running on cs341.hpc.nyu.edu.
Rank 1/8 running on cs340.hpc.nyu.edu.
Rank 6/8 running on cs345.hpc.nyu.edu.
Rank 5/8 running on cs344.hpc.nyu.edu.
Rank 7/8 running on cs346.hpc.nyu.edu.
pingpong latency: 1.249545e-03 ms
pingpong bandwidth: 1.155863e+01 GB/s
```

**Problem 2**

I parallelized the serial code and ran it on AMD 64-bit CPU, Intel® Core™ i5-10210U CPU, with 4 compute cores. The OS is Windows Subsystem for Linux distro Ubuntu 22.04. The runtime with different thread number are following:

thread number = 2, run time = 0.167012s
thread number = 3, run time = 0.170121s
thread number = 4, run time = 0.236609s
thread number = 5, run time = 0.201763s
thread number = 6, run time = 0.225366s
thread number = 7, run time = 0.223382s
thread number = 8, run time = 0.222939s
thread number = 9, run time = 0.270187s
thread number = 10, run time = 0.242522s
thread number = 11, run time = 0.232214s
thread number = 12, run time = 0.246561s
thread number = 13, run time = 0.259333s
thread number = 14, run time = 0.225655s
thread number = 15, run time = 0.232036s
thread number = 16, run time = 0.228077s

**Problem 3**

Jacobi: Run time for different N and number of threads used (in seconds):

| N\num of threads | 2        | 4        | 8        | 12       | 16       | 32       |
| ---------------- | -------- | -------- | -------- | -------- | -------- | -------- |
| 10               | 0.000911 | 0.001169 | 0.001628 | 0.259923 | 0.345807 | 0.704130 |
| 20               | 0.001295 | 0.001224 | 0.001860 | 0.262076 | 0.348330 | 0.688536 |
| 40               | 0.001781 | 0.001846 | 0.002243 | 0.262964 | 0.368311 | 0.694200 |
| 80               | 0.003787 | 0.003333 | 0.003277 | 0.262515 | 0.347103 | 0.689056 |
| 100              | 0.005197 | 0.004778 | 0.004211 | 0.262674 | 0.349931 | 0.688023 |
| 200              | 0.015535 | 0.014766 | 0.009952 | 0.267112 | 0.354343 | 0.696541 |
| 400              | 0.082922 | 0.069810 | 0.065902 | 0.314272 | 0.418587 | 0.722531 |
| 800              | 0.834939 | 0.847266 | 1.138915 | 1.262281 | 1.276995 | 1.401142 |
| 1600             | 3.803361 | 3.710784 | 4.250760 | 4.463524 | 4.562097 | 4.915007 |

**Problem 4**

(Credit to Kitty Li for writing this part for our group)

For the final project, we will be working in a group of three people: Yifei Zhu, Huilin Li, and Kitty Li. We are planning to **use the Lattice Boltzmann Method(LBM) to model a 2D flow in a rectangular channel**, which is a method that instead of solving the Navier-Stokes equations directly, it models fluid particles moving from one discrete grid point to another according to collision rules. The numerical steps for simulating fluid are the collision step and the streaming step:

The collision step:

$$
f_i(\vec{x}, t + \delta_t) = f_i(\vec{x}, t) + \frac{f_i^{eq} (\vec{x}, t) - f_i (\vec{x}, t)}{\tau_f}
$$

The streaming step:

$$
f_i(\vec{x} + \vec{e}_i, t + \delta_t) = f_i(\vec{x}, t)
$$

We will be modeling the flow with two types of boundary conditions: physical boundary conditions and parallel boundary conditions. The physical boundary conditions require a no-slip condition at the upper and lower boundaries of the flow, while the left and right boundaries are periodic. The parallel boundary conditions involve partitioning the domain into subdomains, with either OpenMP or MPI used to communicate boundary condition information during the collision step.
