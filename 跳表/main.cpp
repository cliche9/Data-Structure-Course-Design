#include "skipList.h"
#include "skipList.cpp"
#include <iostream>
using namespace std;

int main() {
	// freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/duplicate/input/input_9.txt", "r", stdin);
	// freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/duplicate/my_output/output_9.txt", "w", stdout);
	freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/generator/data/input/input_9.txt", "r", stdin);
	freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/generator/data/output/output_9.txt", "w", stdout);
	// freopen("a.in", "r", stdin);
	// freopen("a.out", "w", stdout);
	int n = 0, m = 0;
	cin >> n >> m;
	skipList<int> sList(INT_MAX, 1024);

	for (int i = 0; i < m; i++) {
		int t = 0;
		cin >> t;
		sList.insert(t);
	}
	sList.restruct();

	for (int i = 1; i < n; i++) {
		int opt = 0;
		cin >> opt;
		switch (opt) {
			case 1: {
				int target = 0;
				cin >> target;
				bool res = sList.find(target);
				int before = sList.compareResult();
				cout << "# find-before-restruct : " << target << ", 比较次数: " << before << ", result : ";
				if (res) 
					cout << "Found";
				else
					cout << "Not Found";
				cout << endl;
				// 重构后再次查找
				/*
				sList.restruct();
				res = sList.find(target);
				int after = sList.compareResult();
				cout << "# find-after-restruct : " << target << ", 比较次数: " << after << ", result : ";
				if (res) 
					cout << "Found";
				else
					cout << "Not Found";
	
				cout << ", after / before = " << setprecision(5) << (double) after / before << endl;
				*/
				break;
			}
			case 2: {
				int element = 0;
				cin >> element;
				sList.insert(element);
				int before = sList.compareResult();
				cout << "# insert-before-restruct : " << element << ", 比较次数: " << before << endl;
				// 删除+重构再次查找
				/*
				sList.erase(element);
				sList.restruct();
				sList.insert(element);
				int after = sList.compareResult();
				cout << "# insert-after-restruct : " << element << ", 比较次数: " << after
					 << ", after / before = " << setprecision(5) << (double) after / before << endl;
				*/
				break;
			}
			case 3: {
				int target = 0;
				cin >> target;
				sList.erase(target);
				int before = sList.compareResult();
				cout << "# erase-before-restruct : " << target <<", 比较次数: " << before << endl;
				// 插入+重构再次查找
				/*
				sList.insert(target);
				sList.restruct();
				sList.erase(target);
				int after = sList.compareResult();
				cout << "# erase-after-restruct : " << target << ", 比较次数: " << after
					 << ", after / before = " << setprecision(5) << (double) after / before << endl;
				*/
				break;
			}
			case 4: {
				cout << "# removeMin : " << sList.removeMin() << ", 比较次数: " << sList.compareResult() << endl;
				break;
			}
			case 5: {
				int maxElement = sList.removeMax();
				int before = sList.compareResult();
				cout << "# removeMax-before-restruct : " << maxElement << ", 比较次数: " << before << endl;
				// 插入+重构再次删除
				
				sList.insert(maxElement);
				sList.restruct();
				sList.removeMax();
				int after = sList.compareResult();
				cout << "# removeMax-after-restruct : " << maxElement << ", 比较次数: " << after
					 << ", after / before = " << setprecision(5) << (double) after / before << endl;
				
				break;
			}
			case 6: {
				cout << "# xorValue : " << sList.xorValue() << endl;
				break;
			}
		}
		cout << "# display : " << endl;
		sList.display();
		cout << endl;
	}

	return 0;
}
