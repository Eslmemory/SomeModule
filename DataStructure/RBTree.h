#pragma once

#include <iostream>

namespace DS {
	
	enum Color {
		RED = 0, BLACK
	};

	template<class Key, class Value>
	struct RBTreeNode {
		RBTreeNode<Key, Value>* Left;
		RBTreeNode<Key, Value>* Right;
		RBTreeNode<Key, Value>* Parent;
		std::pair<Key, Value> KV;
		Color color;
		RBTreeNode(const std::pair<Key, Value>& val)
			: Left(nullptr), Right(nullptr), Parent(nullptr), KV(val), color(RED)
		{
		}
	};

	template<class Key, class Value>
	class RBTree {
		typedef RBTreeNode<Key, Value> Node;
	public:
		RBTree()
			: m_Root(nullptr)
		{}

		~RBTree() {
			DestroyRBTree(m_Root);
		}

		void RightRotate(Node* parent) {
			Node* leftChild = parent->Left;
			Node* grandParent = parent->Parent;
			parent->Left = leftChild->Right;
			if (leftChild->Right != nullptr) {
				leftChild->Right->Parent = parent;
			}
			leftChild->Right = parent;
			parent->Parent = leftChild;

			if (parent == m_Root) {
				m_Root = leftChild;
				leftChild->Parent = grandParent;
			}
			else {
				if (grandParent->Left == parent)
					grandParent->Left = leftChild;
				else
					grandParent->Right = leftChild;
				leftChild->Parent = grandParent;
			}
		}

		void LeftRotate(Node* parent) {
			Node* rightChild = parent->Right;
			Node* grandParent = parent->Parent;

			parent->Right = rightChild->Left;
			if (rightChild->Left != nullptr) {
				rightChild->Left->Parent = parent;
			}
			rightChild->Left = parent;
			parent->Parent = rightChild;

			if (parent == m_Root) {
				m_Root = rightChild;
				rightChild->Parent = grandParent;
			}
			else {
				if (grandParent->Left == parent)
					grandParent->Left = rightChild;
				else
					grandParent->Right = rightChild;
				rightChild->Parent = grandParent;
			}
		}

		bool CheckRBTree() {
			if (m_Root == nullptr)
				return true;

			if (m_Root->color == RED) {
				std::cout << "root color is red" << std::endl;
				return false;
			}

			int blackNum = 0;
			Node* leftBase = m_Root;
			while (leftBase != nullptr) {
				if (leftBase->color == BLACK) {
					blackNum++;
				}
				leftBase = leftBase->Left;
			}

			int num = 0;
			return _CheckRBTree(m_Root, blackNum, num);
		}

		Node* Search(const Key& key) {
			Node* cur = m_Root;
			Node* ret = nullptr;
			while (cur != nullptr) {
				if (key > cur->KV.first)
					cur = cur->Right;
				else if (key < cur->KV.first)
					cur = cur->Left;
				else {
					ret = cur;
					break;
				}
			}
			return ret;
		}

		std::pair<Node*, bool> Insert(const std::pair<Key, Value> val) {
			if (m_Root == nullptr) {
				m_Root = new Node(val);
				m_Root->color = BLACK;
				return std::make_pair(m_Root, true);
			}

			Node* cur = m_Root;
			Node* parent = nullptr;
			while (cur != nullptr) {
				if (cur->KV.first < val.first) {
					parent = cur;
					cur = cur->Right;
				}
				else if (cur->KV.first > val.first) {
					parent = cur;
					cur = cur->Left;
				}
				else {
					return std::make_pair(cur, false);
				}
			}

			cur = new Node(val);
			cur->color = RED;
			if (parent->KV.first > val.first) {
				parent->Left = cur;
			}
			else {
				parent->Right = cur;
			}
			cur->Parent = parent;

			while (parent != nullptr && parent->color == RED) {
				Node* grandParent = parent->Parent;
				if (parent == grandParent->Left) {
					Node* uncle = grandParent->Right;
					if (uncle && uncle->color == RED) {
						parent->color = uncle->color = BLACK;
						grandParent->color = RED;
						cur = grandParent;
						parent = cur->Parent;
					}
					else {
						if (cur == parent->Left) {
							RightRotate(grandParent);
							grandParent->color = RED;
							parent->color = BLACK;
						}
						else {
							LeftRotate(parent);
							RightRotate(grandParent);
							cur->color = BLACK;
							parent->color = grandParent->color = RED;
						}
						break;
					}
				}
				else {
					Node* uncle = grandParent->Left;
					if (uncle && uncle->color == RED) {
						parent->color = uncle->color = BLACK;
						grandParent->color = RED;
						cur = grandParent;
						parent = cur->Parent;
					}
					else {
						if (cur == parent->Right) {
							LeftRotate(grandParent);
							grandParent->color = RED;
							parent->color = BLACK;
						}
						else {
							RightRotate(parent);
							LeftRotate(grandParent);
							cur->color = BLACK;
							parent->color = grandParent->color = RED;
						}
						break;
					}
				}
			}

			m_Root->color = BLACK;
			return std::make_pair(cur, true);
		}

