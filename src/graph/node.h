#include <vector>

class Node {
public:
    Node();

    std::vector<int> get_inputs() const;

    void set_inputs(std::vector<int>& indices);

    std::vector<int> get_outputs() const;

    void set_outputs(std::vector<int>& indices);

private:
    std::vector<int> inputs_;
    std::vector<int> outputs_;
};