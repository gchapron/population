\name{get_cores}
\alias{get_cores}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Get number of available cores
}
\description{
Get number of available cores for parallel simulations. Non-Windows systems only.
}
\usage{
get_cores(runs)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{runs}{
Number of times (or Monte Carlo runs) to repeat the simulation.
}
}
\details{
This function detects the number of cores (see 'detectCores' in package 'parallel') available and returns the largest possible number of cores being an integer divider of the number of runs. On multi-core machines at least one core is not used for the simulation.
}

\value{
get_cores()
}

\examples{
get_cores(2)
get_cores(1000)
}
