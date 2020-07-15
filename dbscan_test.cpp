#include <DBSCAN.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

std::vector<dbscan::pPointType> loadMatrixFromCSVFile(std::string);
std::vector<std::string> splitLine(std::string, char);

int main(int argc, char const *argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " INPUT.csv eps minPts" << std::endl;
        return 1;
    }

    int minPts = std::atoi(argv[3]);
    double eps = std::atof(argv[2]);
    std::vector<dbscan::pPointType> pPoints = loadMatrixFromCSVFile(argv[1]);

    std::cout << "minPts: " << minPts << std::endl;
    std::cout << "eps: " << eps << std::endl;

    std::clock_t time_req = std::clock();
    dbscan::DBSCAN db(eps, minPts);
    db.run(pPoints);
    time_req = std::clock() - time_req;
    std::cout << "DBSCAN took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << std::endl;

    for(auto &pPoint: pPoints)
    {
        for(auto &coord: pPoint->m_coords)
        {
            std::cout << float(coord) << ", ";
        }

        std::cout << int(pPoint->m_clusterId) << std::endl;
    }

    std::cout << "Done!" << std::endl;

    return 0;
}

std::vector<dbscan::pPointType> loadMatrixFromCSVFile(std::string file_path) {
    std::vector<dbscan::pPointType> mat{};

    if (file_path != "") {
        std::ifstream inf;
        std::string line;
        std::vector<std::string> splited;

        inf.open(file_path);

        if (inf.is_open()) {
            while (getline(inf, line)) {
                splited = splitLine(line, ',');
                if (splited.size() > 0) {
                    std::vector<float> coords{};
                    for(auto &elem : splited)
                    {
                        coords.push_back(std::stof(elem));
                    }
                    mat.push_back(
                        dbscan::pPointType(new dbscan::Point(coords))
                    );
                }
            }
            inf.close();
        }
    }

    return mat;
}

std::vector<std::string> splitLine(std::string line, char delim) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> v = std::vector<std::string>();

    while (getline(ss, token, delim)) {
        v.push_back(token);
    }

    return v;
}
