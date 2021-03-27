#include <iostream>
#include <vector>
#include <random>
#include <thread>

using namespace std;


// Merge given sections of the vector object passed
void MergeSection(vector<int>& array_in, int left, int middle, int right)
{
	vector<int> left_sub, right_sub;

	for (int i = 0; i < middle + 1 - left; i++)
	{
		left_sub.push_back(array_in[left + i]);
	}

	for (int i = 0; i < right - middle; i++)
	{
		right_sub.push_back(array_in[middle + 1 + i]);
	}

	int l = 0;
	int r = 0;
	for (int i = left; i < right + 1; i++)
	{
		// Check counters are within bounds
		if (l < left_sub.size() && r < right_sub.size())
		{
			if (left_sub[l] < right_sub[r])
			{
				array_in[i] = left_sub[l];
				l++;
			}
			else
			{
				array_in[i] = right_sub[r];
				r++;
			}
		}
		else
		{
			//one side fully inserted, now insert other side
			vector<int>* v;
			int* c;

			if (l < left_sub.size())
			{
				v = &left_sub;
				c = &l;
			}
			else
			{
				v = &right_sub;
				c = &r;
			}

			while (i < right + 1)
			{
				array_in[i] = v->operator[](*c);
				(*c)++;
				i++;
			}

			/*
			if (l < left_sub.size())
			{
				while (i < right + 1) {
					array_in[i] = left_sub[l];
					l++;
					i++;
				}
				
			}
			else
			{
				while (i < right + 1) {
					array_in[i] = right_sub[r];
					r++;
					i++;
				}
			}
			*/

		}
	}
}

void MergeSort(vector<int>& array_in, int left, int right)
{	
	
	if (left < right)
	{
		//int middle = left + (right - 1) / 2;

		int middle = (left + right) / 2;
		MergeSort(array_in, left, middle);
		MergeSort(array_in, middle + 1, right);
		MergeSection(array_in, left, middle, right);
	}
}

void MergeSortThreaded(vector<int>& array_in, int left, int right)
{
	if (left < right)
	{
		//int middle = left + (right - 1) / 2;

		int middle = (left + right) / 2;
		MergeSort(array_in, left, middle);

		// Sort other half on new thread
		cout << "New thread\n";
		thread t1(MergeSort, ref(array_in), middle + 1, right);
		//MergeSort(array_in, middle + 1, right);
		t1.join();

		MergeSection(array_in, left, middle, right);
	}
}


int main()
{
	//Driver code

	//Create and fill vector with random numbers
	vector<int> v;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, 1000);

	for (int i = 0; i < 100; i++)
	{
		v.push_back(distrib(gen));
	}

	cout << "Unsorted: \n";

	for (auto i : v)
	{
		cout << i << ", ";
	}


	// Call merge sort
	MergeSortThreaded(v, 0, v.size() - 1);

	cout << "\nSorted: \n";

	for (auto i : v)
	{
		cout << i << ", ";
	}

	cout << endl;

	return 0;
}