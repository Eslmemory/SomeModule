#pragma once

namespace MA {

	// ��������
	void quickSort(int arr[], int length);
	void qsort(int arr[], int left, int right);
	int partition(int arr[], int left, int right);

	// �鲢����
	void mergeSort(int src[], int tmpSrc[], int start, int end);

	// Ͱ����
	void heapSort(int arr[], int length);
	void maxHeapify(int arr[], int start, int end);

}