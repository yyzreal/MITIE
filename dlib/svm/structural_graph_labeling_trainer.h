// Copyright (C) 2012  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_STRUCTURAL_GRAPH_LABELING_tRAINER_H__
#define DLIB_STRUCTURAL_GRAPH_LABELING_tRAINER_H__

#include "structural_graph_labeling_trainer_abstract.h"
#include "../algs.h"
#include "../optimization.h"
#include "structural_svm_graph_labeling_problem.h"
#include "graph_labeler.h"


namespace dlib
{

// ----------------------------------------------------------------------------------------

    template <
        typename vector_type 
        >
    class structural_graph_labeling_trainer
    {
    public:
        typedef std::vector<node_label> label_type;
        typedef graph_labeler<vector_type> trained_function_type;

        structural_graph_labeling_trainer (
        )  
        {
            C = 10;
            verbose = false;
            eps = 0.1;
            num_threads = 2;
            max_cache_size = 40;
        }

        void set_num_threads (
            unsigned long num
        )
        {
            num_threads = num;
        }

        unsigned long get_num_threads (
        ) const
        {
            return num_threads;
        }

        void set_epsilon (
            double eps_
        )
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(eps_ > 0,
                "\t void structural_graph_labeling_trainer::set_epsilon()"
                << "\n\t eps_ must be greater than 0"
                << "\n\t eps_: " << eps_ 
                << "\n\t this: " << this
                );

            eps = eps_;
        }

        double get_epsilon (
        ) const { return eps; }

        void set_max_cache_size (
            unsigned long max_size
        )
        {
            max_cache_size = max_size;
        }

        unsigned long get_max_cache_size (
        ) const
        {
            return max_cache_size; 
        }

        void be_verbose (
        )
        {
            verbose = true;
        }

        void be_quiet (
        )
        {
            verbose = false;
        }

        void set_oca (
            const oca& item
        )
        {
            solver = item;
        }

        const oca get_oca (
        ) const
        {
            return solver;
        }

        void set_c (
            double C_ 
        )
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(C_ > 0,
                "\t void structural_graph_labeling_trainer::set_c()"
                << "\n\t C_ must be greater than 0"
                << "\n\t C_:    " << C_ 
                << "\n\t this: " << this
                );

            C = C_;
        }

        double get_c (
        ) const
        {
            return C;
        }

        template <
            typename graph_type
            >
        const graph_labeler<vector_type> train (  
            const dlib::array<graph_type>& samples,
            const std::vector<label_type>& labels
        ) const
        {
            // TODO
            // make sure requires clause is not broken

            structural_svm_graph_labeling_problem<graph_type> prob(samples, labels, num_threads);

            if (verbose)
                prob.be_verbose();

            prob.set_c(C);
            prob.set_epsilon(eps);
            prob.set_max_cache_size(max_cache_size);

            matrix<double,0,1> w;
            solver(prob, w, prob.get_num_edge_weights());

            vector_type edge_weights = rowm(w,range(0, prob.get_num_edge_weights()-1));
            vector_type node_weights = rowm(w,range(prob.get_num_edge_weights(),w.size()-1));
            return graph_labeler<vector_type>(edge_weights, node_weights);
        }

    private:

        double C;
        oca solver;
        double eps;
        bool verbose;
        unsigned long num_threads;
        unsigned long max_cache_size;
    };

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_STRUCTURAL_GRAPH_LABELING_tRAINER_H__

