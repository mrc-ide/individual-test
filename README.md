Testing performance of the `individual` package
-----------------------------------------------

This repo uses the SIR model with changing population described in the
tutorial
[here](https://mrc-ide.github.io/individual/articles/Tutorial.html) as
an example for benchmarking performance of `individual` against pure C++
code.

We compare performance of the R model against 1. a model written
entirely in C++ using the C++ components of `individual` and 2. an
`individual` R model that uses custom "prefab" C++ processes.

### Closed SIR Model

    ##   model_version median_time_seconds speedup
    ## 1             R          0.73188955     1.0
    ## 2             C          0.03573859    20.0
    ## 3        Prefab          0.22616319     3.2

### Open SIR Model

    ##   model_version median_time_seconds speedup
    ## 1             R           1.3406097     1.0
    ## 2             C           0.5096369     2.6
    ## 3        Prefab           0.7546200     1.8