		Node* GetSuccessor(Node* node) {
			Node* tmpNode;
			if (node->Left != nullptr) {
				tmpNode = node->Left;
				while (tmpNode->Right != nullptr) {
					tmpNode = tmpNode->Right;
				}
			}
			else {
				tmpNode = node->Right;
				while (tmpNode->Left != nullptr) {
					tmpNode = tmpNode->Left;
				}
			}
			return tmpNode;
		}

		void FixDelete(Node* node) {
			Node* parent = node->Parent;
			if (parent == nullptr)
				node->color = BLACK;

			if (parent->Right == node) {
				Node* brother = parent->Left;
				if (brother->color == BLACK) {
					if (brother->Left->color == RED) {
						brother->color = parent->color;
						brother->Left->color = BLACK;
						parent->color = BLACK;
						RightRotate(parent);
					}
					else if (brother->Right->color == RED) {
						brother->color = RED;
						brother->Right->color = BLACK;
						LeftRotate(brother);
						FixDelete(node);
						return;
					}
					else {
						if (parent->color == RED) {
							parent->color = BLACK;
							brother->color = RED;
						}
						else {
							brother->color = RED;
							FixDelete(parent);
						}
					}
				}
				else {
					parent->color = RED;
					brother->color = BLACK;
					RightRotate(parent);
					FixDelete(node);
				}
			}
			// TODO
			else {
				Node* brother = m_Root->Right;
				if (brother->color == BLACK) {
					if (brother->Left->color == RED) {
						brother->color = RED;
						brother->Left->color = BLACK;
						RightRotate(brother);
						FixDelete(node);
					}
					else if (brother->Right->color = RED) {
						brother->color = parent->color;
						parent->color = BLACK;
						brother->Right->color = BLACK;
						LeftRotate(parent);
					}
					else {
						if (parent->color == RED) {
							parent->color = BLACK;
							brother->color = RED;
						}
						else {
							brother->color = RED;
							FixDelete(parent);
						}
					}
				}
				else {
					parent->color = RED;
					brother->color = BLACK;
					LeftRotate(parent);
					FixDelete(node);
				}
			}
		}

		void SwapValue(Node* node1, Node* node2) {
			Key key = node1->KV.first;
			Value value = node1->KV.second;
			node1->KV = { node2->KV.first, node2->KV.second };
			node2->KV = { key, value };
		}

		Node* DeleteNode(Node* node) {
			Node* parent = node->Parent;
			if (node->Left == nullptr && node->Right == nullptr) {
				if (node->color != RED)
					FixDelete(node);
				if (parent == nullptr)
					m_Root = nullptr;
				else {
					node->Parent = nullptr;
					if (parent->Left == node)
						parent->Left = nullptr;
					else
						parent->Right = nullptr;
				}
			}
			else {
				Node* tmpNode = GetSuccessor(node);
				SwapValue(tmpNode, node);
				DeleteNode(tmpNode);
				delete tmpNode;
			}
			
			return node;
		}

		// return: the infomation of swap node(successor/precursor)
		Node* Delete(const Key& key) {
			Node* node = Search(key);
			Node* curNode = DeleteNode(node);
			return curNode;
		}

		void PrintRBTree() {
			_PrintRBTree(m_Root);
		}

		void _PrintRBTree(Node* node) {
			if (node == nullptr) {
				return;
			}

			_PrintRBTree(node->Left);
			std::cout << node->KV.second << std::endl;
			_PrintRBTree(node->Right);
		}
	
	private:
		bool _CheckRBTree(Node* node, int blackNum, int num) {
			if (node == nullptr) {
				if (blackNum == num)
					return true;
				else {
					std::cout << "the num of black node is not same" << std::endl;
					return false;
				}
			}

			if (node->color == RED && node->Parent->color == RED) {
				std::cout << "red node is neighbor" << std::endl;
				return false;
			}
			else if (node->color == BLACK)
				num++;

			return _CheckRBTree(node->Left, blackNum, num) && _CheckRBTree(node->Right, blackNum, num);
		}

		void DestroyRBTree(Node* node) {
			if (node == nullptr)
				return;
			DestroyRBTree(node->Left);
			DestroyRBTree(node->Right);
			delete node;
		}

	private:
		Node* m_Root;
	};
}