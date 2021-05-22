#pragma once

#include "sort.h"
#include "../unsorted_sequence_table.h"

namespace structures
{

	/// <summary> Triedenie Quick sort. </summary>
	/// <typeparam name = "K"> Kluc prvkov v tabulke. </typepram>
	/// <typeparam name = "T"> Typ dat ukladanych v tabulke. </typepram>
	template <typename K, typename T>
	class QuickSort : public Sort<K, T>
	{
	public:
		/// <summary> Utriedi tabulku triedenim Quick sort. </summary>
		/// <param name = "table"> NonortedSequenceTable, ktoru ma utriedit. </param>
		void sort(UnsortedSequenceTable<K, T>& table, bool ascending) override;
		int partition(UnsortedSequenceTable<K, T>& table, int min, int max, bool ascending);
		void quickSort(UnsortedSequenceTable<K, T>& table, int min, int max, bool ascending);
	};

	template<typename K, typename T>
	inline int QuickSort<K, T>::partition(UnsortedSequenceTable<K, T>& table, int min, int max, bool ascending)
	{
		/*if (table.size() == 0)
		{
			break;
		}*/

		T pivot = table.getItemAtIndex(max).accessData();
		int x = min - 1;

		for (int i = min; i < max; i++)
		{
			if (ascending) {
				if (table.getItemAtIndex(i).accessData() <= pivot)
				{
					x++;
					table.swap(x, i);
				}
			}
			else
			{
				if (table.getItemAtIndex(i).accessData() > pivot)
				{
					x++;
					table.swap(x, i);
				}
			}
		}
		table.swap((x + 1), max);
		return x + 1;
	}
	
	template<typename K, typename T>
	inline void structures::QuickSort<K, T>::quickSort(UnsortedSequenceTable<K, T>& table, int min, int max, bool ascending)
	{
		/*if (table.size() == 0)
		{
			break;
		}*/

		if (min < max)
		{
			int pivot = partition(table, min, max, ascending);
			quickSort(table, min, (pivot - 1), ascending);
			quickSort(table, (pivot + 1), max, ascending);
		}
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sort(UnsortedSequenceTable<K, T>& table, bool ascending)
	{
		quickSort(table, 0, (table.size() - 1), ascending);
	}

}