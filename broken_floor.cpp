//  
//  task3
//  broken floor (испорченный паркет)
//
//  Created by danyanya 05.03.15
//  Previous version was so beautiful, but with TL.


#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

std::vector<std::string> input_data;
std::vector<std::vector<int>> graph;
std::vector<int> current_pairs;
std::vector<int> used;
int counter;
int number_of_rows, number_of_cols, cost_of_type_a, cost_of_type_b;

bool is_broken(int row, int col) {
    return row >= 0 && row < number_of_rows && col >= 0 && 
        col < number_of_cols && input_data[row][col] == '*';
}

bool depth_search(int vert_from) {
    if  (used[vert_from] == counter) {
        return false;
    }
    used[vert_from] = counter;
    for (auto vert_to : graph[vert_from]) {
        if  (current_pairs[vert_to] == -1 || depth_search(current_pairs[vert_to])) {
            current_pairs[vert_to] = vert_from;
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv) {
    std::cin >> number_of_rows >> number_of_cols >> cost_of_type_a >> cost_of_type_b;
    if (number_of_rows == 0 || number_of_cols == 0) {
        return 0;
    }
    input_data.resize(number_of_rows);
    getline(std::cin, input_data[0]);
    for (auto index_i = 0; index_i != number_of_rows; ++index_i) {
        getline(std::cin, input_data[index_i]);
        input_data[index_i].resize(number_of_cols);
    }

    int directions[4] = {0, 0, 1, -1};
    int number_of_broken = 0, amount = number_of_cols * number_of_rows;
    graph.resize(amount);
    for (auto index_i = 0; index_i != number_of_rows; ++index_i) {
        for (auto index_j = 0; index_j != number_of_cols; ++index_j) {
            if (is_broken(index_i, index_j)) {
                ++number_of_broken;
                if ((index_i + index_j) % 2) { 
                    for (auto cur_dir = 0; cur_dir != 4; ++cur_dir) {
                        int sht_i = index_i + directions[cur_dir];
                        int sht_j = index_j + directions[3 - cur_dir];
                        if (is_broken(sht_i, sht_j)) {
                            int index_cuz_line_so_long = index_i * number_of_cols + index_j;
                            graph[index_cuz_line_so_long].push_back(sht_i * number_of_cols + sht_j);
                        }
                    }
                }    
            }
        }
    }

    if ((cost_of_type_b << 1) <= cost_of_type_a) {
        std::cout << number_of_broken * cost_of_type_b << std::endl;
    } else {
        current_pairs.resize(amount, -1);
        used.resize(amount, -1);
        counter = 0;
        for (auto vert_from = 0; vert_from != amount; ++vert_from) {
            ++counter;
            depth_search(vert_from);
        }
        int number_of_pairs = 0;
        for (auto pair: current_pairs) {
            number_of_pairs += (pair != -1);
        }
        int answer = number_of_pairs * cost_of_type_a;
        answer += (number_of_broken - (number_of_pairs << 1)) * cost_of_type_b;
        std::cout << answer << std::endl;
    }
    return 0;
}
