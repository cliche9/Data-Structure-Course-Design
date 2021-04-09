#include "skipList.h"
#include "skipList.cpp"
#include <iostream>
using namespace std;

int main() {
	// freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/duplicate/input/input_9.txt", "r", stdin);
	// freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/duplicate/my_output/output_9.txt", "w", stdout);
	// freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/generator/data/input/input_7.txt", "r", stdin);
	// freopen("/Users/abc_mac/Documents/Code/数据结构课设/跳表/SkipListData/generator/data/output/output_7.txt", "w", stdout);
	// freopen("a.in", "r", stdin);
	// freopen("a.out", "w", stdout);
	cout << "请输入跳表的操作数目(实际操作次数比n少1), n = ";
	int n = 0, m = 0;
	cin >> n;
	cout << "请输入跳表的初始元素数量m = ";
	cin >> m;
	skipList<int> sList(INT_MAX);
	cout << "请输入" << m << "个数(以空格分隔)，这些数将被用来初始化跳表." << endl;

	for (int i = 0; i < m; i++) {
		int t = 0;
		cin >> t;
		sList.insert(t);
	}
	cout << "初始化后的跳表如下:" << endl;
	sList.display();
	sList.restruct();
	cout << "重构后的跳表如下:" << endl;
	sList.display();

	/* 
	自生成数据，每组操作120次（每个操作20次），共n / 120组
	n /= 120;
	int opt_count[7][n];
	memset(opt_count, 0, sizeof(opt_count));
	*/

	for (int i = 1; i < n; i++) {
		/*
		每过一组重构一次，比较两者效率差别
		if (i & 1)
			sList.restruct();
		for (int j = 0; j < 120; j++) {
		*/
		cout << "==================================================================================" << endl;
		cout << "- `1 num`，查找跳表中是否含有元素 `num`，含有则输出 `YES`，否则输入 `NO`" << endl
			<< "- `2 num`，向跳表中插入元素 `num`" << endl
			<< "- `3 num`，将跳表中的元素 `num` 删除" << endl
			<< "- `4`，删除跳表中的最小元素，并将该元素输出" << endl
			<< "- `5`，删除跳表中的最大元素，并将该元素输出" << endl
			<< "- `6`，输出跳表中所有元素的亦或" << endl
			<< "请输入你的操作:  ";
			int opt = 0;
			cin >> opt;
			switch (opt) {
				case 1: {
					int target = 0;
					cin >> target;
					bool res = sList.find(target);
					int before = sList.compareResult();
					cout << "# find : " /*-before-restruct : "*/ << target << ", 比较次数: " << before << ", result : ";
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
					cout << "# insert : " /*-before-restruct : "*/ << element << ", 比较次数: " << before << endl;
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
					cout << "# erase : " /*-before-restruct : "*/ << target <<", 比较次数: " << before << endl;
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
					cout << "# removeMax : " /*-before-restruct : "*/ << maxElement << ", 比较次数: " << before << endl;
					// 插入+重构再次删除
					/*
					sList.insert(maxElement);
					sList.restruct();
					sList.removeMax();
					int after = sList.compareResult();
					cout << "# removeMax-after-restruct : " << maxElement << ", 比较次数: " << after
						<< ", after / before = " << setprecision(5) << (double) after / before << endl;
					*/
					break;
				}
				case 6: {
					cout << "# xorValue : " << sList.xorValue() << endl;
					break;
				}
			}
			// opt_count[opt][i] += sList.compareResult();
			cout << "# display : " << endl;
			sList.display();
		/*
		}
		for (int k = 1; k <= 6; k++)
			opt_count[k][i] /= 20;
		cout << "第" << i << "轮比较平均次数:" << endl
			 << "find: " << opt_count[1][i] << endl
			 << "insert: " << opt_count[2][i] << endl
			 << "erase: " << opt_count[3][i] << endl
			 << "removeMax: " << opt_count[5][i] << endl
			 << endl;
		if (i & 1)
			cout << "比较次数: after-restruct / before-restruct:" << endl
			 	 << "find: " << setprecision(5) << (float)opt_count[1][i] / (float)opt_count[1][i - 1] << endl
				 << "insert: " << setprecision(5) << (float)opt_count[2][i] / (float)opt_count[2][i - 1] << endl
				 << "erase: " << setprecision(5) << (float)opt_count[3][i] / (float)opt_count[3][i - 1] << endl
				 << "removeMax: " << setprecision(5) << (float)opt_count[5][i] / (float)opt_count[5][i - 1] << endl
				 << endl;
		*/
	}

	return 0;
}
