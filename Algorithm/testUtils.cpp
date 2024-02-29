#include <chrono>
#include <iostream>
#include "sort.h"
#include "leetcode.h"
#include "testUtils.h"

namespace Test {

    void testSort() {
        int arr[] = { 2, 3, 38, 5, 15, 36, 27, 4, 39, 7, 9, 12, 81, 4, 3 };

        auto start = std::chrono::high_resolution_clock::now();
        // MA::quickSort(arr, sizeof(arr) / sizeof(int));
        MA::heapSort(arr, sizeof(arr) / sizeof(int));
        auto end = std::chrono::high_resolution_clock::now();
        auto period = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
        std::cout << period << std::endl;

        for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
        {
            std::cout << arr[i] << " ";
        }
    }

    void problem2() {
        int arr[8] = { 2, 3, 5, 4, 3, 2, 6, 7 };
        int result = MA::getDuplication(arr, sizeof(arr) / sizeof(arr[0]));
        printf("result is %d", result);
    }

    void problem7() {
        int preorder[8] = { 1, 2, 4, 7, 3, 5, 6, 8 };
        int inorder[8] = { 4, 7, 2, 1, 5, 3, 8, 6 };
        MA::ReConstructTree(preorder, inorder, 8);
    }

    void problem9() {
        MA::queue<int> q;
        q.appendTail(10);
        q.appendTail(9);
        q.appendTail(8);
        q.appendTail(7);
        q.deleteHead();
        q.deleteHead();
        q.appendTail(11);
        q.appendTail(12);
        q.deleteHead();
        q.appendTail(13);
        q.printQueue();
    }

    void problem11() {
        int arr[5] = { 1, 1, 1, 0, 1 };
        int result = MA::minRotateArray(arr, sizeof(arr) / sizeof(arr[0]));
        printf("result is %d", result);
    }

    void problem12() {
        char matrix[] = { 'a', 'b', 't', 'g', 'c', 'f', 'c', 's', 'j', 'd', 'e', 'h' };
        char str[] = "bfce";
        int row = 3, col = 4;
        bool result = MA::hasPath(matrix, row, col, str);
        printf("result is %d", result);
    }

    void leetcode256() {
        std::vector<std::vector<int>> costs;
        costs.emplace_back(std::initializer_list<int>({ 17, 2, 17 }));
        costs.emplace_back(std::initializer_list<int>({ 16, 16, 5 }));
        costs.emplace_back(std::initializer_list<int>({ 14, 3, 19 }));
        int cost = MA::paintHouse(costs);
        printf("cost is %d", cost);
    }

}
