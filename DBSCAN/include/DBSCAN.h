#ifndef H_DBSCAN_H
#define H_DBSCAN_H

#include <memory>
#include <vector>
#include <ostream>

namespace dbscan {

enum class Label {
    UNCLASSIFIED,
    NOISE,
    CORE
};

struct Point {
    std::vector<float> m_coords;
    Label m_label;
    int m_clusterId;
    Point(std::vector<float> &coords) :
        m_coords(coords),
        m_clusterId(0),
        m_label(Label::UNCLASSIFIED)
    {

    }

};

using pPointType = std::shared_ptr<Point>;


class DBSCAN {
public:
    DBSCAN(double eps, int minPts);
    void run(std::vector<pPointType> &points);

private:
    bool expandCluster(pPointType &pPoint, int clusterId, std::vector<pPointType> &pPoints);
    std::vector<pPointType> getEpsNeighb(pPointType &pPoint, std::vector<pPointType> &points);
    double euclideanDist(pPointType &pPointa, pPointType &pPointb);

    double m_eps;
    int m_minPts;
};

} // dbscan namespace

#endif
