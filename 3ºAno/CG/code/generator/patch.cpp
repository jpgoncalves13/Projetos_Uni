#include <regex>
#include <fstream>
#include "patch.hpp"
#include "../shared/matrixOp.hpp"
#include "../shared/point.hpp"
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <unordered_map>
#include <vector>

/**
 * Get the values of the vector from the function (f³, f², f, 1)
 * @param f value to calculate
 * @param v vector to fill
 */
void getVectorF(float f, float *v) {
    v[0] = f * f * f;
    v[1] = f * f;
    v[2] = f;
    v[3] = 1;
}

/**
 * Get the values of the vector from the derivative function (3f³, 2f, 1, 0)
 * @param f value to calculate
 * @param v vector to fill
 */
void getVectorFDeriv(float f, float *v) {
    v[0] = 3 * f * f;
    v[1] = 2 * f;
    v[2] = 1;
    v[3] = 0;
}

/**
 * Calculate the derivative relative to u coord
 * @param u u value
 * @param v v value
 * @param p point matrix with all the points in the patch (already multiplied by M)
 * @param res result to be returned
 */
void getDerivU(float u, float v, Point *p, Point *res) {
    float vecV[4], vecUDeriv[4];
    getVectorF(v, vecV);

    Point pn[4];
    getVectorFDeriv(u, vecUDeriv);
    multiplyVectorPointMatrix(vecUDeriv, p, pn); // uDeriv x P
    multiplyPointVectorVector(pn, vecV, res); // resAbove x vecV
}

/**
 * Calculate the derivative of relative to v coord
 * @param u u value
 * @param v v value
 * @param p point matrix with all the points in the patch (already multiplied by M)
 * @param res result to be returned
 */
void getDerivV(float u, float v, Point *p, Point *res) {
    float vecU[4], vecVDeriv[4];
    getVectorF(u, vecU);

    Point pn[4];
    getVectorFDeriv(v, vecVDeriv);
    multiplyVectorPointMatrix(vecU, p, pn);
    multiplyPointVectorVector(pn, vecVDeriv, res);
}

/**
 * Calculate the normal to a surface in (u,v)
 * @param u u value
 * @param v v value
 * @param p point matrix (already multiplied by M)
 * @return the normal
 */
Point getNormal(float u, float v, Point *p) {
    Point derivU, derivV;
    getDerivU(u, v, p, &derivU);
    getDerivV(u, v, p, &derivV);
    Point r = Point::crossProduct(derivU, derivV); // Normal as the crossProduct of the tangent vectors in both directions
    r.normalize(); // Must normalize always
    return r;
}

/**
 * Generate the points of the patch
 * @param controlPoints
 * @param patchesIndexes
 * @param tesselation
 * @param indexes
 * @param normals
 * @param textCoord
 * @param radiusSphere
 * @param approxCenter
 * @return
 */
