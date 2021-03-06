#ifndef RESIDUAL_H
#define RESIDUAL_H

#include "la/la.h"
#include "autodiff/autodiff.h"
#include "opt/opt.h"

namespace residual {

    // x -------------------------------------------------- add -- 
    //     \                                          /
    //      ---- relu -- lin -- add -- relu -- lin ---
    //                           |
    //                          lin
    //                           |
    //                           y

    struct unit_param_t {
        la::matrix<double> weight1;
        la::vector<double> bias1;
        la::matrix<double> weight2;
        la::vector<double> bias2;
    };

    void imul(unit_param_t& p, double a);
    void iadd(unit_param_t& p1, unit_param_t const& p2);

    unit_param_t load_unit_param(std::istream& is);
    void save_unit_param(unit_param_t const& p, std::ostream& os);

    void adagrad_update(unit_param_t& param, unit_param_t const& grad,
        unit_param_t& accu_grad_sq, double step_size);

    void rmsprop_update(unit_param_t& param, unit_param_t const& grad,
        unit_param_t& opt_data, double decay, double step_size);

    struct nn_unit_t {
        std::shared_ptr<autodiff::op_t> weight1;
        std::shared_ptr<autodiff::op_t> bias1;
        std::shared_ptr<autodiff::op_t> weight2;
        std::shared_ptr<autodiff::op_t> bias2;

        std::shared_ptr<autodiff::op_t> cell;
        std::shared_ptr<autodiff::op_t> output;
    };

    nn_unit_t make_unit_nn(autodiff::computation_graph& graph,
        std::shared_ptr<autodiff::op_t> cell,
        unit_param_t& param);

    void unit_nn_tie_grad(nn_unit_t const& nn, unit_param_t& grad);

    void resize_as(unit_param_t& p1, unit_param_t const& p2);

    unit_param_t copy_unit_grad(nn_unit_t const& unit);

    struct nn_param_t {
        std::vector<unit_param_t> layer;

        la::matrix<double> input_weight;
        la::vector<double> input_bias;
        la::matrix<double> softmax_weight;
        la::vector<double> softmax_bias;
    };

    void imul(nn_param_t& p, double a);
    void iadd(nn_param_t& p1, nn_param_t const& p2);

    nn_param_t load_nn_param(std::istream& is);
    nn_param_t load_nn_param(std::string filename);
    void save_nn_param(nn_param_t const& p, std::ostream& os);
    void save_nn_param(nn_param_t const& p, std::string filename);

    void adagrad_update(nn_param_t& param, nn_param_t const& grad,
        nn_param_t& accu_grad_sq, double step_size);

    void rmsprop_update(nn_param_t& param, nn_param_t const& grad,
        nn_param_t& opt_data, double decay, double step_size);

    struct nn_t {
        std::shared_ptr<autodiff::op_t> input;

        std::shared_ptr<autodiff::op_t> input_weight;
        std::shared_ptr<autodiff::op_t> input_bias;

        std::vector<nn_unit_t> layer;
    };

    nn_t make_nn(autodiff::computation_graph& graph,
        nn_param_t& param);

    void nn_tie_grad(nn_t const& nn, nn_param_t& grad);

    void resize_as(nn_param_t& p1, nn_param_t const& p2);

    nn_param_t copy_nn_grad(nn_t const& nn);

    struct y_unit_param_t {
        la::matrix<double> weight1;
        la::vector<double> bias1;
        la::matrix<double> weight2;
        la::vector<double> bias2;
        la::matrix<double> input_weight;
    };

    void imul(y_unit_param_t& p, double a);
    void iadd(y_unit_param_t& p1, y_unit_param_t const& p2);

    void resize_as(y_unit_param_t& p1, y_unit_param_t const& p2);

    y_unit_param_t load_y_unit_param(std::istream& is);
    void save_y_unit_param(y_unit_param_t const& p, std::ostream& os);

    void adagrad_update(y_unit_param_t& param, y_unit_param_t const& grad,
        y_unit_param_t& accu_grad_sq, double step_size);

    void rmsprop_update(y_unit_param_t& param, y_unit_param_t const& grad,
        y_unit_param_t& opt_data, double decay, double step_size);

    struct y_unit_nn_t {
        std::shared_ptr<autodiff::op_t> weight1;
        std::shared_ptr<autodiff::op_t> bias1;
        std::shared_ptr<autodiff::op_t> weight2;
        std::shared_ptr<autodiff::op_t> bias2;
        std::shared_ptr<autodiff::op_t> input_weight;

        std::vector<std::shared_ptr<autodiff::op_t>> cell;
    };

    y_unit_nn_t make_y_unit_nn(autodiff::computation_graph& graph,
        y_unit_param_t& param,
        std::vector<std::shared_ptr<autodiff::op_t>> const& inputs);

    void y_unit_nn_tie_grad(y_unit_nn_t const& nn, y_unit_param_t& grad);

    y_unit_param_t copy_y_unit_grad(y_unit_nn_t const& unit);

}

#endif
