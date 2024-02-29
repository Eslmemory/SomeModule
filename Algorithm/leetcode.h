#pragma once
#include <exception>
#include <stack>
#include <vector>

namespace MA {

	//-----------面试题2:不修改数组找出重复的数字-----------
	bool countRange(const int arr[], int length, int start, int end) {
		int count = 0;
		for (int i = 0; i < length - 1; i++) {
			if (arr[i] <= end && arr[i] >= start) {
				count++;
			}
		}

		if (count > end - start + 1)
			return true;
		return false;
	}

	int getDuplication(const int arr[], int length) {
		if (arr == nullptr || length <= 0)
			return -1;

		int start = 1, end = length - 1;
		while (start < end) {
			int mid = (start + end) / 2;
			if (countRange(arr, length, start, mid)) {
				end = mid;
			}
			else {
				start = mid + 1;
			}
		}
		return start;
	}

	//-----------面试题7:重建二叉树, 给定前序遍历和中序遍历, 重建该二叉树-----------
	
	struct TreeNode {
		int value;
		TreeNode* leftNode;
		TreeNode* rightNode;
	};

	TreeNode* ReConstructCore(int* preorderStart, int* preorderEnd, int* inorderStart, int* inorderEnd) {
		int rootValue = preorderStart[0];
		TreeNode* root = new TreeNode();
		root->value = rootValue;
		root->leftNode = nullptr;
		root->rightNode = nullptr;

		if (preorderStart == preorderEnd) {
			if (inorderStart == inorderEnd && *inorderStart == *preorderStart)
				return root;
			else
				throw std::exception("Invalid Input!!");
		}

		int* rootInorder = inorderStart;
		while (rootInorder <= inorderEnd && *rootInorder != rootValue)
			rootInorder++;
		if (rootInorder > inorderEnd) {
			throw std::exception("Invalid Input!");
		}

		int leftLength = rootInorder - inorderStart;
		int* newPreorderEnd = preorderStart + leftLength;

		if (leftLength > 0) {
			root->leftNode = ReConstructCore(preorderStart + 1, newPreorderEnd, inorderStart, rootInorder - 1);
		}

		if (leftLength < preorderEnd - preorderStart) {
			root->rightNode = ReConstructCore(newPreorderEnd + 1, preorderEnd, rootInorder + 1, inorderEnd);
		}
		return root;
	}

	TreeNode* ReConstructTree(int* preorderStart, int* inorderStart, int length) {
		if (preorderStart == nullptr || inorderStart == nullptr || length <= 0)
			return nullptr;
		return ReConstructCore(preorderStart, preorderStart + length - 1,
			inorderStart, inorderStart + length - 1);
	}

	//-----------面试题9:用两个栈实现队列-----------
	template<typename T>
	class queue {
	public:
		queue() {}
		~queue() {}

		void printQueue() {

			while (!t2.empty()) {
				printf("%d\n", t2.top());
				t2.pop();
			}

			while (!t1.empty()) {
				t2.push(t1.top());
				t1.pop();
			}

			while (!t2.empty()) {
				printf("%d\n", t2.top());
				t2.pop();
			}
		}

		void appendTail(const T& node) {
			t1.push(node);
		}

		T deleteHead() {
			if (t2.empty()) {
				while (!t1.empty()) {
					t2.push(t1.top());
					t1.pop();
				}
			}
			
			if (t2.empty())
				throw std::exception("queue is empty");
			T deleteValue = t2.top();
			t2.pop();
			return deleteValue;
		}

	private:
		std::stack<T> t1;
		std::stack<T> t2;
	};

	int orderSearch(int arr[], int leftIndex, int rightIndex) {
		int result = arr[leftIndex];
		for (int i = leftIndex; i < rightIndex; i++) {
			if (arr[i] < result)
				result = arr[i];
		}
		return result;
	}

