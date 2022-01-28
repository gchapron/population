\name{project_parallel}
\alias{project_parallel}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Parallel population projections
}
\description{
Run parallel stochastic population projections. Non-Windows systems only.
}
\usage{
project_parallel(years, runs, initial_population, survival, litter, cores)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{years}{
Number of years to project the population.
}
  \item{runs}{
Number of times (or Monte Carlo runs) to project the population.
}
  \item{initial_population}{
  Vector of initial number of individuals for each class. This vector must contain only positive integers.
}
  \item{survival}{
Matrix of survival for each class, with nrow = number of classes and ncol = 2. The first column is the median value of the survival of each class. The second column is the standard deviation of the survival of each class.
}
  \item{litter}{
Matrix of litter size for each class, with nrow = number of classes and ncol = 2. The first column is the median value of the litter size of each class. The second column is the standard deviation of the litter size of each class.
}
  \item{cores}{
(optional) number of cores to use for the parallel projections. If missing, it is set to the value returned by get_cores().
}
}
\details{
Run parallel stochastic population projections with an Individual-Based Model (IBM) compiled in C.
}
\value{
%%  ~Describe the value returned
%%  If it is a LIST, use
  \item{runs}{a 3-dimensional array of numbers of individuals with dimension c(years, classes, runs)}
%% ...
}

\examples{
years <- 10
runs <- 100

init.pop <- c(30, 20, 15, 12, 10, 9, 8, 7, 6, 5)

surv.md <- c(0.5, 0.7, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9)
surv.sd <- c(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1)
surv.msd <- cbind(surv.md, surv.sd)

litter.md <- c(0.2, 1.1, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 1.8, 0.2)
litter.sd <- c(0.1, 0.2, 0.15, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1)
litter.msd <- cbind(litter.md, litter.sd)

nclass <- 4 # vary number of classes

# with 2 cores
projection <- project_parallel(
	years = years,
	runs = runs,
	initial_population = init.pop[1:nclass],
	survival = surv.msd[1:nclass,],
	litter = litter.msd[1:nclass,],
	cores = 2
	)

# with all possible cores (not run)
# projection <- project_parallel(
# 	years = years,
# 	runs = runs,
# 	initial_population = init.pop[1:nclass],
# 	survival = surv.msd[1:nclass,],
# 	litter = litter.msd[1:nclass,]
# )
}
