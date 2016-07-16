#include <algorithm>
#include <vector>

#include "opp_utils.h"

#include "ObjectiveFunFuzz.h"


int ObjectiveFunFuzz::IsParent(const IPv6Address& srcAddr,double rank)
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

double ObjectiveFunFuzz::UpdateParent(double crisp_value, int srID, const IPv6Address& srcAddr,int rank, const MACAddress& srcMACAddr)
{
    ParentStructure* rcv = new ParentStructure();
    rcv->srnode = srID;
    rcv->Crisp_value = crisp_value;
    rcv->ParentId = srcAddr;
    rcv->ParentMACAddr = srcMACAddr;
    rcv->ParentRank = rank;
    EV<<"rcv mac addr:"<<srcMACAddr<<endl;
    EV<<"rcv parent ipv6 addr:"<<rcv->ParentId<<endl;
    std::vector<ParentStructure*>::iterator it;
    for(it = countercache.begin(); it != countercache.end(); it++)
    {
        if((*it)->srnode == rcv->srnode)
        {
            (*it)->Crisp_value = rcv->Crisp_value;
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
    metrics = countercache[0]->Crisp_value; //here is the so-called off.metrics (objective function fuzzy)
    return countercache[0]->Crisp_value;

}

void ObjectiveFunFuzz::test()
{
    std::vector<ParentStructure*>::iterator it;
    for(it = countercache.begin(); it != countercache.end(); it++)
        {
            EV<<(*it)->ParentId<<" ";
        }
    EV<<endl;
}

double ObjectiveFunFuzz::counterCal(int srID, double metrics_t, double metrics_plus)
{
    //--begin--handling with counter cache, added 25th April
    ParentStructure* rcv = new ParentStructure();
    rcv->srnode = srID;

    std::vector<ParentStructure*>::iterator it;
    EV<<"metrics_t:"<<metrics_t<<endl;
    EV<<"metrics_plus:"<<metrics_plus<<endl;
    metrics_t = metrics_t;
    metrics_plus = metrics_plus;

    rcv->Crisp_value = fuzzsystem(metrics_t, metrics_plus);
    for(it = countercache.begin(); it != countercache.end(); it++)
    {
        if(rcv->srnode == (*it)->srnode)
        {
            (*it)->Crisp_value = rcv->Crisp_value;
            return rcv->Crisp_value;
        }
    }
    if (it == countercache.end()) {
            countercache.push_back(rcv);
            EV<<"First Time rcving:"<<rcv->Crisp_value<<endl;
        }

    return rcv->Crisp_value;
        //--end--handling with counter cache, added 5th June
}

//For addictive metrics calculation
double ObjectiveFunFuzz::addictiveMetrics_t(int srID, double precount, int NumDio)
{
    //--begin--handling with counter cache, added 25th April
    ParentStructure* rcv = new ParentStructure();
    rcv->srnode = srID;
    rcv->Rcvdcounter = 1;
    std::vector<ParentStructure*>::iterator it;
    EV<<"precount:"<<precount<<endl;
    EV<<"Numdio:"<<NumDio<<endl;
    for(it = countercache.begin(); it != countercache.end(); it++)
    {
        if(rcv->srnode == (*it)->srnode)
        {
            double temp = ++((*it)->Rcvdcounter);
            EV<<"Existed update to:"<<temp<<endl;
            (*it)->Addcounter = (NumDio/temp) + precount;//(temp/NumDio) * precount;
            return (NumDio/temp) + precount;//(temp/NumDio) * precount;
        }
    }
    if (it == countercache.end()) {
            countercache.push_back(rcv);
            EV<<"First Time rcving:"<<rcv->Rcvdcounter<<endl;
        }
    rcv->Addcounter = NumDio/rcv->Rcvdcounter + precount;//rcv->Rcvdcounter/NumDio * precount;
    metrics_t = NumDio/rcv->Rcvdcounter + precount;
    return NumDio/rcv->Rcvdcounter + precount;//(rcv->Rcvdcounter/NumDio) * precount;
        //--end--handling with counter cache, added 25th April
}



//Added May, for handling by processDIO
bool ObjectiveFunFuzz::counterLessThan(const ParentStructure *a, const ParentStructure *b)
{
    if(a->Crisp_value != b->Crisp_value)
        {
            return a->Crisp_value < b->Crisp_value;
        }
        else
        {
            return a->ParentRank < b->ParentRank;
        }
}

//This function serves as the fuzzification system, it will make use of all the below function to
//calculate the composite metrics with the two inputs
double ObjectiveFunFuzz::fuzzsystem(double metrics_t, double metrics_plus)
{
    ObjectiveFunFuzz off;
    std::vector<double> fuzz = off.fuzzy(metrics_t);
    std::vector<double> fuzz1 = off.fuzzy1(metrics_plus);
    std::vector<double> mam = off.mamdani(fuzz, fuzz1);
    return off.deffuzification(mam);
}


//Here below is all about fuzzification process, it will describe the fuzzification process in detail

std::vector<double> ObjectiveFunFuzz::fuzzy(double metrics_t)
{
    std::vector<double> fuzzy(3);

    if(metrics_t <= 2.17)
    {
        fuzzy[0] = 1;
        fuzzy[1] = 0;
        fuzzy[2] = 0;
    }
    else if(2.17 < metrics_t && metrics_t < 3.83)
    {

        fuzzy[0] = (3.83 - metrics_t)/(3.83-2.17);
        fuzzy[1] = (metrics_t - 2.17)/(3.83-2.17);
        fuzzy[2] = 0;
    }
    else if(3.83 <= metrics_t && metrics_t <= 6.8)
    {
        fuzzy[0] = 0;
        fuzzy[1] = 1;
        fuzzy[2] = 0;
    }
    else if(6.8 < metrics_t && metrics_t < 11.2)
    {
        fuzzy[0] = 0;
        fuzzy[1] = (11.2 - metrics_t)/(11.2-6.8);
        fuzzy[2] = (metrics_t - 6.8)/(11.2-6.8);
    }
    else
    {
        fuzzy[0] = 0;
        fuzzy[1] = 0;
        fuzzy[2] = 1;
    }
    return fuzzy;
}


std::vector<double> ObjectiveFunFuzz::fuzzy1(double metrics_t)
{
    std::vector<double> fuzzy(3);
    metrics_t = metrics_t*100;   //according to the judge condition, in percent form

    if(metrics_t >= 92.7)
    {
        fuzzy[0] = 1;
        fuzzy[1] = 0;
        fuzzy[2] = 0;
    }
    else if(75.8 < metrics_t && metrics_t < 92.7)
    {

        fuzzy[0] = (metrics_t - 75.8)/(92.7-75.8);
        fuzzy[1] = (92.7 - metrics_t)/(92.7-75.8);
        fuzzy[2] = 0;
    }
    else if(62.5 <= metrics_t && metrics_t <= 75.8)
    {
        fuzzy[0] = 0;
        fuzzy[1] = 1;
        fuzzy[2] = 0;
    }
    else if(37.5 < metrics_t && metrics_t < 62.5)
    {
        fuzzy[0] = 0;
        fuzzy[1] = (metrics_t - 37.5)/(62.5-37.5);
        fuzzy[2] = (62.5 - metrics_t)/(62.5-37.5);
    }
    else
    {
        fuzzy[0] = 0;
        fuzzy[1] = 0;
        fuzzy[2] = 1;
    }
    return fuzzy;
}

//Mamdani Model
std::vector<double> ObjectiveFunFuzz::mamdani(std::vector<double> fuzz, std::vector<double> fuzz1)
{
    std::vector<double> mamdani;
    for (unsigned int mamindex = 0; mamindex < (fuzz.size()+fuzz1.size()-1); mamindex++)
    {
        std::vector<double> tmp(fuzz.size()+fuzz1.size()-1);
        for (unsigned int i = 0; i <= mamindex; i++)
        {
            if(mamindex <= fuzz1.size())
                tmp.push_back(fmin(fuzz[i],fuzz1[mamindex-i]));  //fmin(x,y)=x<y?x:y defined in math.h
        }
        std::vector<double>::iterator it;
        it = std::max_element(tmp.begin(),tmp.end());
        mamdani.push_back(*it);
    }
    return mamdani;
}

double ObjectiveFunFuzz::deffuzification(std::vector<double> mamdani)
{
    //Here in the deffuzification process, we take centroid means, which is the most common and valid means for
    //deffuzification, it will also be reflected in the membership function, however here we can just use the
    //centroid to calculate the crisp value

    double crisp_value = 0.0;

    double centroid[5] = {0.1,0.3,0.5,0.7,0.9};
    int i = 0;

    std::vector<double>::iterator it;
    for(it = mamdani.begin();it != mamdani.end(); it++)
    {
        crisp_value = crisp_value + (*it)*centroid[i];
        i++;
    }

    return crisp_value;
}



