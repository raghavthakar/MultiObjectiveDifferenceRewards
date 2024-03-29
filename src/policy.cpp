#include "policy.h"
#include "torch/torch.h"

Policy::Policy() {}

Policy::Policy(int inputSize, double weightLowerLimit, double weightUpperLimit) {
    // Define the layers of the neural network
    fc1 = register_module("fc1", torch::nn::Linear(inputSize, 6));
    fc2 = register_module("fc2", torch::nn::Linear(6, 6));
    fc3 = register_module("fc3", torch::nn::Linear(6, 2));

    // Initialize weights with random values
    torch::NoGradGuard no_grad; // Disable gradient computation temporarily
    torch::nn::init::uniform_(fc1->weight, weightLowerLimit, weightUpperLimit);
    torch::nn::init::uniform_(fc2->weight, weightLowerLimit, weightUpperLimit);
    torch::nn::init::uniform_(fc3->weight, weightLowerLimit, weightUpperLimit);
}

std::pair<double, double> Policy::forward(const std::vector<double>& input) {
    // Convert input vector to a torch::Tensor
    torch::Tensor x = torch::tensor(input).view({1, -1});

    // Apply the layers sequentially with ReLU activation
    x = torch::relu(fc1->forward(x));
    x = torch::relu(fc2->forward(x));
    x = fc3->forward(x);

    // Extract output values from tensor and return as pair
    auto output_tensor = x.squeeze();
    double delta_x = output_tensor[0].item<double>();
    double delta_y = output_tensor[1].item<double>();
    return {delta_x, delta_y};
}

// Display method to print out the weights of each layer
void Policy::display() {
    std::cout << "Weights of fc1:" << std::endl;
    displayWeights(fc1->weight);
    std::cout << "Weights of fc2:" << std::endl;
    displayWeights(fc2->weight);
    std::cout << "Weights of fc3:" << std::endl;
    displayWeights(fc3->weight);
}

// Function to display the weights of a tensor
void Policy::displayWeights(const torch::Tensor& weight) {
    auto weight_accessor = weight.accessor<float, 2>();
    for (int i = 0; i < weight_accessor.size(0); ++i) {
        for (int j = 0; j < weight_accessor.size(1); ++j) {
            std::cout << weight_accessor[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
