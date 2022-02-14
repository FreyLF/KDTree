#pragma once

template<class T> 
class KDTree
{
public:
	struct Node
	{
		T* Point;
		Node* Left;
		Node* Right;

		Node() 
		{
			Point = nullptr;
			Left = nullptr;
			Right = nullptr;
		}
		~Node()
		{
			delete[] Point;
		}

		T& operator[](unsigned int index)
		{
			return Point[index];
		}

		// No overload= due to lack of K.
	};

private:
	// Number of arrayCapacity.
	unsigned int K;
	Node* Root;

public:
	KDTree(unsigned int dimensions) { K = dimensions; Root = nullptr;}
	~KDTree() { DeleteTree(); }

	// Insert a point in the tree.
	void Insert(T* point)
	{
		InsertRecursion(Root, point);
	}

	/* Confirm that given point exists in the tree.
	 * Return true if point exists, otherwise false.
	*/
	bool Search(T* point)
	{
		if (!Root)
		{
			return false;
		}
		return SearchRecursion(Root, point);
	}

	// Delete the whole tree.
	void DeleteTree()
	{
		DeleteTreeRecursion(Root);
		Root = nullptr;
	}

	// Delete node with given value.
	void DeleteNode(T* point)
	{
		if (Root)
		{
			DeleteNodeRecursion(Root, point);
		}
	}

	/* Finds the nearest neighbor to the target.
	 * target: Find nearest neighbor to this target.
	 * result: A container to store the result.
	 * arrayCapacity: Capacity of the result. Used to make sure the container can support the result.
	 * This is a roundabout way of doing, but it helps keeping the tree safe.
	*/
	void NearestNeighbor(T* target, T result[], unsigned int arrayCapacity)
	{
		if (!Root)
		{
			return;
		}

		// The parameter arrayCapacity is not needed, it is just a remainder for the user.
		if (arrayCapacity != K)
		{
			return;
		}

		T* temp = NearestNeighborRecursion(Root, target)->Point;

		for (unsigned int i = 0; i < arrayCapacity; ++i)
		{
			result[i] = temp[i];
		}
	}

	/* Return root of the tree.
	 * Return value is const to protect the tree, user will need to store the value.
	*/
	const Node* GetRoot() { return Root; }

	private:

	// ======================================================
	// ================== Helpers ===========================
	// ======================================================

	// Returns the distance between two points.
	double NodeDistance(T* target, T* node)
	{
		double sum = 0;
		/* Go through all arrayCapacity and do a distance formula.
		 * Distance = sqrt((x1 - x2)^2 + (y1 - y2)^2 ...)
		 * Might need to cut some corners for speed later.
		*/ 
		for (unsigned int i = 0; i < K; ++i)
		{
			double s = (double)(target[i] - node[i]);
			s *= s;
			sum += s;
		}
		return sqrtl(sum);
	}

	/* Returns closest node to a target.
	 * target: Target that the nodes will use to measure distance.
	 * A: First node to compare.
	 * B: Second node to compare.
	*/
	Node* ClosestNode(T* target, Node* A, Node* B)
	{
		Node* closest = nullptr;
		if (A) // is A valid?
		{
			closest = A;
			if (B) // is B valid?
			{
				// Is B closest to the target?
				if (NodeDistance(target, B->Point) < NodeDistance(target, A->Point))
				{
					closest = B;
				}
			}
		}
		else if (B) // is B valid?
		{
			closest = B;
		}		
		return closest;
	}

	// Copy data from A to B.
	void CopyAtoB(T* A, T* B)
	{
		for (unsigned int i = 0; i < K; ++i)
		{
			B[i] = A[i];
		}
	}

	/* If the points are the same return true.
	 * A: First point to compare.
	 * B: Second point to compare.
	*/
	bool ComparePoints(T* A, T* B)
	{
		for (unsigned int i = 0; i < K; ++i)
		{
			if (A[i] != B[i])
			{
				return false;
			}
		}
		return true;
	}