vector<float> generatePatches(vector<Point> controlPoints, vector<unsigned int> patchesIndexes, int tesselation, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoord, float* radiusSphere, Point approxCenter) {
    vector<float> points;
    int index = 0;
    size_t pSize = patchesIndexes.size();
    float delta = 1.0f / (float) tesselation;
    float vecU[4], vecNU[4], vecV[4];

    // Catmull-Rom Matrix
    float m[16] = {-1, 3, -3, 1,
                   3, -6, 3, 0,
                   -3, 3, 0, 0,
                   1, 0, 0, 0};

    if (patchesIndexes.size() == 0){
        pSize = controlPoints.size();
    }

    // Map to store the (u,v) pairs and the index of normal in the normals array of the points with the normal "nan" 
    std::unordered_map<Point, std::vector<std::tuple<float, float, int>>, Point::HashFunction>* pointsMap = new std::unordered_map<Point, std::vector<std::tuple<float, float, int>>, Point::HashFunction>[pSize / 16];
    // Map to store the normals of the vertice with the (u,v) pair
    std::map<std::pair<float, float>, Point>* patchNormals = new std::map<std::pair<float, float>, Point>[pSize / 16];
    
    
    // Iterate over the patches
    for(int k=0; k< pSize; k+=16){
        Point p[16];
        // Get the patches control Points
        if (patchesIndexes.size() > 0) {
            p[0] = controlPoints[patchesIndexes[k]]; p[1] = controlPoints[patchesIndexes[k + 4]];
            p[2] = controlPoints[patchesIndexes[k + 8]]; p[3] = controlPoints[patchesIndexes[k + 12]];
            p[4] = controlPoints[patchesIndexes[k + 1]]; p[5] = controlPoints[patchesIndexes[k + 5]];
            p[6] = controlPoints[patchesIndexes[k + 9]]; p[7] = controlPoints[patchesIndexes[k + 13]];
            p[8] = controlPoints[patchesIndexes[k + 2]]; p[9] = controlPoints[patchesIndexes[k + 6]];
            p[10] = controlPoints[patchesIndexes[k + 10]]; p[11] = controlPoints[patchesIndexes[k + 14]];
            p[12] = controlPoints[patchesIndexes[k + 3]]; p[13] = controlPoints[patchesIndexes[k + 7]];
            p[14] = controlPoints[patchesIndexes[k + 11]]; p[15] = controlPoints[patchesIndexes[k + 15]];
        }
        else{
            p[0] = controlPoints[k]; p[1] = controlPoints[k+4];
            p[2] = controlPoints[k+8]; p[3] = controlPoints[k+12];
            p[4] = controlPoints[k+1]; p[5] = controlPoints[k+5];
            p[6] = controlPoints[k+9]; p[7] = controlPoints[k+13];
            p[8] = controlPoints[k+2]; p[9] = controlPoints[k+6];
            p[10] = controlPoints[k+10]; p[11] = controlPoints[k+14];
            p[12] = controlPoints[k+3]; p[13] = controlPoints[k+7];
            p[14] = controlPoints[k+11]; p[15] = controlPoints[k+15];
        }
        Point a[16]; Point b[16]; // a = M * P, b = a * Mt
        multiplyMatrixPointMatrix(m, p, a);
        multiplyPointMatrixMatrix(a, m, b); // b is the multiplication M x P x M^T

        float textInc = 1.0f/(float)tesselation;
        float distToCenter;
        for (int i = 0; i < tesselation; i++) {
            float u = delta * (float) i;
            float nextU = delta * (float) (i+1);

            getVectorF(u, vecU);
            getVectorF(nextU, vecNU);
            Point c[4], nc[4]; // C = u x M x P x M^T
            multiplyVectorPointMatrix(vecU, b, c); // Multiply by U (c)
            multiplyVectorPointMatrix(vecNU, b, nc);  // Multiply by U(nextC)

            getVectorF(0, vecV);
            Point f1, f2; // Points starting the one column of the patch
            // Final calculation : P = u x M x P x M^T x V
            multiplyPointVectorVector(c, vecV, &f1); // Multiply by V (c)
            multiplyPointVectorVector(nc, vecV, &f2); // Multiply by V(nextC) - the point next

            points.push_back(f1.getX());points.push_back(f1.getY());points.push_back(f1.getZ());

            distToCenter = f1.distanceTo(approxCenter);
            if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;

            // Normals in those points
            Point normal1 = getNormal(u, 0, b);

            normals->push_back(normal1.getX());
            normals->push_back(normal1.getY());
            normals->push_back(normal1.getZ());

            patchNormals[k/16][make_pair(u, 0)] = normal1;
            if (isnan(normal1.getX()) && isnan(normal1.getY()) && isnan(normal1.getZ())) {
                pointsMap[k/16][f1].push_back(make_tuple(u, 0, normals->size()-3));
            }

            textCoord->push_back(0);
            textCoord->push_back(1-i*textInc);

            points.push_back(f2.getX());points.push_back(f2.getY());points.push_back(f2.getZ());
            distToCenter = f2.distanceTo(approxCenter);
            if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;
            Point normal2 = getNormal(nextU, 0, b);

            normals->push_back(normal2.getX());
            normals->push_back(normal2.getY());
            normals->push_back(normal2.getZ());

            patchNormals[k/16][make_pair(nextU, 0)] = normal2;
            if (isnan(normal2.getX()) && isnan(normal2.getY()) && isnan(normal2.getZ())) {
                pointsMap[k/16][f2].push_back(make_tuple(nextU, 0, normals->size()-3));
            }

            textCoord->push_back(0);
            textCoord->push_back(1-(i+1)*textInc);

            // Iterate over this column (u is fixed) (v is variable)
            for(int j = 1; j <= tesselation; j++) {
                float v = delta * (float) j;

                getVectorF(v, vecV);
                Point f3, f4;
                // Calculate the points again (V vector changed)
                multiplyPointVectorVector(c, vecV, &f3);
                multiplyPointVectorVector(nc, vecV, &f4);

                points.push_back(f3.getX());points.push_back(f3.getY());points.push_back(f3.getZ());
                distToCenter = f3.distanceTo(approxCenter);
                if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;
                
                Point normal3 = getNormal(u, v, b);

                normals->push_back(normal3.getX());
                normals->push_back(normal3.getY());
                normals->push_back(normal3.getZ());
              
                patchNormals[k/16][make_pair(u, v)] = normal3;
                if (isnan(normal3.getX()) && isnan(normal3.getY()) && isnan(normal3.getZ())) {
                    pointsMap[k/16][f3].push_back(make_tuple(u, v, normals->size()-3));
                }
                textCoord->push_back(j*textInc);
                textCoord->push_back(1-i*textInc);

                points.push_back(f4.getX());points.push_back(f4.getY());points.push_back(f4.getZ());
                distToCenter = f4.distanceTo(approxCenter);
                if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;
                
                Point normal4 = getNormal(nextU, v, b);

                normals->push_back(normal4.getX());
                normals->push_back(normal4.getY());
                normals->push_back(normal4.getZ());
                
                patchNormals[k/16][make_pair(nextU, v)] = normal4;
                if (isnan(normal4.getX()) && isnan(normal4.getY()) && isnan(normal4.getZ())) {
                    pointsMap[k/16][f4].push_back(make_tuple(nextU, v, normals->size()-3));
                }
                textCoord->push_back(j*textInc);
                textCoord->push_back(1-(i+1)*textInc);

                indexes->push_back(index);
                indexes->push_back(index+1);
                indexes->push_back(index+2);

                indexes->push_back(index+3);
                indexes->push_back(index+2);
                indexes->push_back(index+1);

                index += 2;
            }

            index += 2;
        }
    }

    // Auxiliary map to calculate the interpolated normals
    unordered_map<Point, tuple<Point, int, vector<int>>, Point::HashFunction> normalsMap;

    for(int k=0; k<pSize/16; k++) {
        
        for (auto& pair : pointsMap[k]) {
            Point p = get<0>(pair);
            vector<tuple<float, float, int>> tuples = get<1>(pair);

            bool same_u = false;

            if (get<0>(tuples[0]) == get<0>(tuples[1])) {
                same_u = true;
            }


            for (int i = 0; i < tuples.size(); i++) {
                tuple<float, float, int> tp = tuples[i];
                float u = get<0>(tp);
                float v = get<1>(tp);
                int index = get<2>(tp);


                if (same_u) {
                    if (normalsMap.find(p) == normalsMap.end()) {
                        Point normal(0,0,0);
                        normalsMap[p] = make_tuple(normal, 0, vector<int>());
                    }
                    if (u==1) {
                        delta = -delta;
                    }

                    tuple<Point, int, vector<int>> n = normalsMap[p];
                    Point newNormal(get<0>(n).getX() + patchNormals[k][make_pair(u+delta,v)].getX(), get<0>(n).getY() + patchNormals[k][make_pair(u+delta,v)].getY(), get<0>(n).getZ() + patchNormals[k][make_pair(u+delta,v)].getZ());
                    int proxCount = get<1>(n) + 1;
                    vector<int> indexes = get<2>(n);
                    indexes.push_back(index);
                    normalsMap[p] = make_tuple( newNormal, proxCount , indexes);

                    if (u==1) {
                        delta = -delta;
                    }

                } else {
                    if (normalsMap.find(p) == normalsMap.end()) {
                        Point normal(0,0,0);
                        normalsMap[p] = make_tuple(normal, 0, vector<int>());
                    }

                    if (v==1) {
                        delta = -delta;
                    }

                    tuple<Point, int, vector<int>> n = normalsMap[p];
                    Point newNormal(get<0>(n).getX() + patchNormals[k][make_pair(u,v+delta)].getX(), get<0>(n).getY() + patchNormals[k][make_pair(u,v+delta)].getY(), get<0>(n).getZ() + patchNormals[k][make_pair(u,v+delta)].getZ());
                    int proxCount = get<1>(n) + 1;
                    vector<int> indexes = get<2>(n);
                    indexes.push_back(index);
                    normalsMap[p] = make_tuple( newNormal, proxCount , indexes);

                    if (v==1) {
                        delta = -delta;
                    }

                }
            }
        }
    }

    for (auto& pair : normalsMap) {
        tuple<Point, int, vector<int>> normal = pair.second;        
        Point p = get<0>(normal);
        int divide = get<1>(normal);
        p.setPoint(p.getX()/ divide, p.getY() / divide, p.getZ() / divide);
        vector<int> indexes = get<2>(normal);

        for (int i = 0; i < indexes.size(); i++) {
            (*normals)[indexes[i]] = p.getX();
            (*normals)[indexes[i] + 1] = p.getY();
            (*normals)[indexes[i] + 2] = p.getZ();
        }
    }

    delete[] pointsMap;
    delete[] patchNormals;

    return points;
}

