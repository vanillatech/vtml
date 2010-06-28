#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>

typedef std::vector<int> TTreeItemIndex;
typedef std::set<TTreeItemIndex> THandleSet;
typedef std::map<std::string, THandleSet> TTreeSync;

typedef std::vector<std::pair<int, std::string> > TDebugLogLayer;
typedef std::vector<TDebugLogLayer> TDebugLog;
