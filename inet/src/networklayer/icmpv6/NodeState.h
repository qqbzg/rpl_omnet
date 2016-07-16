#ifndef NODE_STATE_H
#define NODE_STATE_H

#include "IPv6Address.h"

class NodeState {
    public:
        //added Jul 29
        struct NodeInfo {
            int Rank;
            IPv6Address prfParent;
            simtime_t JoiningTime;
            //more data can be added here
        };

        typedef std::map<IPv6Address, NodeInfo> NodeCache;
        NodeCache nodeCache;

        void UpdateNodeState(int rank,IPv6Address prfparent,simtime_t time)
        {
            NodeInfo &n = nodeCache[IPv6Address()];
            n.Rank = rank;             //Rank;
            n.prfParent = prfparent;   //Parents[0].ParentId;
            n.JoiningTime = time;      //simTime();

        }
};
#endif
