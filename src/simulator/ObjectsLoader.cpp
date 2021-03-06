///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: ObjectsLoader.cpp


#include "ObjectsLoader.h"
using namespace std;

void ObjectsLoader::loadRoad(const string fileName)
{
    cout << "Loading objects from " << fileName << "...  ";

    ifstream file;
    file.open(fileName.c_str());
    if (file.good())
    {
        vector<Cross*>crosses;

        while (!file.eof())
        {
            try
            {
                string data;
                getline(file, data);
                stringstream ss;
                ss << data;

                if (ss.fail()) throw ExceptionClass("failed to read line of text");

                string type;
                string id;

                ss >> type;
                if (ss.fail()) throw EmptyLineException();
                transform(type.begin(), type.end(), type.begin(), [] (unsigned char c) -> unsigned char {return toupper(c);});

                ss >> id;
                if (ss.fail()) throw ExceptionClass("failed to read object ID");

                if (findObjectByName(id) != nullptr) throw ExceptionClass("object with ID " + id + " already exists");

                if (type.compare("CR") == 0 || type.compare("CROSS") == 0 || type.compare("IN") == 0 || type.compare("INTERSECTION") == 0)
                {
                    float x1,y1,z1;

                    ss >> x1 >> y1 >> z1;
                    if (ss.fail()) throw ExceptionClass("failed to load position of intersection " + id);

                    Vec3 v1(x1,y1,z1);

                    Cross *temp;
                    temp = new Cross(v1);
                    temp->id = id;

                    loadedNewObject(temp);
                    crosses.push_back(temp);
                }
                else if (type.compare("ST") == 0 || type.compare("STREET") == 0)
                {
                    string begCrossID;
                    string endCrossID;
                    ss >> begCrossID >> endCrossID;
                    if (ss.fail()) throw ExceptionClass("failed to load info about street " + id);
                    Cross *crossB = nullptr;
                    Cross *crossE = nullptr;

                    for (unsigned int i=0;i<crosses.size();i++)
                    {
                        if (crosses[i]->id.compare(begCrossID) == 0)
                        {
                            crossB = crosses[i];
                            break;
                        }
                    }
                    if (crossB == nullptr) throw ExceptionClass("could not find intersection " + begCrossID + " for street " + id);

                    for (unsigned int i=0;i<crosses.size();i++)
                    {
                        if (crosses[i]->id.compare(endCrossID) == 0)
                        {
                            crossE = crosses[i];
                            break;
                        }
                    }
                    if (crossE == nullptr) throw ExceptionClass("could not find intersection " + begCrossID + " for street " + id);

                    GameObject *temp;
                    temp = new Street(crossB, crossE);
                    temp->id = id;

                    loadedNewObject(temp);
                }
                else if (type.compare("GA") == 0 || type.compare("GARAGE") == 0)
                {
                    string jointCross;
                    string vehType;
                    float x,y,z;
                    float spotFrec;
                    int maxVehicles;
                    ss >> vehType >> jointCross >> x >> y >> z >> spotFrec >> maxVehicles;
                    if (ss.fail()) throw ExceptionClass("failed to load info about garage " + id);

                    transform(vehType.begin(), vehType.end(), vehType.begin(), [] (unsigned char c) -> unsigned char {return toupper(c);});

                    Vec3 v(x,y,z);
                    Cross *cross = nullptr;
                    for(unsigned int i=0;i<crosses.size();i++)
                    {
                        if (crosses[i]->id.compare(jointCross) == 0)
                        {
                            cross = crosses[i];
                            break;
                        }
                    }
                    if (cross == nullptr) throw ExceptionClass("could not find intersection " + jointCross + " for garage " + id);

                    Garage *temp = nullptr;

                         if (vehType.compare("C") == 0 || vehType.compare("CAR") == 0) temp = new GarageCar(v, cross);
                    else if (vehType.compare("B") == 0 || vehType.compare("BUS") == 0) temp = new GarageBus(v, cross);

                    if (temp == nullptr) throw ExceptionClass("failed to create garage " + id + " of vehicle type " + vehType);

                    temp->id = id;
                    temp->maxVehicles = maxVehicles;
                    temp->frecSpot = spotFrec;

                    loadedNewObject(temp);
                    loadedNewFactory(temp);
                }
                else if (type.compare("CL") == 0 || type.compare("CROSSLIGHTS") == 0 || type.compare("IL") == 0 || type.compare("INTERSECTIONLIGHTS") == 0)
                {
                    float x1,y1,z1;
                    ss >> x1 >> y1 >> z1;
                    if (ss.fail()) throw ExceptionClass("failed to load info about intersection with lights " + id);
                    Vec3 v1(x1,y1,z1);

                    Cross *temp;
                    temp = new CrossLights(v1);
                    temp->id = id;

                    loadedNewObject(temp);
                    crosses.push_back(temp);
                }
                else
                {
                    throw ExceptionClass("could not find type " + type);
                }
            }
            catch (ExceptionClass e)
            {
                cout << endl << "ERROR while loading object: " << e.what() <<endl;
            }
            catch (EmptyLineException e)
            {
                //skipping an empty line; it's ok
            }
        }
    }
    else
    {
        throw ExceptionClass("failed to open file with objects (" + fileName + ")");
    }
    file.close();

    cout << "Success" << endl;
}

void ObjectsLoader::loadRightOfWay(const string fileName)
{
    cout << "Loading right of way from " << fileName << "...  ";

    ifstream file;
    file.open(fileName.c_str());
    if (file.good())
    {
        while (!file.eof())
        {
            try
            {
                string data;
                getline(file, data);
                stringstream ss;
                ss << data;

                string id;
                int number; //of streets in the cross

                ss >> id;
                if (ss.fail()) throw EmptyLineException();

                ss >> number;
                if (ss.fail() || number > 4 || number < 2) throw ExceptionClass("failed to read number of streets at intersection " + id);

                Driveable *ptrs[4];

                for(int i=0;i<number;i++)
                {
                    string ids;
                    ss >> ids;
                    if (ss.fail()) throw ExceptionClass("failed to load streets at intersection " + id);

                    ptrs[i] = dynamic_cast<Driveable*>(findObjectByName(ids));
                    if (ptrs[i] == nullptr) throw ExceptionClass("failed to get street " + ids + " at intersection " + id);
                }

                Cross *cross = dynamic_cast<Cross*>(findObjectByName(id));
                if (cross != nullptr)
                {
                    if (number != (int)cross->streets.size()) throw ExceptionClass("incorrect number of streets at intersection " + id);
                    cross->setDefaultPriority(ptrs[0], ptrs[1], ptrs[2], ptrs[3]);
                }
                else
                {
                    throw ExceptionClass("failed to find intersection " + id);
                }
            }
            catch (ExceptionClass e)
            {
                cout << "ERROR while loading right of way: " << e.what() << endl;
            }
            catch (EmptyLineException e)
            {
                //skipping an empty line; it's ok
            }
        }
    }
    else
    {
        throw ExceptionClass("failed to open file containing right of way (" + fileName + ")");
    }
    file.close();

    cout << "Success" << endl;
}
