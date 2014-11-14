#include <iostream>
#include <random>

#include "binary_tree.hpp"


int insert_into_tree(binary_tree_node * root_node, int new_value) {

	//std::cout << "inserting number " << new_value << std::endl;

	// no tree is present -> create the first node
	if (root_node == nullptr) {

		std::cout << "creating node" << std::endl;

		root_node = new binary_tree_node;
		root_node->node_data = new_value;
		root_node->left = nullptr;
		root_node->right = nullptr;
		return 0;
	}

	// insert on the left side
	if (new_value < root_node->node_data) {
		//std::cout << "inserting left" << std::endl;

		// no left node present,create new left node
		if (root_node->left == nullptr) {
			root_node->left = new binary_tree_node;
			root_node->left->node_data = new_value;
			root_node->left->left = nullptr;
			root_node->left->right = nullptr;
			return 0;
		}
		// there is a node on the left
		else {
			return insert_into_tree(root_node->left, new_value);
		}
	}
	// insert on the right side
	else if(new_value > root_node->node_data) {
		//std::cout << "inserting right" << std::endl;

		// no right node present,create new left node
		if (root_node->right == nullptr) {
			root_node->right = new binary_tree_node;
			root_node->right->node_data = new_value;
			root_node->right->left = nullptr;
			root_node->right->right = nullptr;
			return 0;
		}
		// there is a node on the left
		else {
			return insert_into_tree(root_node->right, new_value);
		}
	}
	else {
		// value already present in tree -> error
		return 1;
	}
}

void print_tree(binary_tree_node * root_node) {

	if (root_node != nullptr) {
		std::cout << root_node->node_data << std::endl;
		std::cout << "left "; print_tree(root_node->left);
		std::cout << "right "; print_tree(root_node->right);
	}
	else {
		std::cout << "empty" << std::endl;
	}
}


binary_tree_node * find_value_in_tree(binary_tree_node * root, int value_to_find) {

	// value not found:
	if (root == nullptr) {
		return nullptr;
	}
	if (root->node_data == value_to_find) {
		return root;
	}
	if (value_to_find < root->node_data) {
		return find_value_in_tree(root->left, value_to_find);
	}
	if (value_to_find > root->node_data) {
		return find_value_in_tree(root->right, value_to_find);
	}

	// fallback
	return nullptr;
}

int calc_nodes(binary_tree_node * root) {
	if (root == nullptr) { return 0; }
	return 1 + calc_nodes(root->left) + calc_nodes(root->right);
}

binary_tree_node * example_tree() {
	int numbers[7] = {3, 7, 2, 4, 6, 8, 1};
	binary_tree_node * root = new binary_tree_node;
	//(*root).node_data
	root->node_data = 5;

	//binary_tree_node root2 (5);
	//root2.node_data = 10;

	for (int i=0; i<7; i++) {
		//std::cout << numbers[i] << std::endl;
		insert_into_tree(root, numbers[i]);
	}

	//std::cout << std::endl;

	print_tree(root);

	for (int i=0; i<10; i++) {
		binary_tree_node * test = find_value_in_tree(root, i);
		if (test == nullptr) {
			std::cout << i << " not found" << std::endl;
		}
		else {
			std::cout << i << " found" << std::endl;
		}

	}

	std::cout << "number of nodes in the tree: " << calc_nodes(root) << std::endl;

	return root;
}

int main() {
	example_tree();
	return 0;
}