	/* Create a new node.
	 * point: An array of values to fill the node.
	 * Returns a new node.
	*/
	Node* CreateNode(T point[])
	{
		Node* temp = new Node();
		temp->Point = new T[K];

		// Fill every dimension of the k dimensional point.
		for (unsigned int i = 0; i < K; ++i)
		{
			temp->Point[i] = point[i];
		}
		temp->Left = nullptr;
		temp->Right = nullptr;

		return temp;
	}

	/* Find the minimum value of a dimension between nodes.
	 * node: Node to start the minimum search from.
	 * dimension: Dimension to compare.
	 * depth: How deep in the tree the recursion is. Used to decide wich axis of the point is being compared.
	 * Returns node with the minimum value.
	*/
	Node* FindMin(Node* node, int dimension, int depth)
	{
		if (!node)
		{
			return nullptr;
		}

		unsigned int curDimesion = depth % K;

		// If current dimension is the same dimension, then minimun is to the left.
		if (curDimesion = dimension)
		{
			// If left is null, just return the value of the point at this dimension.
			if (!node->Left)
			{
				return node;
			}
			// Compare point to left child.
			return Min(node, FindMin(node->Left, dimension, depth + 1), dimension);
		}

		// If current dimension is different then minimum can be anywhere.
		// Comapre point to min from left and right.
		Node* min = Min(node, FindMin(node->Left, dimension, depth + 1), dimension);
		return Min(min, FindMin(node->Right, dimension, depth + 1), dimension);
	}

	/* Return the node with minimum value type between two nodes.
	 * A: First node to compare.
	 * B: Second node to compare.
	*/
	Node* Min(Node* A, Node* B, unsigned int dimension)
	{
		Node* min = nullptr;
		
		if (A) // Check if A is valid.
		{
			min = A;
			// Check if B is valid.
			if (B)
			{
				// Compare A and B.
				if (A->Point[dimension] > B->Point[dimension])
				{
					min = B;
				}
			}
		}
		else if (B) // In case A was not valid, check B.
		{
			min = B;
		}
		return min;
	}

	// ======================================================
	// ================== Recursions ========================
	// ======================================================

	/* Actual code for DeleteTree().
	 * node: Node to look at, initially the root of the tree.
	 * Goes through the whole tree and delete each node.
	*/
	void DeleteTreeRecursion(Node* node)
	{
		if (node)
		{
			DeleteTreeRecursion(node->Left);
			DeleteTreeRecursion(node->Right);
			delete node;
			node = nullptr;
		}
	}

	/* Actual code for Search().
	 * node: Node to look at, initially the root of the tree.
	 * point: Point to be searched for.
	 * depth: How deep in the tree the recursion is. Used to decide wich axis of the point is being compared.
	 * Returns true if point exists in the list, false otherwise.
	*/
	bool SearchRecursion(Node* node, T* point, unsigned int depth = 0)
	{
		// If node is null, the point does not exist.
		if (!node)
		{
			return false;
		}

		// Check if both points are the same.
		if (ComparePoints(node->Point, point))
		{
			return true;
		}

		unsigned int curDimension = depth % K;

		if (point[curDimension] < node->Point[curDimension])
		{
			return SearchRecursion(node->Left, point, depth + 1);
		}

		return SearchRecursion(node->Right, point, depth + 1);
	}

