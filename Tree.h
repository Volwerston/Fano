#pragma once

using namespace std;

template<class T> class Tree;

template<class T>
struct Node
{
	friend class Tree<T>;

	Node* left;
	Node* right;
	Node* parent;

	T data;
	int level;

	Node(T _d, Node* par = nullptr, int _l = 0)
		: data(_d),
		left(nullptr),
		right(nullptr),
		parent(par),
		level(_l)
	{
	}

	int getLevel() const
	{
		return level;
	}

	Node<T>* & getLeft()
	{
		return left;
	}

	Node<T>* & getRight()
	{
		return right;
	}


	void setLeftSon(const T& data)
	{
		Node<T>* newOne = new Node<T>(data, this, this->getLevel() + 1);
		this->left = newOne;
	}

	void setRightSon(const T& data)
	{
		Node<T>* newOne = new Node<T>(data, this, this->getLevel() + 1);
		this->right = newOne;
	}

};

template <class T>
class Tree
{
	Node<T>* root;

	void addAtNode(const T& dat, Node<T>* & node, Node<T>* parent, int level)
	{
		if (node == nullptr)
		{
			node = new Node<T>(dat, parent, level);
		}
		else
		{
			if (node->data < dat)
			{
				addAtNode(dat, node->right, node, level + 1);
			}
			else if (node->data > dat)
			{
				addAtNode(dat,node->left, node, level + 1);
			}
		}
	}

	void prefixFromNode(Node<T>* node) const
	{
		if (node != nullptr)
		{
			cout << node->data << " ";

			prefixFromNode(node->left);
			prefixFromNode(node->right);
		}
	}

	void postfixFromNode(Node<T>* node) const
	{
		if (node != nullptr)
		{
			postfixFromNode(node->left);
			postfixFromNode(node->right);

			cout << node->data << " ";
		}
	}

	void infixFromNode(Node<T>* node) const
	{
		if (node != nullptr)
		{
			infixFromNode(node->left);
			cout << node->data << " ";
			infixFromNode(node->right);
		}
	}

	void getLeavesFromNode(Node<T>* node, vector<T>& toReturn)
	{
		if (node != nullptr)
		{
			getLeavesFromNode(node->left, toReturn);

			if (node->left == nullptr && node->right == nullptr)
			{
				toReturn.push_back(node->data);
			}

			getLeavesFromNode(node->right, toReturn);
		}
	}

	void graphicFromNode(Node<T>* node) const // !
	{
		if (node != nullptr)
		{
			graphicFromNode(node->right);

			for (size_t i = 0; i < node->getLevel(); ++i)
			{
				cout << "   ";
			}

			cout << node->data << endl;

			graphicFromNode(node->left);
		}
	}

	void deleteFromNode(Node<T>* & node)
	{
		if (node != nullptr)
		{
			Node<T>* leftDel = node->left;
			Node<T>* rightDel = node->right;

			delete node;

			deleteFromNode(leftDel);
			deleteFromNode(rightDel);
		}
	}

	void findElement(const T& data, Node<T>* node, Node<T>*& toReturn)
	{
		if (node != nullptr)
		{

			if (node->data == data)
			{
				toReturn = node;
			}
			else
			{
				if (node->data > data)
				{
					findElement(data, node->left, toReturn);
				}
				else
				{
					findElement(data, node->right, toReturn);
				}
			}
		}
	}

	void findMinFromNode(Node<T>* node, Node<T>*& toReturn)
	{
		while (node->left != nullptr)
		{
			node = node->left;
		}

		toReturn = node;
	}

	void findMaxFromNode(Node<T>* node, Node<T>*& toReturn)
	{
		while (node->right != nullptr)
		{
			node = node->right;
		}

		toReturn = node;
	}

public:

	Tree()
		: root(nullptr)
	{
	}

	bool isEmpty() const
	{
		return root == nullptr;
	}

	Node<T>* & getRoot()
	{
		return root;
	}

	void add(const T& dat)
	{
		addAtNode(dat, root, nullptr, 0);
	}

	Node<T>* find(const T& data)
	{
		Node<T>* toReturn = nullptr;
		findElement(data, root, toReturn);

		return toReturn;

	}

	void printPostfix() // ABR
	{
		if (isEmpty())
		{
			cout << "No elements found \n";
		}
		else
		{
			postfixFromNode(root);
			cout << endl;
		}
	}

	void printInfix() // ARB
	{
		if (isEmpty())
		{
			cout << "No elements found \n";
		}
		else
		{
			infixFromNode(root);
			cout << endl;
		}
	}

	vector<T> getLeaves()
	{
		vector<T> toReturn;

		getLeavesFromNode(root, toReturn);

		return toReturn;
	}

	void printPrefix() const // RAB
	{
		if (isEmpty())
		{
			cout << "No elements found \n";
		}
		else
		{
			prefixFromNode(root);
			cout << endl;
		}
	}

	void deleteElem(const T& data)
	{
		Node<T>* el = find(data);

		if (el != nullptr)
		{
			if (el->left == nullptr && el->right == nullptr)
			{
				Node<T>* buffer = el->parent;


				cout << buffer->data << endl;

				if (buffer->left == el)
				{
					buffer->left = nullptr;
				}
				else if (buffer->right == el)
				{
					buffer->right = nullptr;
				}

				delete el;
			}
			else if (el->left != nullptr && el->right == nullptr)
			{
				if (el->parent->left == el)
				{
					el->parent->left = el->left;
					el->left->parent = el->parent;

					delete el;
				}
				else
				{
					el->parent->right = el->left;
					el->left->parent = el->parent;
				}
			}
			else if (el->left == nullptr && el->right != nullptr)
			{
				if (el->parent->left == el)
				{
					el->parent->left = el->right;
					el->right->parent = el->parent;
				}
				else
				{
					el->parent->right = el->right;
					el->right->parent = el->parent;
				}
			}
			else
			{
				Node<T>* maxLeft; 
				int buf;
				findMaxFromNode(el->left, maxLeft);

				buf = maxLeft->data;

				deleteElem(maxLeft->data);

				el->data = buf;
			}
		}
	}

	Node<T>* findMin()
	{
		Node<T>* toReturn = nullptr;

		if (root != nullptr)
		{
			findMinFromNode(root, toReturn);
		}

		return toReturn;
	}

	Node<T>* findMax()
	{
		Node<T>* toReturn = nullptr;

		if (root != nullptr)
		{
			findMaxFromNode(root, toReturn);
		}

		return toReturn;
	}

	void print()
	{
		if (isEmpty())
		{
			cout << "No elements found \n";
		}
		else
		{
			cout << endl;
			graphicFromNode(root);
			cout << endl;
		}
	};

	~Tree()
	{
		deleteFromNode(root);
	}
};