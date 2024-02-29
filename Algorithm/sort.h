#pragma once

namespace MA {

	// ¿ìËÙÅÅĞò
	void quickSort(int arr[], int length);
	void qsort(int arr[], int left, int right);
	int partition(int arr[], int left, int right);

	// ¹é²¢ÅÅĞò
	void mergeSort(int src[], int tmpSrc[], int start, int end);

	// Í°ÅÅĞò
	void heapSort(int arr[], int length);
	void maxHeapify(int arr[], int start, int end);

}