#include <algorithm>
#include <vector>

#include "opp_utils.h"
#include "ObjectiveFun4.h"

int ObjectiveFun4::IsParent(const IPv6Address& srcAddr,double rank)
{
    ParentStructure* rcv = new ParentStructure();
    rcv->ParentId = srcAddr;
    rcv->ParentRank = rank;
    std::vector<ParentStructure*>::iterator it;
    EV<<"judge 0.id"<<countercache[0]->ParentId<<" "<<countercache[0]->ParentRank<<endl;
    for(it = countercache.begin(); it != countercache.end(); it++)
    {
        if((*it)->ParentId == rcv->ParentId)
        {
             if((*it)->ParentRank == rcv->ParentRank)
                 return(EXIST);
             else
                 return(SHOULD_BE_UPDATED);
        }
    }
    return(NOT_EXIST);
}

double ObjectiveFun4::UpdateParent(double counter, int srID, const IPv6Address& srcAddr,int rank, const MACAddress& srcMACAddr)
{
    ParentStructure* rcv = new ParentStructure();
    rcv->srnode = srID;
    rcv->Addcounter = counter;
    rcv->ParentId = srcAddr;
    rcv->ParentMACAddr = srcMACAddr;
    rcv->ParentRank = rank;
    EV<<"rcvd src mac addr:"<<srcMACAddr<<endl;
    EV<<"rcvd src parent ipv6 addr:"<<rcv->ParentId<<endl;
    std::vector<ParentStructure*>::iterator it;
    for(it = countercache.begin(); it != countercache.end(); it++)
    {
        if((*it)->srnode == rcv->srnode)
        {
            (*it)->Addcounter = rcv->Addcounter;
            (*it)->ParentRank = rcv->ParentRank;
            (*it)->ParentMACAddr = rcv->ParentMACAddr;
            (*it)->ParentId = rcv->ParentId;
            break;
        }
    }
    if(it == countercache.end())
    {
        countercache.push_back(rcv);
        EV<<"first time be added in coutercache:"<<rcv->ParentId<<endl;
    }
    for(it = countercache.begin(); it != countercache.end(); it++)
        {
            EV<<(*it)->ParentId<<" ";
        }
    EV<<endl;
    std::stable_sort(countercache.begin(), countercache.end(), counterLessThan);
    EV<<countercache[0]->ParentId<<" "<<countercache[0]->ParentMACAddr<<endl;
    prfparent = countercache[0]->ParentId;
    prfMACparent = countercache[0]->ParentMACAddr;
    EV<<"prf parent id:"<<prfparent<<endl;
    EV<<"prf parent addr:"<<prfMACparent<<endl;

    Rank = countercache[0]->ParentRank + 1;
    PRNodeID = countercache[0]->srnode;
    metrics = countercache[0]->Addcounter;
    return countercache[0]->Addcounter;

}

void ObjectiveFun4::test()
{
    std::vector<ParentStructure*>::iterator it;
for(it = countercache.begin(); it != countercache.end(); it++)
        {
            EV<<(*it)->ParentId<<" ";
        }
    EV<<endl;
}
double ObjectiveFun4::counterCal(int srID, double prev_count_perhop, int NumDio, int hop_count)
{
    //--begin--handling with counter cache, added 25th April
    ParentStructure* rcv = new ParentStructure();
    rcv->srnode = srID;
    rcv->Rcvdcounter = 1;
    std::vector<ParentStructure*>::iterator it;
    EV<<"precount:"<<prev_count_perhop<<endl;
    EV<<"Numdio:"<<NumDio<<endl;
    for(it = countercache.begin(); it != countercache.end(); it++)
    {
        if(rcv->srnode == (*it)->srnode)
        {
            double temp = ++((*it)->Rcvdcounter);
            EV<<"Existed update to:"<<temp<<endl;
            (*it)->Addcounter = ((NumDio/temp) + prev_count_perhop*hop_count)/(hop_count+1);//(temp/NumDio) * precount;
            return ((NumDio/temp) + prev_count_perhop*hop_count)/(hop_count+1);//(temp/NumDio) * precount;
        }
    }
    if (it == countercache.end()) {
            countercache.push_back(rcv);
            EV<<"First Time rcving:"<<rcv->Rcvdcounter<<endl;
        }
    rcv->Addcounter = (NumDio/rcv->Rcvdcounter + prev_count_perhop*hop_count)/(hop_count+1);//rcv->Rcvdcounter/NumDio * precount;
    return (NumDio/rcv->Rcvdcounter + prev_count_perhop*hop_count)/(hop_count+1);//(rcv->Rcvdcounter/NumDio) * precount;
        //--end--handling with counter cache, added 25th April
}

//Added May, review for this part of code
int ObjectiveFun4::counterSetting(int srID)
{
    //--begin--handling with counter cache, added 25th April
    ParentStructure* rcv = new ParentStructure();
    rcv->srnode = srID;
    rcv->Rcvdcounter = 1;
    std::vector<ParentStructure*>::iterator it;
    for(it = countercache.begin(); it != countercache.end(); it++)
    {
        if(rcv->srnode == (*it)->srnode)
        {
            (*it)->Rcvdcounter++;
            break;
        }
    }
    if (it == countercache.end()) {
            countercache.push_back(rcv);
    }
    std::stable_sort(countercache.begin(), countercache.end(), counterLessThan);
    return countercache[0]->srnode;
        //--end--handling with counter cache, added 25th April
}

//Added May, for handling by processDIO
bool ObjectiveFun4::counterLessThan(const ParentStructure *a, const ParentStructure *b)
{
    if(a->Addcounter != b->Addcounter)
        {
            return a->Addcounter < b->Addcounter;
        }
        else
        {
            return a->ParentRank < b->ParentRank;
        }
}