	/* Actual code for DeleteNode().
	 * node: Node to look at, initially the root of the tree.
	 * point: Point to be deleted, if there are two equal points, only one will be deleted.
	 * depth: How deep in the tree the recursion is. Used to decide wich axis of the point is being compared.
	 * Return is used internaly to find node to delete, this function will always return nullptr in the end.
	*/
	Node* DeleteNodeRecursion(Node* node, T* point, unsigned int depth = 0)
	{
		if (!node)
		{
			return nullptr;
		}

		unsigned int curDimension = depth % K;

		// Is this the point to delete?
		if (ComparePoints(node->Point, point))
		{
			if (node->Right) // Has right child.
			{
				// Find minimum of current node’s dimension in right subtree.
				Node* min = FindMin(node->Right, curDimension, depth);

				// Replace the node with above found minimumand recursively delete minimum in right subtree.
				CopyAtoB(min->Point, node->Point);

				// Delete the minimum.
				node->Right = DeleteNodeRecursion(node->Right, min->Point, depth + 1);
			}
			else if (node->Left) // Has left child
			{
				// Find minimum of current node’s dimension in left subtree.
				Node* min = FindMin(node->Left, curDimension, depth);

				// Replace the node with above found minimum and recursively delete minimum in left subtree.
				CopyAtoB(min->Point, node->Point);

				// Make new left subtree as right child of current node.
				node->Left = DeleteNodeRecursion(node->Left, min->Point, depth);
			}
			else // If this is a leaf node, simply delete it
			{
				delete node;
				node = nullptr;
				return nullptr;
			}
			return node;
		}
		// Keep searching.
		if (point[curDimension] < node->Point[curDimension])
		{
			node->Left = DeleteNodeRecursion(node->Left, point, depth + 1);
		}
		else
		{
			node->Right = DeleteNodeRecursion(node->Right, point, depth + 1);
		}
		return node;
	}

	/* Actual code for Insert().
	 * node: Node to look at, initially the root of the tree.
	 * point: Point to insert in the tree.
	 * depth: How deep in the tree the recursion is. Used to decide wich axis of the point is being compared.
	 * Returns the node of the tree.
	 */
	Node* InsertRecursion(Node* node, T* point, unsigned int depth = 0)
	{
		// If node is null, fill it.
		if (!node)
		{
			node = CreateNode(point);
			if (!Root)
			{
				Root = node;
			}
			return node;
		}

		unsigned int curDimention = depth % K;

		/* Compare the point to be inserted with the current node at the current dimension. 
		 * Decide wether to go left or right on the subtree.
		*/
		if (point[curDimention] < node->Point[curDimention])
		{
			node->Left = InsertRecursion(node->Left, point, depth + 1);
		}
		else
		{
			node->Right = InsertRecursion(node->Right, point, depth + 1);
		}

		return node;
	}

	/* Actual code for NearestNeighbor().
	 * node: Node to look at, initially the root of the tree.
	 * target: Point to look for nearest.
	 * depth: How deep in the tree the recursion is. Used to decide wich axis of the point is being compared.
	 * Returns the nearest node to the target.
	*/
	Node* NearestNeighborRecursion(Node* node, T* target, unsigned int depth = 0)
	{
		if (!node)
		{
			return nullptr;
		}

		unsigned int curDimention = depth % K;
		Node* nextBranch = nullptr;
		Node* otherBranch = nullptr;

		// If the target is smaller, go to the left first.
		if (target[curDimention] < node->Point[curDimention])
		{
			nextBranch = node->Left;
			otherBranch = node->Right;
		}
		else
		{
			nextBranch = node->Right;
			otherBranch = node->Left;
		}

		// Find the closest node one the lower branches.
		Node* temp = NearestNeighborRecursion(nextBranch, target, depth + 1);

		// Compare nodes to see which one is the closest.
		Node* closest = ClosestNode(target, temp, node);

		// Current best distance found.
		double bestDistance = NodeDistance(target, closest->Point);

		// There might be something good on the "bad" side of the tree.
		double possibleBest = (double)(target[curDimention]) - (double)(node->Point[curDimention]);

		/* If the best distance is bigger than a straing line on one dimension, 
		 * there might be a better option on the other branch.*/
		if (bestDistance >= abs(possibleBest))
		{
			temp = NearestNeighborRecursion(otherBranch, target, depth + 1);
			closest = ClosestNode(target, temp, closest);
		}
		return closest;
	}
};