#include <individual.h>
#include <Rcpp.h>
#include <algorithm>
#include <random>

// [[Rcpp::export]]
Rcpp::XPtr<process_t> birth_process(
    Rcpp::XPtr<CategoricalVariable> variable,
    Rcpp::XPtr<TargetedEvent> event,
    const double birth_rate
){

    // make pointer to lambda function and return XPtr to R
    return Rcpp::XPtr<process_t>(
        new process_t([variable,event,birth_rate](size_t t){

            auto n_births = R::rpois(birth_rate);
            std::vector<std::string> new_s = std::vector<std::string>(n_births);
            for (size_t i = 0; i < n_births; i++) {
              new_s[i] = "S";
            }
            variable->queue_extend(new_s);
            event->queue_extend(n_births);
        }),
        true
    );
}

// [[Rcpp::export]]
Rcpp::XPtr<process_t> infection_process(
    Rcpp::XPtr<CategoricalVariable> variable,
    const double beta,
    const size_t N,
    const double dt
){

    // make pointer to lambda function and return XPtr to R
    return Rcpp::XPtr<process_t>(
        new process_t([variable, beta, N, dt](size_t t){

           auto I = variable->get_size_of("I");
           auto foi = beta * I / N;
           individual_index_t S = variable->get_index_of("S");
           bitset_sample_internal(S, R::pexp(foi * dt, 1, true, false));
           variable->queue_update("I", S);
        }),
        true
    );
}

// [[Rcpp::export]]
Rcpp::XPtr<process_t> recovery_process(
    Rcpp::XPtr<CategoricalVariable> variable,
    Rcpp::XPtr<TargetedEvent> event,
    const double gamma,
    const double dt
){

    // make pointer to lambda function and return XPtr to R
    return Rcpp::XPtr<process_t>(
        new process_t([variable, event, gamma, dt](size_t t){

           individual_index_t I = variable->get_index_of("I");
           individual_index_t already_scheduled = event->get_scheduled();
           I &= !(already_scheduled);
           auto rec_times = Rcpp::rgeom(I.size(), R::pexp(gamma * dt, 1, true, false));
           std::vector<double> vec_rec_times(rec_times.begin(), rec_times.end());
           for (size_t i = 0; i < I.size(); ++i) {
               vec_rec_times[i] = vec_rec_times[i] + 1;
           }
           event->schedule(I, vec_rec_times);
        }),
        true
    );
}

// [[Rcpp::export]]
Rcpp::XPtr<process_t> death_process(
    Rcpp::XPtr<CategoricalVariable> variable,
    Rcpp::XPtr<TargetedEvent> event,
    const double death_rate
){

    // make pointer to lambda function and return XPtr to R
    return Rcpp::XPtr<process_t>(
        new process_t([variable, event, death_rate](size_t t){

           size_t pop_size = variable->size();
           double dr = death_rate;
           if (dr > 1) {
             dr = 1;
           }
           int n_deaths = R::rbinom(pop_size, dr);
           std::vector<size_t> ivec(pop_size);
           std::vector<size_t> out;
           std::iota(ivec.begin(), ivec.end(), 0); // ivec will become: [0..n]
           std::sample(ivec.begin(), ivec.end(),
                    std::back_inserter(out),
                    n_deaths, std::mt19937{std::random_device{}()});
           variable->queue_shrink(out);
           event->queue_shrink(out);
        }),
        true
    );
}
