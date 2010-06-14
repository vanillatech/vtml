#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>

typedef std::vector<int> TTreeItemIndex;
typedef std::set<TTreeItemIndex> THandleSet;
typedef std::map<std::string, THandleSet> TTreeSync;