	//-----------面试题11:旋转数组的最小数字-----------
	int minRotateArray(int arr[], int length) {
		int leftIndex = 0, rightIndex = length - 1;
		while (true) {
			int mid = (leftIndex + rightIndex) / 2;
			if (arr[mid] == arr[leftIndex] && arr[mid] == arr[rightIndex]) {
				return orderSearch(arr, leftIndex, rightIndex);
			}

			if (arr[mid] >= arr[leftIndex]) {
				leftIndex = mid;
			}
			if (arr[mid] <= arr[rightIndex]) {
				rightIndex = mid;
			}
			if (leftIndex + 1 == rightIndex) {
				return arr[rightIndex];
			}
		}

		return -1;
	}

	//-----------面试题12:矩阵中的路径-----------
	bool hasPathCore(char* matrix, bool* visited, char* str, int strLength, 
					 int row, int col, int rowCur, int colCur) {
		if (str[strLength] == '\0')
			return true;

		bool hasPath = false;
		if (rowCur < row && rowCur >= 0 && colCur < col && colCur >= 0 && 
			!visited[rowCur * col + colCur] && str[strLength] == matrix[rowCur * col + colCur]) {
			visited[rowCur * col + colCur] = true;
			hasPath = hasPathCore(matrix, visited, str, strLength + 1, row, col, rowCur + 1, colCur) ||
						hasPathCore(matrix, visited, str, strLength + 1, row, col, rowCur - 1, colCur) ||
						hasPathCore(matrix, visited, str, strLength + 1, row, col, rowCur, colCur + 1) ||
						hasPathCore(matrix, visited, str, strLength + 1, row, col, rowCur, colCur - 1);
			if (!hasPath)
				visited[rowCur * col + colCur] = false;
		}

		return hasPath;
	}
	
	bool hasPath(char* matrix, int row, int col, char* str) {
		if (matrix == nullptr || str == nullptr)
			return false;

		int rowIndex = 0, colIndex = 0;
		bool* visited = new bool[row * col];
		if (visited == nullptr) {
			throw std::exception("new failed");
		}
		memset(visited, false, row * col);

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (hasPathCore(matrix, visited, str, 0, row, col, i, j))
					return true;
			}
		}

		delete[] visited;
		return false;
	}

	//-----------面试题13:机器人的运动范围-----------
	int computeDigit(int num) {
		int result = 0;
		while (num != 0) {
			result += num % 10;
			num = num / 10;
		}
		return result;
	}

	int movingCountCore(bool* visited, int row, int col, int k, int rowCur, int colCur) {
		int count = 0;
		if (computeDigit(rowCur) + computeDigit(colCur) > k)
			return count;

		if (rowCur >= 0 && rowCur < row && colCur >= 0 && colCur < col && !visited[rowCur * col + colCur]) {
			visited[rowCur * col + colCur] = true;
			count = 1 + movingCountCore(visited, row, col, k, rowCur + 1, colCur)
				+ movingCountCore(visited, row, col, k, rowCur - 1, colCur)
				+ movingCountCore(visited, row, col, k, rowCur, colCur + 1)
				+ movingCountCore(visited, row, col, k, rowCur, colCur - 1);
		}
		return count;
	}

	int movingCount(int row, int col, int k = 0) {
		if (row <= 0 || col <= 0)
			return 0;

		bool* visited = new bool[row * col];
		if (visited == nullptr)
			throw std::exception("new failed");
		memset(visited, false, row * col);

		int moveCount = 0;
		movingCountCore(visited, row, col, k, 0, 0);
		
		delete[] visited;
		return moveCount;
	}

	//-----------leetcode 256 粉刷房子-----------
	int paintHouse(std::vector<std::vector<int>> costs) {
		int n = costs.size();
		std::vector<std::vector<int>> f(n, std::vector<int>(3, 0));
		
		for (int i = 0; i < 3; i++)
			f[0][i] = costs[0][i];

		for (int i = 1; i < n; i++) {
			for (int j = 0; j < 3; j++) {
				if (j == 0)
					f[i][j] = std::min(f[i - 1][1], f[i - 1][2]) + costs[i][j];
				else if(j == 1)
					f[i][j] = std::min(f[i - 1][0], f[i - 1][2]) + costs[i][j];
				else
					f[i][j] = std::min(f[i - 1][0], f[i - 1][1]) + costs[i][j];
			}
		}

		return std::min(std::min(f[n - 1][0], f[n - 1][1]), f[n - 1][2]);
	}
}


