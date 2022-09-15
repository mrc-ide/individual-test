Testing performance of the `individual` package
-----------------------------------------------

This repo uses the SIR model with changing population described in the
tutorial
[here](https://mrc-ide.github.io/individual/articles/Tutorial.html) as
an example for benchmarking performance of `individual` against pure C++
code.

We compare performance of the R model against 1. a model written
entirely in C++ using the C++ components of `individual` and 2. an
`individual` R model that uses custom “prefab” C++ processes.

### Closed SIR Model

    ##   model_version median_time_seconds speedup
    ## 1             R          0.66823413     1.0
    ## 2             C          0.03069129    22.0
    ## 3        Prefab          0.20348917     3.3

### Open SIR Model

    ##   model_version median_time_seconds speedup
    ## 1             R           1.3776422     1.0
    ## 2             C           0.3273255     4.2
    ## 3        Prefab           0.6290169     2.2

Profiling c++ code
------------------

1.  install system deps:

         sudo apt install \
         libgoogle-perftools-dev \
         libprotoc-dev libprotobuf-dev protobuf-compiler \
         golang-go \
         graphviz

2.  Install `pprof`: `go install github.com/google/pprof@latest`

3.  Install this package: `R CMD INSTALL .`

4.  Run the profiling script: `./profile/run_profiler.sh`

5.  Examine the outputs using [pprof](https://github.com/google/pprof).
    The web interface is probably the best way:
    `pprof -http=localhost:5050 profile.log`
