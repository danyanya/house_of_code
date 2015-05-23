//
//  task2
// baseline predictor
//
//  Created by danyanya on 06.03.15.
//

#include <iostream>
#include <vector>
#include <iomanip>
#include <istream>
#include <fstream>
#include <cmath>
#include <string>


class One_elem {
public:
    int user;
    int film;
    double mark;
};


class SVD_plus_plus {
private:
    // constants
    int MAX_FEATURES;
    int MAX_ITERATIONS;
    // input variables
    int film_amt;
    int people_amt;
    int mark_amt;
    int learn_amt;
    int test_amt;
    
    // input learning data
    std::vector<One_elem> learn_data;
    
    // variables for baseline predictors
    double all_avg_mark;
    
    // features vectors
    std::vector<std::vector<double>> user_factors;
    std::vector<std::vector<double>> film_factors;
    
    // baseline predictors vectors
    std::vector<double> bias_user;
    std::vector<double> bias_film;
    
public:
    void init(std::istream &in) {
        MAX_ITERATIONS = 25;
        MAX_FEATURES = 25;
        // reading
        in >> mark_amt >> people_amt >> film_amt >> learn_amt >> test_amt;
        std::string trash;
        std::getline(in, trash);
        learn_data.resize(learn_amt);
        if (learn_amt != 0) {
            for (auto index = 0; index != learn_amt; ++index) {
                int first_elem, second_elem, third_elem;
                std::cin >> first_elem >> second_elem >> third_elem;
                learn_data[index].user = first_elem;
                learn_data[index].film = second_elem;
                learn_data[index].mark = third_elem;
                all_avg_mark += third_elem;
            }
        }
        all_avg_mark /= learn_amt;
        // resizing nessecery vectors
        bias_user.resize(people_amt);
        bias_film.resize(film_amt);
        user_factors.resize(MAX_FEATURES, std::vector<double>(people_amt, 0.1));
        film_factors.resize(MAX_FEATURES, std::vector<double>(film_amt, 0.1));
    }
    void learning() {
        
        // constants
        double EPS = 1e-10;
        double LRATE_ub = 0.012;
        double LRATE_mb = 0.003;
        double LAMBDA_ub = 0.03;
        double LAMBDA_mb = 0.001;
        double LEARNING_RATE_USER = 0.006;
        double LEARNING_RATE_MOVIE = 0.011;
        const double LEARNING_RATE_DECREASE_RATE = 0.9;
        double K_user = 0.08;
        double K_movie = 0.006;
        
        // variables for learning work
        double error_func;
        double eta = 0.1;
        
        // variables for rmse
        double rmse;
        double old_rmse;
        double threshold = 0.1;
        
        int iteration = 0;
        double temp_predictor;
        int temp_user;
        int temp_film;
        while (iteration != MAX_ITERATIONS || abs(rmse - old_rmse) > EPS) {
            old_rmse = rmse;
            rmse = 0.0;
            for (auto index = 0; index != learn_amt; ++index) {
                temp_user = learn_data[index].user;
                temp_film = learn_data[index].film;
                temp_predictor = all_avg_mark + bias_user[temp_user] +
                bias_film[temp_film] +
                compute_vectores(temp_user, temp_film);
                temp_predictor = (temp_predictor > mark_amt) ? mark_amt : ((temp_predictor < 1.0) ?
                    1.0 : temp_predictor);
                
                error_func = learn_data[index].mark - temp_predictor;
                rmse = error_func * error_func;
                all_avg_mark += eta * error_func;
                
                bias_user[temp_user] += LRATE_ub * (error_func -
                                                    LAMBDA_ub * bias_user[temp_user]);
                bias_film[temp_film] += LRATE_mb * (error_func -
                                                    LAMBDA_mb * bias_film[temp_film]);
                
                for (auto jndex = 0; jndex != MAX_FEATURES; ++jndex) {
                    user_factors[jndex][temp_user] += LEARNING_RATE_USER * (error_func *
                        film_factors[jndex][temp_film] - K_user * user_factors[jndex][temp_user]);
                    film_factors[jndex][temp_film] += LEARNING_RATE_MOVIE * (error_func *
                        user_factors[jndex][temp_user] - K_movie * film_factors[jndex][temp_film]);
                }
            }
            rmse = sqrt(rmse / learn_amt);
            // decrease the learning rates
            
            LEARNING_RATE_USER *= LEARNING_RATE_DECREASE_RATE;
            LEARNING_RATE_MOVIE *= LEARNING_RATE_DECREASE_RATE;
            LRATE_ub *= LEARNING_RATE_DECREASE_RATE;
            LRATE_mb *= LEARNING_RATE_DECREASE_RATE;
            
            if (rmse > old_rmse - threshold)
            {
                eta = eta * 0.66;
                threshold = threshold * 0.5;
            }
            ++iteration;
        }
    }
    void make_solution(std::istream &in, std::ostream &out) {
        int test_user, test_film;
        double solution;
        for (auto index = 0; index != test_amt; ++index) {
            in >> test_user >> test_film;
            solution = all_avg_mark + bias_user[test_user] + bias_film[test_film] +
            compute_vectores(test_user, test_film);
            solution = (solution > mark_amt) ? mark_amt : ((solution < 1.0) ? 1.0 : solution);
            out << std::fixed << std::setprecision(6) << solution << std::endl;
        }
    }
    double compute_vectores(int cur_user, int cur_film) {
        double solution = 0.0;
        for (auto index = 0; index != MAX_FEATURES; ++index) {
            solution += user_factors[index][cur_user] * film_factors[index][cur_film];
        }
        return solution;
    }
};


int main(int argc, char** argv) {
    SVD_plus_plus svd_model;
    svd_model.init(std::cin);
    svd_model.learning();
    svd_model.make_solution(std::cin, std::cout);
    return 0;
}
