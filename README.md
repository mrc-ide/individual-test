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
    ## 1             R          0.59080680     1.0
    ## 2             C          0.02948761    20.0
    ## 3        Prefab          0.18778400     3.1

### Open SIR Model

    ##   model_version median_time_seconds speedup
    ## 1             R           1.1542080     1.0
    ## 2             C           0.3327583     3.5
    ## 3        Prefab           0.6385800     1.8

Profiling c++ code
------------------

1.  install system deps:

         sudo apt install \
         libgoogle-perftools-dev \
         libprotoc-dev libprotobuf-dev protobuf-compiler \
         golang-go \
         graphviz

2.  install `pprof`: `go install github.com/google/pprof@latest`

3.  install this package: `R CMD INSTALL .`

4.  run the profiling script: `./profile/run_profiler.sh`
