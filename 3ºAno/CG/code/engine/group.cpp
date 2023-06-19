#include <string>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Transformations/transformation.hpp"
#include "Transformations/scale.hpp"
#include "Transformations/translation.hpp"
#include "Transformations/rotation.hpp"
#include "../libraries/tinyxml2.h"
#include "../shared/IO.hpp"
#include "group.hpp"
#include "BoundingVolumes/plane.hpp"
#include <tuple>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace tinyxml2;

Group::Group() {
}

/**
 * Draws all the models in a group, after being applied all the transformations
 */
int Group::drawGroup(bool vboActive, Plane *planes, float *matrix, map<string, tuple<Point, float>> *teleports, bool mipmap, bool change) {
    glPushMatrix(); // Save the current matrix (because when we leave this group we want to "clean" this group's transformations)

    int nt = 0;
    // Apply the transformations
    for (int i=0; i < this->transformations.size(); i++) {
        this->transformations[i]->applyTransformation(matrix);
    }

    for (int i = 0; i < this->models.size(); i++) {
        nt += this->models[i].drawModel(vboActive, planes, matrix, teleports, mipmap, change);
    }

    float matrixCopy[16];
    for(int i = 0; i < 16; i++) {
        matrixCopy[i] = matrix[i];
    }
    // Recursively draw each subgroup with the transformations of this group enabled
    for (int i = 0; i < this->subgroups.size(); i++) {
        nt += this->subgroups[i].drawGroup(vboActive, planes, matrix, teleports, mipmap, change);
        for(int j = 0; j < 16; j++) {
            matrix[j] = matrixCopy[j];
        }
    }

    glPopMatrix(); // Restore the transformations

    return nt;
}

/**
 * Read all the attributes of a group given a XMLElement of a group and its children
 * Reads:
 * - the transformations
 * - the models/rings
 * - the subgroups
 * @param group XMLElement of a group and its children
 */
