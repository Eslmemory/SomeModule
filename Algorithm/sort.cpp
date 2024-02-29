#include "sort.h"
#include <iostream>

namespace MA {

	// -------quick sort-------
	void quickSort(int arr[], int length)
	{
		qsort(arr, 0, length - 1);
	}

	void qsort(int arr[], int left, int right)
	{
		if (left >= right)
			return;

		int pivot = partition(arr, left, right);
		qsort(arr, left, pivot - 1);
		qsort(arr, pivot + 1, right);
	}

	int partition(int arr[], int left, int right)
	{
		int pivotValue = arr[left];
		int leftBegin = left;
		while (left < right) {
			while (arr[right] >= pivotValue && left < right) right--;
			while (arr[left] <= pivotValue && left < right) left++;

			if (left < right) {
				int tmp = arr[left];
				arr[left] = arr[right];
				arr[right] = tmp;
			}
		}

		int tmp = arr[left];
		arr[left] = pivotValue;
		arr[leftBegin] = tmp;

		return left;
	}

	// -------heap sort-------
	void heapSort(int arr[], int length)
	{
		for (int i = length / 2 - 1; i >= 0; i--)
			maxHeapify(arr, i, length - 1);

		for (int i = length - 1; i > 0; i--)
		{
			std::swap(arr[0], arr[i]);
			maxHeapify(arr, 0, i - 1);
		}
	}

	void maxHeapify(int arr[], int start, int end)
	{
		int father = start;
		int child = father * 2 + 1;
		while (child <= end)
		{
			if (child + 1 <= end)
				child = arr[child] > arr[child + 1] ? child : child + 1;
			if (arr[child] <= arr[father])
				return;
			else
			{
				std::swap(arr[child], arr[father]);
				father = child;
				child = father * 2 + 1;
			}
		}
	}

	// -------merge sort-------
	void mergeSort(int src[], int tmpSrc[], int start, int end) {
		if (start >= end)
			return;

		int mid = start + ((end - start) >> 1);
		int start1 = start, end1 = mid;
		int start2 = mid + 1, end2 = end;

		mergeSort(src, tmpSrc, start1, end1);
		mergeSort(src, tmpSrc, start2, end2);

		int i = start1, j = start2, k = start;
		while (i <= end1 && j <= end2)
			tmpSrc[k++] = src[i] < src[j] ? src[i++] : src[j++];
		while (i <= end1)
			tmpSrc[k++] = src[i++];
		while (j <= end2)
			tmpSrc[k++] = src[j++];

		for (int m = start; m <= end; m++) {
			src[m] = tmpSrc[m];
		}
	}

}
