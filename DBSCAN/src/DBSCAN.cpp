#include <DBSCAN.h>

#include <cmath>
#include <cassert>
#include <iostream>

namespace dbscan {



DBSCAN::DBSCAN(double eps, int minPts):
    m_eps(eps),
    m_minPts(minPts)
{

}

void DBSCAN::run(std::vector<pPointType> &pPoints) {

    int clusterId = 1;
    std::vector<pPointType>::iterator iter;
    for (iter = pPoints.begin(); iter != pPoints.end(); ++iter) {
        if ((*iter)->m_label == Label::UNCLASSIFIED) {
            if (expandCluster(*iter, clusterId, pPoints)) {
                clusterId++;
            }
        }
    }
}

bool DBSCAN::expandCluster(pPointType &pPoint, int clusterId, std::vector<pPointType> &pPoints) {
    std::vector<pPointType> neighbPoints = std::move(getEpsNeighb(pPoint, pPoints));

    if (neighbPoints.size() < this->m_minPts) {
        pPoint->m_label = Label::NOISE;
        return false;
    }
    else {
        for (auto iter = neighbPoints.begin(); iter != neighbPoints.end(); ++iter) {
            (*iter)->m_label = Label::CORE;
            (*iter)->m_clusterId = clusterId;
        }

        while (!neighbPoints.empty()) {
            pPointType pCurrentP = neighbPoints.back();
            std::vector<pPointType> innerNeighb = std::move(getEpsNeighb(pCurrentP, pPoints));

            if (innerNeighb.size() >= m_minPts) {
                for (auto iter = innerNeighb.begin(); iter != innerNeighb.end(); ++iter) {
                    if ((*iter)->m_label == Label::UNCLASSIFIED || (*iter)->m_label == Label::NOISE) {
                        if ((*iter)->m_label == Label::UNCLASSIFIED) {
                            neighbPoints.push_back((*iter));
                        }

                        (*iter)->m_label = Label::CORE;
                        (*iter)->m_clusterId = clusterId;
                    }
                }
            }

            neighbPoints.pop_back();
        }
    }

    return true;
}

std::vector<pPointType> DBSCAN::getEpsNeighb(pPointType &pPoint, std::vector<pPointType> &points)
{
    std::vector<pPointType> neighb{};
    for (auto iter = points.begin(); iter != points.end(); ++iter) {
        if (euclideanDist((*iter), pPoint) <= m_eps) {
            neighb.push_back((*iter));
        }
    }

    return neighb;
}

double DBSCAN::euclideanDist(pPointType &pPointa, pPointType &pPointb) {
    assert(pPointa->m_coords.size() == pPointb->m_coords.size());
    float sum = 0;
    auto first1 = pPointa->m_coords.begin();
    auto first2 = pPointb->m_coords.begin();
    auto last = pPointa->m_coords.end();
    while(first1 != last)
    {
        sum += pow(((*first1++) - (*first2++)), 2);
    }
    return sqrt(sum);
}

} // dbscan namespace