void Group::readXML(XMLElement *group, vector<string>* keys) {
    if (group) {
        /* Transformations */
        XMLElement* transformationsElem = group->FirstChildElement("transform");

        if (transformationsElem) { // Optional: can have no transformations
            int numTranslates=0, numScales=0, numRotates=0; // Control that only one transformation of each type is applied

            for (XMLElement* transform = transformationsElem->FirstChildElement(); transform != NULL; transform = transform->NextSiblingElement()) {
                string tagName = transform->Value();
                float x=0, y=0, z=0;
                const char *strX = transform->Attribute("x");
                const char *strY = transform->Attribute("y");
                const char *strZ = transform->Attribute("z");

                if (strX) {
                    x = stof(strX);
                }
                if (strY) {
                    y = stof(strY);
                }
                if (strZ) {
                    z = stof(strZ);
                }

                if (tagName.compare("translate") == 0 && numTranslates == 0){
                    float time;
                    bool align;
                    int tesselation;
                    bool show = true;
                    vector<Point> controlPoints;

                    if (!strX && !strY && !strZ) {

                        const char* strTime = transform->Attribute("time");
                        const char* strAlign = transform->Attribute("align");
                        const char* strTesselation = transform->Attribute("tesselation");
                        const char* strShow = transform->Attribute("show");

                        time = stof(strTime);
                        tesselation = strTesselation ? stof(strTesselation) : 100;

                        if (strShow && strcmp(strShow, "false") == 0) {
                            show = false;
                        }

                        if (std::strcmp(strAlign, "true") == 0) {
                            align = true;
                        }
                        else if (std::strcmp(strAlign, "false") == 0) {
                            align = false;
                        }

                        XMLElement* set = transform->FirstChildElement("pointSet");

                        if (set){ // Ex: <pointSet type="ellipse" num_points="4" a="4" b="2" angleX="30" angleY="0" angleZ="0"/>
                            const char* type = set->Attribute("type");
                            if (std::strcmp(type, "ellipse") == 0) {
                                int numPoints = stoi(set->Attribute("num_points"));
                                float a = stof(set->Attribute("a"));
                                float b = stof(set->Attribute("b"));
                                float angleX = stof(set->Attribute("angleX"));
                                float angleY = stof(set->Attribute("angleY"));
                                float angleZ = stof(set->Attribute("angleZ"));

                                float alfa=360.0/numPoints;

                                for(int i=0; i<numPoints; i++){
                                    float angle = alfa*i;
                                    float z = b*sin(angle*M_PI/180);
                                    float x = a*cos(angle*M_PI/180);
                                    Point p(x,0,z);
                                    float radius = 1; // Not necessary
                                    Rotation rX = Rotation(1,0,0, angleX, 0, 0);
                                    Rotation rY = Rotation(0,1,0, angleY, 0, 0);
                                    Rotation rZ = Rotation(0,0,1, angleZ, 0, 0);
                                    rX.applyTransformationToPoint(&p, &radius);
                                    rY.applyTransformationToPoint(&p, &radius);
                                    rZ.applyTransformationToPoint(&p, &radius);
                                    controlPoints.push_back(p);
                                }
                            }
                        }
                        else {
                            // Iterate over the control points
                            for (XMLElement *point = transform->FirstChildElement("point");
                                 point != NULL; point = point->NextSiblingElement("point")) {
                                x = stof(point->Attribute("x"));
                                y = stof(point->Attribute("y"));
                                z = stof(point->Attribute("z"));
                                Point p(x, y, z);
                                controlPoints.push_back(p);
                            }
                        }

                    }
                    else {
                        time = 0;
                        align = false;
                        tesselation = 0;
                    }
                    Translation *t = new Translation(x, y, z, time, align, tesselation, show, controlPoints);
                    this->transformations.push_back(t);
                    numTranslates++;
                }
                else if (tagName.compare("rotate") == 0 && numRotates == 0){
                    const char* strAngle = transform->Attribute("angle");
                    const char* strTime = transform->Attribute("time");
                    float angle, time;
                    if (strAngle) {
                        angle = stof(strAngle);
                    }
                    else {
                        angle = 0;
                    }
                    if (strTime) {
                        time = stof(strTime);
                    }
                    else {
                        time = 0;
                    }

                    Rotation *r = new Rotation(x, y, z,angle, 0, time);
                    this->transformations.push_back(r);
                    numRotates++;
                }
                else if (tagName.compare("scale") == 0 && numScales == 0){
                    Scale *s = new Scale(x, y, z);
                    this->transformations.push_back(s);
                    numScales++;
                }
            }
        }
        /* Models */
        XMLElement* models = group->FirstChildElement("models");
        if (models) {
            for (XMLElement* model = models->FirstChildElement("model"); model != NULL; model = model->NextSiblingElement("model")) {

                Model m;
                
                XMLElement* textureFile = model->FirstChildElement("texture");
                if (textureFile) {
                    string fileName = textureFile->Attribute("file");
                    m.setTextureFile("../Textures/" + fileName);
                }
                m.readModel(model->Attribute("file"));
                
                if (model->Attribute("label")){
                    m.setLabel(model->Attribute("label"));
                    keys->push_back(model->Attribute("label"));
                }
                else {
                    m.setLabel("undefined");
                }

                XMLElement* color = model->FirstChildElement("color");
                if (color){
                    XMLElement* diffuse = color->FirstChildElement("diffuse");
                    XMLElement* ambient = color->FirstChildElement("ambient");
                    XMLElement* specular = color->FirstChildElement("specular");
                    XMLElement* emissive = color->FirstChildElement("emissive");
                    XMLElement* shininess = color->FirstChildElement("shininess");

                    if (diffuse){
                        string red = diffuse->Attribute("R");
                        string green = diffuse->Attribute("G");
                        string blue = diffuse->Attribute("B");
                        m.setDiffuse(Point(stof(red), stof(green), stof(blue)));
                    }
                    if (ambient){
                        string red = ambient->Attribute("R");
                        string green = ambient->Attribute("G");
                        string blue = ambient->Attribute("B");
                        m.setAmbient(Point(stof(red), stof(green), stof(blue)));
                    }
                    if (specular){
                        string red = specular->Attribute("R");
                        string green = specular->Attribute("G");
                        string blue = specular->Attribute("B");
                        m.setSpecular(Point(stof(red), stof(green), stof(blue)));
                    }
                    if (emissive){
                        string red = emissive->Attribute("R");
                        string green = emissive->Attribute("G");
                        string blue = emissive->Attribute("B");
                        m.setEmissive(Point(stof(red), stof(green), stof(blue)));
                    }
                    if (shininess){
                        string value = shininess->Attribute("value");
                        m.setShininess(stof(value));
                    }
                }
                this->models.push_back(m);
            }
        }

        /* Groups */
        for (XMLElement* gr = group->FirstChildElement("group"); gr != NULL; gr = gr->NextSiblingElement("group")) {
            Group g;
            g.readXML(gr, keys);
            this->subgroups.push_back(g);
        }
    }
}

/*
 * Free the memory allocated
 */
void Group::freeGroup() {
    for (auto transform : this->transformations) {
        delete transform;
    }
    this->transformations.clear();
}
