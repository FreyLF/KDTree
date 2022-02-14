// KD_Tree_Template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "KDTree.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
    int points[7][2] = { {3, 6}, {17, 15}, {13, 15}, {6, 12}, {9, 1}, {2, 7}, {10, 19} };
    KDTree<int> tree(2);

	double points3D[7][3] = { {3, 6, 1}, {17, 15, 2}, {13, 15, 3}, {6, 12, 4}, {9, 1, 5}, {2, 7, 6}, {10, 19, 7} };
	KDTree<double> tree3(3);

	for (int i = 0; i < 7; i++)
	{
		tree3.Insert(points3D[i]);
	}

	double test3[] = { 6,12,4 };
	(tree3.Search(test3)) ? std::cout << "Found\n" : std::cout << "Not Found\n";
	tree3.DeleteNode(test3);
	(tree3.Search(test3)) ? std::cout << "Found\n" : std::cout << "Not Found\n";
	double target[] = { 20, 15, 3 };
	double neighbor[] = { 0,0,0 };
	tree3.NearestNeighbor(target,neighbor,3);

	std::cout << '(' << neighbor[0] << ", " << neighbor[1] << ", " << neighbor[2] << ")\n";
	tree3.DeleteTree();
	//for (int i = 0; i < 7; i++)
	//{
	//	tree.Insert(points[i]);
	//}

	//int test1[] = { 20,20 };
	//KDTree<int>::Node* neighbor = tree.NearestNeighbor(test1);
	//
	//std::cout << '(' << neighbor->Point[0] << ", " << neighbor->Point[1] << ")\n";
	//int point1[] = { 17, 15 };
	//(tree.Search(point1)) ? std::cout << "Found\n" : std::cout << "Not Found\n";
	//
	//tree.DeleteNode(point1);
	//(tree.Search(point1)) ? std::cout << "Found\n" : std::cout << "Not Found\n";
	//
	//tree.DeleteTree();
	_CrtDumpMemoryLeaks();
}

