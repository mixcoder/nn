#ifndef LSTM_H
#define LSTM_H

#include "la/la.h"
#include "autodiff/autodiff.h"
#include <random>

namespace lstm {

    void bound(la::vector_like<double>& u, double min, double max);
    void bound(la::matrix_like<double>& u, double min, double max);

    struct lstm_feat_param_t {
        la::matrix<double> hidden_input;
        la::matrix<double> hidden_output;
        la::vector<double> hidden_bias;

        la::matrix<double> input_input;
        la::matrix<double> input_output;
        la::vector<double> input_peep;
        la::vector<double> input_bias;

        la::matrix<double> output_input;
        la::matrix<double> output_output;
        la::vector<double> output_peep;
        la::vector<double> output_bias;

        la::matrix<double> forget_input;
        la::matrix<double> forget_output;
        la::vector<double> forget_peep;
        la::vector<double> forget_bias;
    };

    lstm_feat_param_t load_lstm_feat_param(std::istream& is);
    lstm_feat_param_t load_lstm_feat_param(std::string filename);

    void save_lstm_feat_param(lstm_feat_param_t const& p, std::ostream& os);
    void save_lstm_feat_param(lstm_feat_param_t const& p, std::string filename);

    void bound(lstm_feat_param_t& p, double min, double max);

    void const_step_update_momentum(lstm_feat_param_t& p, lstm_feat_param_t const& grad,
        lstm_feat_param_t& opt_data, double momentum, double step_size);

    void adagrad_update(lstm_feat_param_t& p, lstm_feat_param_t const& grad,
        lstm_feat_param_t& opt_data, double step_size);

    void rmsprop_update(lstm_feat_param_t& p, lstm_feat_param_t const& grad,
        lstm_feat_param_t& opt_data, double decay, double step_size);

    struct lstm_feat_nn_t {
        std::shared_ptr<autodiff::op_t> hidden_input;
        std::shared_ptr<autodiff::op_t> hidden_output;
        std::shared_ptr<autodiff::op_t> hidden_bias;

        std::shared_ptr<autodiff::op_t> input_input;
        std::shared_ptr<autodiff::op_t> input_output;
        std::shared_ptr<autodiff::op_t> input_peep;
        std::shared_ptr<autodiff::op_t> input_bias;

        std::shared_ptr<autodiff::op_t> output_input;
        std::shared_ptr<autodiff::op_t> output_output;
        std::shared_ptr<autodiff::op_t> output_peep;
        std::shared_ptr<autodiff::op_t> output_bias;

        std::shared_ptr<autodiff::op_t> forget_input;
        std::shared_ptr<autodiff::op_t> forget_output;
        std::shared_ptr<autodiff::op_t> forget_peep;
        std::shared_ptr<autodiff::op_t> forget_bias;

        std::vector<std::shared_ptr<autodiff::op_t>> cell;
        std::vector<std::shared_ptr<autodiff::op_t>> hidden;
        std::vector<std::shared_ptr<autodiff::op_t>> input_gate;
        std::vector<std::shared_ptr<autodiff::op_t>> output_gate;
        std::vector<std::shared_ptr<autodiff::op_t>> forget_gate;
        std::vector<std::shared_ptr<autodiff::op_t>> output;

        std::shared_ptr<autodiff::op_t> input_mask;
        std::shared_ptr<autodiff::op_t> hidden_mask;
    };

    lstm_feat_nn_t make_forward_lstm_feat_nn(autodiff::computation_graph& g,
        lstm_feat_param_t const& p,
        std::vector<std::shared_ptr<autodiff::op_t>> const& inputs);

    lstm_feat_nn_t make_backward_lstm_feat_nn(autodiff::computation_graph& g,
        lstm_feat_param_t const& p,
        std::vector<std::shared_ptr<autodiff::op_t>> const& inputs);

    lstm_feat_param_t copy_lstm_feat_grad(lstm_feat_nn_t const& nn);

    struct blstm_feat_param_t {
        lstm_feat_param_t forward_param;
        lstm_feat_param_t backward_param;

        la::matrix<double> forward_output_weight;
        la::matrix<double> backward_output_weight;
        la::vector<double> output_bias;
    };

    blstm_feat_param_t load_blstm_feat_param(std::istream& is);
    blstm_feat_param_t load_blstm_feat_param(std::string filename);

    void save_blstm_feat_param(blstm_feat_param_t const& p, std::ostream& os);
    void save_blstm_feat_param(blstm_feat_param_t const& p, std::string filename);

    void bound(blstm_feat_param_t& p, double min, double max);

    void const_step_update_momentum(blstm_feat_param_t& p, blstm_feat_param_t const& grad,
        blstm_feat_param_t& opt_data, double momentum, double step_size);

    void adagrad_update(blstm_feat_param_t& p, blstm_feat_param_t const& grad,
        blstm_feat_param_t& opt_data, double step_size);

    void rmsprop_update(blstm_feat_param_t& p, blstm_feat_param_t const& grad,
        blstm_feat_param_t& opt_data, double decay, double step_size);

    struct blstm_feat_nn_t {
        lstm_feat_nn_t forward_feat_nn;
        lstm_feat_nn_t backward_feat_nn;

        std::shared_ptr<autodiff::op_t> forward_output_weight;
        std::shared_ptr<autodiff::op_t> backward_output_weight;
        std::shared_ptr<autodiff::op_t> output_bias;

        std::vector<std::shared_ptr<autodiff::op_t>> output;
    };

    blstm_feat_nn_t make_blstm_feat_nn(autodiff::computation_graph& g,
        blstm_feat_param_t const& p,
        std::vector<std::shared_ptr<autodiff::op_t>> const& inputs);

    blstm_feat_param_t copy_blstm_feat_grad(blstm_feat_nn_t const& nn);

    struct dblstm_feat_param_t {
        std::vector<blstm_feat_param_t> layer;
    };

    dblstm_feat_param_t load_dblstm_feat_param(std::istream& is);
    dblstm_feat_param_t load_dblstm_feat_param(std::string filename);

    void save_dblstm_feat_param(dblstm_feat_param_t const& p, std::ostream& os);
    void save_dblstm_feat_param(dblstm_feat_param_t const& p, std::string filename);

    void bound(dblstm_feat_param_t& p, double min, double max);

    void const_step_update_momentum(dblstm_feat_param_t& p, dblstm_feat_param_t const& grad,
        dblstm_feat_param_t& opt_data, double momentum, double step_size);

    void adagrad_update(dblstm_feat_param_t& p, dblstm_feat_param_t const& grad,
        dblstm_feat_param_t& opt_data, double step_size);

    void rmsprop_update(dblstm_feat_param_t& p, dblstm_feat_param_t const& grad,
        dblstm_feat_param_t& opt_data, double decay, double step_size);

    struct dblstm_feat_nn_t {
        std::vector<blstm_feat_nn_t> layer;
    };

    dblstm_feat_nn_t make_dblstm_feat_nn(autodiff::computation_graph& graph,
        dblstm_feat_param_t const& p,
        std::vector<std::shared_ptr<autodiff::op_t>> const& inputs);

    dblstm_feat_param_t copy_dblstm_feat_grad(dblstm_feat_nn_t const& nn);

    void apply_random_mask(dblstm_feat_nn_t& nn, dblstm_feat_param_t const& param,
        std::default_random_engine& gen, double prob);

    void apply_mask(dblstm_feat_nn_t& nn, dblstm_feat_param_t const& param, double prob);

}

#endif