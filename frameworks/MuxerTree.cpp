//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "MuxerTree.h"

std::shared_ptr<Node> Node::GetParentNode() {
    if (parentNode.expired()) {
        return parentNode.lock();
    }
    return nullptr;
}

std::vector<std::shared_ptr<Node>> Node::GetSonNodes() {
    return sons;
}
