#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch) {
    size_t pos = txt.find(ch);
    size_t initialPos = 0;
    strs.clear();
    while (pos != std::string::npos) {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;
        pos = txt.find(ch, initialPos);
    }
    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));
    return strs.size();
}

int get_priority(std::string &operation) {
    if (operation == "~") {
        return 6;
    } else if (operation == "/\\") {
        return 5;
    } else if (operation == "\\/" || operation == "^") {
        return 4;
    } else if (operation == "->") {
        return 3;
    } else if (operation == "<=>") {
        return 2;
    } else if (operation == "|" || operation == "!") {
        return 1;
    } else if (operation == "(") {
        return 0;
    }
    return -1;
}

void
process_operation(std::string &operation, std::vector<std::stack<bool>> &calculating_stacks) {
    if (operation == "~") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v = calculating_stack.top();
            calculating_stack.pop();
            v = !v;
            calculating_stack.push(v);
        }
    } else if (operation == "/\\") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v1 = calculating_stack.top();
            calculating_stack.pop();
            bool v2 = calculating_stack.top();
            calculating_stack.pop();
            v1 = v2 & v1;
            calculating_stack.push(v1);
        }
    } else if (operation == "\\/") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v1 = calculating_stack.top();
            calculating_stack.pop();
            bool v2 = calculating_stack.top();
            calculating_stack.pop();
            v1 = v2 | v1;
            calculating_stack.push(v1);
        }
    } else if (operation == "^") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v1 = calculating_stack.top();
            calculating_stack.pop();
            bool v2 = calculating_stack.top();
            calculating_stack.pop();
            v1 = v2 ^ v1;
            calculating_stack.push(v1);
        }
    } else if (operation == "->") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v1 = calculating_stack.top();
            calculating_stack.pop();
            bool v2 = calculating_stack.top();
            calculating_stack.pop();
            v1 = !v2 | v1;
            calculating_stack.push(v1);
        }
    } else if (operation == "<=>") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v1 = calculating_stack.top();
            calculating_stack.pop();
            bool v2 = calculating_stack.top();
            calculating_stack.pop();
            v1 = (v2 & v1) | (!v1 & !v2);
            calculating_stack.push(v1);
        }
    } else if (operation == "|") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v1 = calculating_stack.top();
            calculating_stack.pop();
            bool v2 = calculating_stack.top();
            calculating_stack.pop();
            v1 = !v2 | !v1;
            calculating_stack.push(v1);
        }
    } else if (operation == "!") {
        for (auto &calculating_stack : calculating_stacks) {
            bool v1 = calculating_stack.top();
            calculating_stack.pop();
            bool v2 = calculating_stack.top();
            calculating_stack.pop();
            v1 = !v2 & !v1;
            calculating_stack.push(v1);
        }
    }
}

int main() {
    std::ifstream fin("input.txt");
    std::string input;
    std::getline(fin, input);

    std::vector<std::string> words;
    split(input, words, ' ');
    std::stack<std::string> operations;

    int number_of_variables = 0;
    std::vector<bool> is_var_present;
    for (auto &word : words) {
        if (word.at(0) == 'x') {
            std::string buf(word);
            buf.erase(0, 1);
            int var_num = std::stoi(buf);
            if (var_num > number_of_variables) {
                number_of_variables = var_num;
                is_var_present.resize(var_num);
            }
            is_var_present[var_num - 1] = true;
        }
    }

    int number_of_calc_vars = 0;
    std::map<int, int> var_mapping;
    std::map<int, int> var_mapping_rev;
    for (int i = 0; i < number_of_variables; i++) {
        if (is_var_present[i]) {
            number_of_calc_vars++;
            var_mapping.emplace(i + 1, number_of_calc_vars);
            var_mapping_rev.emplace(number_of_calc_vars, i + 1);
        }
    }

    std::vector<std::stack<bool>> calculating_stacks(1 << number_of_calc_vars);

    for (std::string &word : words) {
        if (word == "(") {
            operations.push("(");
        } else if (word == ")") {
            while (operations.top() != "(") {
                process_operation(operations.top(), calculating_stacks);
                operations.pop();
            }
            operations.pop();
        } else if (get_priority(word) != -1) {
            while (!operations.empty() && get_priority(word) < get_priority(operations.top())) {
                process_operation(operations.top(), calculating_stacks);
                operations.pop();
            }
            operations.push(word);
        } else {
            if (word.at(0) == 'x') {
                word.erase(0, 1);
                int var_num = std::stoi(word);
                var_num = var_mapping.at(var_num);
                var_num = 1 << (number_of_calc_vars - var_num);
                for (int i = 0; i < calculating_stacks.size(); i++) {
                    calculating_stacks[i].push((i / var_num) % 2 != 0);
                }
            } else {
                bool v = (word == "1");
                for (auto &calculating_stack : calculating_stacks) {
                    calculating_stack.push(v);
                }
            }
        }
    }
    while (!operations.empty()) {
        process_operation(operations.top(), calculating_stacks);
        operations.pop();
    }

    std::vector<bool> function_vec;
    function_vec.reserve(calculating_stacks.size());
    for (auto &calculating_stack : calculating_stacks) {
        function_vec.push_back(calculating_stack.top());
    }

    for (int i = 0; i < number_of_calc_vars; i++) {
        int chunk = 1 << i;
        int index = chunk;
        for (int j = 0; j < 1 << (number_of_calc_vars - i - 1); j++) {
            for (int k = 0; k < chunk; k++) {
                function_vec[index + k] = function_vec[index + k] ^ function_vec[index + k - chunk];
            }
            index += chunk * 2;
        }
    }

    std::vector<bool> result(1 << number_of_variables);
    for (int i = 0; i < function_vec.size(); i++) {
        if (function_vec[i]) {
            std::vector<int> var_nums;
            int index = i;
            int var_num = 0;
            while (index != 0) {
                if (index % 2) {
                    var_nums.push_back(number_of_calc_vars - var_num);
                }
                index /= 2;
                var_num++;
            }
            int result_index = 0;
            for (auto element : var_nums) {
                if (element != 0) {
                    result_index += 1 << (number_of_variables - var_mapping_rev.at(element));
                }
            }
            result[result_index] = true;
        }
    }

    std::ofstream fout("output.txt");
    for (bool element : result) {
        fout << (element ? 1 : 0);
    }
    return 0;
}