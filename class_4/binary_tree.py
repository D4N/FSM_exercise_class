# -*- coding: utf-8 -*-

from __future__ import division, print_function


class binary_tree_node(object):

	def __init__(self, value = 0.):

		self.node_data = value
		self.left = None
		self.right = None

def insert_into_tree(tree_root, new_value):
	"""
	Inserts value into the tree.
	"""

	if tree_root == None:

		tree_root = binary_tree_node(value = new_value)
		return 0

	elif new_value < tree_root.node_data:

		if tree_root.left == None:
			tree_root.left = binary_tree_node(value = new_value)


		else:
			insert_into_tree(tree_root.left, new_value)

		return 0

	elif new_value > tree_root.node_data:

		if tree_root.right == None:
			tree_root.right = binary_tree_node(value = new_value)

		else:
			insert_into_tree(tree_root.right, new_value)

		return 0

	else:
		raise ValueError("Value " + str(new_value) + " already present in tree")


def print_tree(tree_root):

	if tree_root == None:
		print("empty")

	else:
		print(tree_root.node_data)
		print("left ", end="")
		print_tree(tree_root.left)
		print("right ", end="")
		print_tree(tree_root.right)

def find_value_in_tree(tree_root, value):

	if tree_root == None:
		raise ValueError("Value " + str(value) + " not found in tree")

	elif tree_root.node_data == value:
		return tree_root

	elif value < tree_root.node_data:
		return find_value_in_tree(tree_root.left, value)

	elif value > tree_root.node_data:
		return find_value_in_tree(tree_root.right, value)

def test_tree():

	tree_root = binary_tree_node(5)

	for number in [3, 7, 2, 4, 6, 8, 1]:
		insert_into_tree(tree_root, number)

	print_tree(tree_root)

	for number in xrange(10):
		try:
			find_value_in_tree(tree_root, number)
			print(str(number) +" found")
		except:
			print(str(number) +" not found")



if __name__ == '__main__':

	test_tree()
