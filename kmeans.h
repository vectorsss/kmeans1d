#ifndef __KMEANS_H__
#define __KMEANS_H__

#include <stdlib.h>
#include <stdio.h>

#include <cstdint>
#include <cstddef>
#include <memory>
#include <utility>

#include "interval_sum.hpp"

class kmeans_result {
public:
    double cost;
    std::vector<double> centers;
};

class kmeans {
public:
    virtual ~kmeans() {};
    virtual std::unique_ptr<kmeans_result> compute(size_t k) = 0;
};

class kmeans_hirschberg_larmore : public kmeans {
public:
    kmeans_hirschberg_larmore(const std::vector<double> &points);
    std::unique_ptr<kmeans_result> compute(size_t k) override;
    ~kmeans_hirschberg_larmore() override;
private:
    double weight(size_t i, size_t j);
    double g(size_t i, size_t j);
    bool bridge(size_t i, size_t j, size_t k, size_t n);
    std::pair<double, size_t> basic(size_t n);
    std::pair<double, size_t> traditional(size_t n);
    double get_actual_cost(size_t n, double *centers);

    double lambda;
    std::vector<double> f;
    std::vector<std::size_t> bestleft;
    interval_sum<double> is;
    std::size_t n;
};

class kmeans_fast : public kmeans {
public:
    kmeans_fast(const std::vector<double> &points);
    std::unique_ptr<kmeans_result> compute(size_t k) override;
private:
    interval_sum<double> is;
    double cimj(size_t i, size_t m, size_t j);
    void reduce(size_t row_multiplier, size_t *cols, size_t n, size_t m,
                size_t *cols_output, size_t reduce_i);
    void mincompute(size_t row_multiplier, size_t *cols, size_t n, size_t m,
                    size_t reduce_i, size_t *cols_output);
    void fill_row(size_t k);
    void base_case(size_t k);
};

class kmeans_medi : public kmeans {
public:
    kmeans_medi(const std::vector<double> &points);
    std::unique_ptr<kmeans_result> compute(size_t k) override;
private:

    void fill_row_rec(size_t begin, size_t end, size_t k,
                      int64_t split_left, int64_t split_right);
    void fill_row(size_t k);
    interval_sum<double> is;
    std::vector<double> row;
    std::vector<double> row_prev;
    size_t n;
};

class kmeans_slow : public kmeans {
public:
    kmeans_slow(const std::vector<double> &points);
    std::unique_ptr<kmeans_result> compute(size_t k) override;
private:
    interval_sum<double> is;
    std::vector<double> row;
    std::vector<double> row_prev;
    size_t n;
};


typedef double (*kmeans_fn)(double *points, size_t n,
                            double *last_row, size_t k);


kmeans_fn get_kmeans_slow();

kmeans_fn get_kmeans_medi();

kmeans_fn get_kmeans_fast();

kmeans_fn get_kmeans_lloyd();

kmeans_fn get_kmeans_lloyd_slow();

kmeans_fn get_kmeans_hirsch_larmore();

double report_clusters(double *points, size_t n,
                       double *centers, size_t k,
                       kmeans_fn);

#endif /* __KMEANS_H__ */
