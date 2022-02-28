#include <time.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include "BoundaryPolygon.h"
#include "MyEdge.h"
#include "MyVertex.h"
#include "WingedEdgeDataStructure.h"
#include "rg_Circle2D.h"
#include "rg_Const.h"
#include "rg_Point2D.h"

using namespace std;

int main() {
    time_t start, end;
    double result;
    start = time(NULL);

    WingedEdgeDataStructure* myWingedEdge = new WingedEdgeDataStructure();
    WingedEdgeDataStructure newWD;

    vector<WingedEdgeDataStructure::Face*> faceVector;

    vector<rg_Point2D*> generatorList;
    int faceIndex = 0;
    vector<MyVertex*> vertexVector;
    vector<rg_Circle2D*> circleVector;
    int circleIndex = 0;
    vector<MyEdge*> edgeVector;

    vector<vector<MyEdge*>> edgeVectorOfVertexVector;


    ifstream file("./../../data/voronoi_test100.txt");

    int a = 0;

    string tmp;
    getline(file, tmp);

    // 
    while (!file.eof()) {
        string line;
        string buf;
        double i = 0;

        string x, y;

        getline(file, line);

        stringstream ss(line);
        ss >> i >> x >> y;

        rg_Point2D* pt = new rg_Point2D(stod(x), stod(y));
        generatorList.push_back(pt);

        WingedEdgeDataStructure::Face* currentFace = new WingedEdgeDataStructure::Face(faceIndex);
        myWingedEdge->concatenate_face(currentFace);
        faceVector.push_back(currentFace);

        faceIndex++;
    }

    for (int i = 0; i < generatorList.size(); i++) {
        for (int j = i + 1; j < generatorList.size(); j++) {
            for (int k = j + 1; k < generatorList.size(); k++) {
                rg_Circle2D* currentCircle = new rg_Circle2D(*generatorList[i], *generatorList[j], *generatorList[k]);

                double r = sqrt(pow((generatorList[i]->getX() - currentCircle->getCenterPt().getX()), 2) + pow((generatorList[i]->getY() - currentCircle->getCenterPt().getY()), 2));
                currentCircle->setRadius(r);

                bool flag = true;

                for (int l = 0; l < generatorList.size(); l++) {
                    if (currentCircle->doesContain(*generatorList[l]) == true) {
                        flag = false;
                        delete currentCircle;
                    }
                }

                if (flag) {
                    bool isObtuseTriangle = false;

                    MyVertex* currentVertex = new MyVertex(circleIndex);
                    currentVertex->m_point = currentCircle->getCenterPt();

                    myWingedEdge->concatenate_vertex(currentVertex);

                    vertexVector.push_back(currentVertex);
                    circleVector.push_back(currentCircle);

                    vector<MyEdge*> edgeVectorOfVertex;
                    edgeVectorOfVertexVector.push_back(edgeVectorOfVertex);

                    //
                    double d1 = generatorList[i]->distance(*generatorList[j]);
                    double d2 = generatorList[j]->distance(*generatorList[k]);
                    double d3 = generatorList[k]->distance(*generatorList[i]);

                    int maxfaceLeft = 0;
                    int maxfaceRight = 0;

                    double max = 0.0;
                    if (d1 > max) {
                        maxfaceLeft = i;
                        maxfaceRight = j;
                        max = d1;
                    }
                    if (d2 > max) {
                        maxfaceLeft = j;
                        maxfaceRight = k;
                        max = d2;
                    }
                    if (d3 > max) {
                        maxfaceLeft = i;
                        maxfaceRight = k;
                        max = d3;
                    }

                    if (max * max + max * max >= d1 * d1 + d2 * d2 + d3 * d3) {
                        isObtuseTriangle = true;
                    }

                    bool isNewEdge1 = true;

                    for (int m1 = 0; m1 < edgeVector.size(); m1++) {
                        MyEdge* compareEdge = edgeVector[m1];
                        if (compareEdge->left_face()->getID() == i && compareEdge->right_face()->getID() == j) {
                            isNewEdge1 = false;

                            if (compareEdge->end_vertex() == nullptr) {
                                compareEdge->set_end_vertex(currentVertex);
                            }
                            if (compareEdge->start_vertex() == nullptr) {
                                compareEdge->set_start_vertex(currentVertex);
                            }

                            currentVertex->set_first_edge(compareEdge);
                            edgeVectorOfVertexVector[circleIndex].push_back(compareEdge);
                        }
                    }

                    if (isNewEdge1) {
                        MyEdge* currentEdge1 = new MyEdge();

                        currentEdge1->set_left_face(faceVector[i]);
                        currentEdge1->set_right_face(faceVector[j]);

                        rg_Point2D vertexPoint = currentCircle->getCenterPt();
                        rg_Point2D g1 = *generatorList[i];
                        rg_Point2D g2 = *generatorList[j];

                        double x1 = g1.getX() - vertexPoint.getX();
                        double y1 = g1.getY() - vertexPoint.getY();
                        double x2 = g2.getX() - vertexPoint.getX();
                        double y2 = g2.getY() - vertexPoint.getY();
                        double direction = x1 * y2 - y1 * x2;

                        if (direction < 0) {
                            currentEdge1->set_start_vertex(currentVertex);
                        }

                        if (direction > 0) {
                            currentEdge1->set_end_vertex(currentVertex);
                        }
                        edgeVector.push_back(currentEdge1);

                        currentVertex->set_first_edge(currentEdge1);
                        edgeVectorOfVertexVector[circleIndex].push_back(currentEdge1);
                        currentEdge1->m_vector = ((*generatorList[i] + *generatorList[j]) / 2 - currentCircle->getCenterPt()).getUnitVector();

                        if (isObtuseTriangle && maxfaceLeft == i && maxfaceRight == j) {
                            currentEdge1->m_direction = false;

                            MyVertex* temp = (MyVertex*)currentEdge1->end_vertex();
                            currentEdge1->set_end_vertex(currentEdge1->start_vertex());
                            currentEdge1->set_start_vertex(temp);
                        }
                    }

                    bool isNewEdge2 = true;

                    for (int m2 = 0; m2 < edgeVector.size(); m2++) {
                        MyEdge* compareEdge = edgeVector[m2];
                        if (compareEdge->left_face()->getID() == i && compareEdge->right_face()->getID() == k) {
                            isNewEdge2 = false;

                            if (compareEdge->end_vertex() == nullptr) {
                                compareEdge->set_end_vertex(currentVertex);
                            }
                            if (compareEdge->start_vertex() == nullptr) {
                                compareEdge->set_start_vertex(currentVertex);
                            }

                            currentVertex->set_first_edge(compareEdge);
                            edgeVectorOfVertexVector[circleIndex].push_back(compareEdge);
                        }
                    }
                    if (isNewEdge2) {
                        MyEdge* currentEdge1 = new MyEdge();

                        currentEdge1->set_left_face(faceVector[i]);
                        currentEdge1->set_right_face(faceVector[k]);

                        rg_Point2D vertexPoint = currentCircle->getCenterPt();
                        rg_Point2D g1 = *generatorList[i];
                        rg_Point2D g2 = *generatorList[k];

                        double x1 = g1.getX() - vertexPoint.getX();
                        double y1 = g1.getY() - vertexPoint.getY();
                        double x2 = g2.getX() - vertexPoint.getX();
                        double y2 = g2.getY() - vertexPoint.getY();
                        double direction = x1 * y2 - y1 * x2;

                        if (direction < 0) {
                            currentEdge1->set_start_vertex(currentVertex);
                        }

                        if (direction > 0) {
                            currentEdge1->set_end_vertex(currentVertex);
                        }
                        edgeVector.push_back(currentEdge1);

                        currentVertex->set_first_edge(currentEdge1);
                        edgeVectorOfVertexVector[circleIndex].push_back(currentEdge1);
                        currentEdge1->m_vector = ((*generatorList[i] + *generatorList[k]) / 2 - currentCircle->getCenterPt()).getUnitVector();

                        if (isObtuseTriangle && maxfaceLeft == i && maxfaceRight == k) {
                            currentEdge1->m_direction = false;

                            MyVertex* temp = (MyVertex*)currentEdge1->end_vertex();
                            currentEdge1->set_end_vertex(currentEdge1->start_vertex());
                            currentEdge1->set_start_vertex(temp);
                        }
                    }

                    bool isNewEdge3 = true;

                    for (int m3 = 0; m3 < edgeVector.size(); m3++) {
                        MyEdge* compareEdge = edgeVector[m3];
                        if (compareEdge->left_face()->getID() == j && compareEdge->right_face()->getID() == k) {
                            isNewEdge3 = false;

                            if (compareEdge->end_vertex() == nullptr) {
                                compareEdge->set_end_vertex(currentVertex);
                            }
                            if (compareEdge->start_vertex() == nullptr) {
                                compareEdge->set_start_vertex(currentVertex);
                            }

                            currentVertex->set_first_edge(compareEdge);
                            edgeVectorOfVertexVector[circleIndex].push_back(compareEdge);
                        }
                    }

                    if (isNewEdge3) {
                        MyEdge* currentEdge1 = new MyEdge();

                        currentEdge1->set_left_face(faceVector[j]);
                        currentEdge1->set_right_face(faceVector[k]);

                        rg_Point2D vertexPoint = currentCircle->getCenterPt();
                        rg_Point2D g1 = *generatorList[j];
                        rg_Point2D g2 = *generatorList[k];

                        double x1 = g1.getX() - vertexPoint.getX();
                        double y1 = g1.getY() - vertexPoint.getY();
                        double x2 = g2.getX() - vertexPoint.getX();
                        double y2 = g2.getY() - vertexPoint.getY();
                        double direction = x1 * y2 - y1 * x2;

                        if (direction < 0) {
                            currentEdge1->set_start_vertex(currentVertex);
                        }

                        if (direction > 0) {
                            currentEdge1->set_end_vertex(currentVertex);
                        }
                        edgeVector.push_back(currentEdge1);

                        currentVertex->set_first_edge(currentEdge1);
                        edgeVectorOfVertexVector[circleIndex].push_back(currentEdge1);
                        currentEdge1->m_vector = ((*generatorList[j] + *generatorList[k]) / 2 - currentCircle->getCenterPt()).getUnitVector();

                        if (isObtuseTriangle && maxfaceLeft == j && maxfaceRight == k) {
                            currentEdge1->m_direction = false;

                            MyVertex* temp = (MyVertex*)currentEdge1->end_vertex();
                            currentEdge1->set_end_vertex(currentEdge1->start_vertex());
                            currentEdge1->set_start_vertex(temp);
                        }
                    }

                    circleIndex++;
                }
            }
        }
    }

    for (int i = 0; i < edgeVectorOfVertexVector.size(); i++) {
        vector<MyEdge*> currentVectorOfEdge = edgeVectorOfVertexVector[i];

        MyEdge* currentEdge1 = currentVectorOfEdge[0];
        MyEdge* currentEdge2 = currentVectorOfEdge[1];
        MyEdge* currentEdge3 = currentVectorOfEdge[2];

        if (currentEdge1->is_start_vertex(currentEdge2->start_vertex())) {
            if (currentEdge1->right_face() == currentEdge2->left_face()) {
                currentEdge1->set_right_leg_edge(currentEdge2);
                currentEdge2->set_left_leg_edge(currentEdge1);
            }

            if (currentEdge1->left_face() == currentEdge2->right_face()) {
                currentEdge1->set_left_leg_edge(currentEdge2);
                currentEdge2->set_right_leg_edge(currentEdge1);
            }
        }

        if (currentEdge1->is_start_vertex(currentEdge2->end_vertex())) {
            if (currentEdge1->right_face() == currentEdge2->right_face()) {
                currentEdge1->set_right_leg_edge(currentEdge2);
                currentEdge2->set_right_hand_edge(currentEdge1);
            }

            if (currentEdge1->left_face() == currentEdge2->left_face()) {
                currentEdge1->set_left_leg_edge(currentEdge2);
                currentEdge2->set_left_hand_edge(currentEdge1);
            }
        }

        if (currentEdge1->is_end_vertex(currentEdge2->start_vertex())) {
            if (currentEdge1->right_face() == currentEdge2->right_face()) {
                currentEdge1->set_right_hand_edge(currentEdge2);
                currentEdge2->set_right_leg_edge(currentEdge1);
            }

            if (currentEdge1->left_face() == currentEdge2->left_face()) {
                currentEdge1->set_left_hand_edge(currentEdge2);
                currentEdge2->set_left_leg_edge(currentEdge1);
            }
        }

        if (currentEdge1->is_end_vertex(currentEdge2->end_vertex())) {
            if (currentEdge1->left_face() == currentEdge2->right_face()) {
                currentEdge1->set_left_hand_edge(currentEdge2);
                currentEdge2->set_right_hand_edge(currentEdge1);
            }

            if (currentEdge1->right_face() == currentEdge2->left_face()) {
                currentEdge1->set_right_hand_edge(currentEdge2);
                currentEdge2->set_left_hand_edge(currentEdge1);
            }
        }

        if (currentEdge1->is_start_vertex(currentEdge3->start_vertex())) {
            if (currentEdge1->right_face() == currentEdge3->left_face()) {
                currentEdge1->set_right_leg_edge(currentEdge3);
                currentEdge3->set_left_leg_edge(currentEdge1);
            }

            if (currentEdge1->left_face() == currentEdge3->right_face()) {
                currentEdge1->set_left_leg_edge(currentEdge3);
                currentEdge3->set_right_leg_edge(currentEdge1);
            }
        }

        if (currentEdge1->is_start_vertex(currentEdge3->end_vertex())) {
            if (currentEdge1->right_face() == currentEdge3->right_face()) {
                currentEdge1->set_right_leg_edge(currentEdge3);
                currentEdge3->set_right_hand_edge(currentEdge1);
            }

            if (currentEdge1->left_face() == currentEdge3->left_face()) {
                currentEdge1->set_left_leg_edge(currentEdge3);
                currentEdge3->set_left_hand_edge(currentEdge1);
            }
        }

        if (currentEdge1->is_end_vertex(currentEdge3->start_vertex())) {
            if (currentEdge1->right_face() == currentEdge3->right_face()) {
                currentEdge1->set_right_hand_edge(currentEdge3);
                currentEdge3->set_right_leg_edge(currentEdge1);
            }

            if (currentEdge1->left_face() == currentEdge3->left_face()) {
                currentEdge1->set_left_hand_edge(currentEdge3);
                currentEdge3->set_left_leg_edge(currentEdge1);
            }
        }

        if (currentEdge1->is_end_vertex(currentEdge3->end_vertex())) {
            if (currentEdge1->left_face() == currentEdge3->right_face()) {
                currentEdge1->set_left_hand_edge(currentEdge3);
                currentEdge3->set_right_hand_edge(currentEdge1);
            }

            if (currentEdge1->right_face() == currentEdge3->left_face()) {
                currentEdge1->set_right_hand_edge(currentEdge3);
                currentEdge3->set_left_hand_edge(currentEdge1);
            }
        }

        if (currentEdge2->is_start_vertex(currentEdge3->start_vertex())) {
            if (currentEdge2->right_face() == currentEdge3->left_face()) {
                currentEdge2->set_right_leg_edge(currentEdge3);
                currentEdge3->set_left_leg_edge(currentEdge2);
            }

            if (currentEdge2->left_face() == currentEdge3->right_face()) {
                currentEdge2->set_left_leg_edge(currentEdge3);
                currentEdge3->set_right_leg_edge(currentEdge2);
            }
        }

        if (currentEdge2->is_start_vertex(currentEdge3->end_vertex())) {
            if (currentEdge2->right_face() == currentEdge3->right_face()) {
                currentEdge2->set_right_leg_edge(currentEdge3);
                currentEdge3->set_right_hand_edge(currentEdge2);
            }

            if (currentEdge2->left_face() == currentEdge3->left_face()) {
                currentEdge2->set_left_leg_edge(currentEdge3);
                currentEdge3->set_left_hand_edge(currentEdge2);
            }
        }

        if (currentEdge2->is_end_vertex(currentEdge3->start_vertex())) {
            if (currentEdge2->right_face() == currentEdge3->right_face()) {
                currentEdge2->set_right_hand_edge(currentEdge3);
                currentEdge3->set_right_leg_edge(currentEdge2);
            }

            if (currentEdge2->left_face() == currentEdge3->left_face()) {
                currentEdge2->set_left_hand_edge(currentEdge3);
                currentEdge3->set_left_leg_edge(currentEdge2);
            }
        }

        if (currentEdge2->is_end_vertex(currentEdge3->end_vertex())) {
            if (currentEdge2->left_face() == currentEdge3->right_face()) {
                currentEdge3->set_right_hand_edge(currentEdge2);
            }

            if (currentEdge2->right_face() == currentEdge3->left_face()) {
                currentEdge2->set_right_hand_edge(currentEdge3);
                currentEdge3->set_left_hand_edge(currentEdge2);
            }
        }
    }

    for (int i = 0; i < edgeVector.size(); i++) {
        MyEdge* currentEdge = edgeVector[i];
        currentEdge->left_face()->set_first_edge(currentEdge);
        currentEdge->right_face()->set_first_edge(currentEdge);
        myWingedEdge->concatenate_edge(currentEdge);
    }

    double boundary_value = 6000;
    rg_Point2D boundaryPoint1 = rg_Point2D(boundary_value, boundary_value);
    rg_Point2D boundaryPoint2 = rg_Point2D(-boundary_value, boundary_value);
    rg_Point2D boundaryPoint3 = rg_Point2D(-boundary_value, -boundary_value);
    rg_Point2D boundaryPoint4 = rg_Point2D(boundary_value, -boundary_value);

    list<rg_Line2D*> lineList;
    rg_Line2D* BoundaryLine1 = new rg_Line2D(boundaryPoint1, boundaryPoint2);
    rg_Line2D* BoundaryLine2 = new rg_Line2D(boundaryPoint2, boundaryPoint3);
    rg_Line2D* BoundaryLine3 = new rg_Line2D(boundaryPoint3, boundaryPoint4);
    rg_Line2D* BoundaryLine4 = new rg_Line2D(boundaryPoint4, boundaryPoint1);
    lineList.push_back(BoundaryLine1);
    lineList.push_back(BoundaryLine2);
    lineList.push_back(BoundaryLine3);
    lineList.push_back(BoundaryLine4);

    BoundaryPolygon* boundary = new BoundaryPolygon(lineList, 2 * boundary_value);

    for (int i = 0; i < edgeVector.size(); i++) {
        MyEdge* unboundedEdge = edgeVector[i];
        if (unboundedEdge->end_vertex() == nullptr || unboundedEdge->start_vertex() == nullptr) {
            cout << i << endl;
            rg_Point2D* newPoint = new rg_Point2D();

            if (boundary->find_intersect(unboundedEdge, *newPoint)) {
                MyVertex* newVertex = new MyVertex(*newPoint);

                if (unboundedEdge->end_vertex() == nullptr) {
                    unboundedEdge->set_end_vertex(newVertex);
                }

                else if (unboundedEdge->start_vertex() == nullptr) {
                    unboundedEdge->set_start_vertex(newVertex);
                }
            }

            else {
                cout << "gg" << endl;

                if (unboundedEdge->end_vertex() == nullptr) {
                    if (unboundedEdge->m_direction == true) {
                        *newPoint = ((MyVertex*)(unboundedEdge->start_vertex()))->m_point + 2 * boundary_value * unboundedEdge->m_vector;
                    } else if (unboundedEdge->m_direction == false) {
                        *newPoint = ((MyVertex*)(unboundedEdge->start_vertex()))->m_point - 2 * boundary_value * unboundedEdge->m_vector;
                    }
                    MyVertex* newVertex = new MyVertex(*newPoint);
                    unboundedEdge->set_end_vertex(newVertex);
                }

                else if (unboundedEdge->start_vertex() == nullptr) {
                    if (unboundedEdge->m_direction == true) {
                        *newPoint = ((MyVertex*)(unboundedEdge->end_vertex()))->m_point + 2 * boundary_value * unboundedEdge->m_vector;
                    } else if (unboundedEdge->m_direction == false) {
                        *newPoint = ((MyVertex*)(unboundedEdge->end_vertex()))->m_point - 2 * boundary_value * unboundedEdge->m_vector;
                    }
                    MyVertex* newVertex = new MyVertex(*newPoint);
                    unboundedEdge->set_start_vertex(newVertex);
                }
            }
        }
    }

    end = time(NULL);
    result = (double)(end - start);
    cout << "RunTime is " << result << endl;

    ofstream fout;
    fout.open("output100.txt");

    for (int i = 0; i < edgeVector.size(); i++) {
        MyVertex* startV = (MyVertex*)(edgeVector[i]->start_vertex());
        MyVertex* endV = (MyVertex*)(edgeVector[i]->end_vertex());

        double start_x = startV->m_point.getX();
        double start_y = startV->m_point.getY();

        double end_x = endV->m_point.getX();
        double end_y = endV->m_point.getY();

        fout << start_x << ',' << start_y << ',' << end_x << ',' << end_y << endl;
    }

    fout.close();
}