#include "dictionary.h"
#include <map>

int main() {
    freopen("红黑树/data/test.in", "r", stdin);
    map<string, dict<string, string> *> dictSet;
    string opt, dictName, d1, d2;
    cout << "|==============================================|\n"
	     << "|                   使用说明                   |\n"
	     << "|      open:打开词典        create:创建词典    |\n"
	     << "|      merge:合并词典       quit:退出程序      |\n"
	     << "|                                              |\n"
	     << "|==============================================|\n\n\n";
    while (true) {
        cin >> opt;
        if (opt == "open") {
            cin >> dictName;
            if (dictSet.count(dictName))
                dictSet[dictName]->menu();
            else
                cout << "词典不存在\n";
        } else if (opt == "create") {
            cin >> dictName;
            if (dictSet.count(dictName))
                cout << "词典已存在\n";
            else {
                dictSet[dictName] = new dict<string, string>(dictName);
                cout << "词典\"" << dictName << "\"创建成功!\n";
                dictSet[dictName]->init();
            }
        } else if (opt == "merge") {
            cin >> d1 >> d2 >> dictName;
            if (dictSet.count(d1) && dictSet.count(d2)) {
                dictSet[dictName] = dict<string, string>::merge(dictSet[d1], dictSet[d2], dictName);
                cout << "词典\"" << d1 << "\"与\"" << d2 << "\"合并成功, 得到\"" << dictName << "\"\n";
                dictSet.erase(d1);
                dictSet.erase(d2);
            }
            else
                cout << "待合并的词典不存在!\n";
        } else if (opt == "quit")
            break;
        else {
            cout << "Invalid operation.\n";
            fflush(stdin);
        }
    }

    return 0;
}