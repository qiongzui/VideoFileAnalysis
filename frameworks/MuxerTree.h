//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MUXERTREE_H
#define VIDEOFILEANALYSIS_MUXERTREE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Node {
public:
    Node(const std::string&& val) : value(val) {};

    void SetParentNode(const std::shared_ptr<Node>& parent) { parentNode = parent; }
    std::shared_ptr<Node> GetParentNode();
    void AddSonNode(const std::shared_ptr<Node>& son) { sons.emplace_back(son); }
    std::vector<std::shared_ptr<Node>> GetSonNodes();
    std::string value;
private:

    std::weak_ptr<Node> parentNode;
    std::vector<std::shared_ptr<Node>> sons;
};

using NodePtr = std::shared_ptr<Node>;
class MuxerTree {
public:
    void AddNode(const NodePtr& node) { map[node->value] = node; }
private:
    NodePtr root;
    int depth;
    std::unordered_map<std::string, NodePtr> map;
};


#endif //VIDEOFILEANALYSIS_MUXERTREE_H