/**
 * read the control points and the patches (indexes)
 * @param fileName
 * @param indexes
 * @param approxCenter
 * @return
 */
vector<Point> readPatch(const string& fileName, vector<unsigned int>* indexes, Point* approxCenter){
    ifstream file("../patches/" + fileName);
    vector<Point> controlPoints;
    int format = 0; // 0 - normal (with Indexes) 1 - Rhino (without indexes and y->z)

    if (!file) {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else{
        string line;
        regex r("\\s+");
        getline(file, line); // Read the first line (number of patches)
        if (line[0] == 'r'){
            format = 1;
        }
        else {
            line = regex_replace(line, r, "");
            int nPatches = stoi(line);

            for (int i = 0; i < nPatches && getline(file, line); i++) {
                line = regex_replace(line, r, "");

                string first;
                stringstream ss(line);
                for (int j = 0; j < 16 && getline(ss, first, ','); j++) {
                    indexes->push_back(stoi(first));
                }
            }

            getline(file, line); // Read the number of points
            line = regex_replace(line, r, "");
            int nPoints = stoi(line);
        }

        regex re(R"(\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*)");
        smatch match;

        for(int i = 0; getline(file, line); i++) {
            if (regex_search(line, match, re)) {
                if (format == 1){
                    Point p(stof(match.str(1)), stof(match.str(3)), stof(match.str(2)));
                    controlPoints.push_back(p);
                    approxCenter->setX(approxCenter->getX()+p.getX());
                    approxCenter->setY(approxCenter->getY()+p.getY());
                    approxCenter->setZ(approxCenter->getZ()+p.getZ());
                }
                else if (format == 0){
                    Point p(stof(match.str(1)), stof(match.str(2)), stof(match.str(3)));
                    controlPoints.push_back(p);
                    approxCenter->setX(approxCenter->getX()+p.getX());
                    approxCenter->setY(approxCenter->getY()+p.getY());
                    approxCenter->setZ(approxCenter->getZ()+p.getZ());
                }

            }
        }
    }
    // Calculate the center as the mean of all the control points
    approxCenter->setX(approxCenter->getX()/controlPoints.size());
    approxCenter->setY(approxCenter->getY()/controlPoints.size());
    approxCenter->setZ(approxCenter->getZ()/controlPoints.size());
    file.close();

    return controlPoints;
}