Testing performance of the `individual` package
-----------------------------------------------

This repo uses the SIR model with changing population described in the
tutorial
[here](https://mrc-ide.github.io/individual/articles/Changing_Populations.html)
as an example for benchmarking performance of `individual` against pure
C++ code.

We compare performance of the R model against 1. a model written
entirely in C++ using the C++ components of `individual` and 2. an
`individual` R model that uses custom "prefab" C++ processes.

    ##   model_version median_time_seconds
    ## 1             R           1.9332614
    ## 2             C           0.6550511
    ## 3        Prefab           0.9024894
