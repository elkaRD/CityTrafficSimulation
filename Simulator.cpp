///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Simulator.cpp


#include"Simulator.h"
using namespace std;

Simulator* Simulator::instance = NULL;

Simulator* Simulator::getInstance()
{
    if (instance == NULL)
    {
        throw "Symulator nie zostal zainicjonowany";
    }
    return instance;
}

Simulator* Simulator::getInstance(int argc, char **argv)
{
    if (instance == NULL)
    {
        instance = new Simulator(argc, argv);
    }

    return getInstance();
}

void Simulator::run()
{
    EngineCore::run();
}

void Simulator::redraw()
{
    rotateX(cameraRot.y);
    rotateY(cameraRot.x);

    scale(10, 10, 10);

    translate(-cameraPos);

    scale(1, 1, -1);

    pushMatrix();

    for (const auto &object : objects)
    {
        object->drawObject();
    }

    popMatrix();
}

Simulator::Simulator(int argc, char **argv)
{
    init(argc, argv);

    cameraPos = Vec3(12.3, 2.4, 16.3);
    cameraRot = Vec3(-36.2, 9.8, 0);

    cameraDirection = STAY;
}

void Simulator::keyPressed(char k)
{
    if (k == 27)
    {
        cleanSimulation();
        exit(0);
    }

    if (k >= 'A' && k <= 'Z')
    {
        k += 32;
        cameraVelocity = CAMERA_VELOCITY * 5.0f;
    }
    else
    {
        cameraVelocity = CAMERA_VELOCITY;
    }

    switch (k)
    {
        case 'w': cameraDirection = FORWARD;    break;
        case 's': cameraDirection = BACK;       break;
        case 'a': cameraDirection = LEFT;       break;
        case 'd': cameraDirection = RIGHT;      break;
        case 'e': cameraDirection = UP;         break;
        case 'q': cameraDirection = DOWN;       break;
        default:  cameraDirection = STAY;
    }
}

void Simulator::cameraMove(const float delta)
{
    float multiplyMove = cameraVelocity * delta;
    float multiplyMoveHorizontal = cos(-cameraRot.y * M_PI / 180) * multiplyMove;

    switch (cameraDirection)
    {
    case FORWARD:
        cameraPos.z -=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x +=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y +=sin((-cameraRot.y)*M_PI/180)*multiplyMove;
        break;

    case BACK:
        cameraPos.z +=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x -=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y -=sin((-cameraRot.y)*M_PI/180)*multiplyMove;
        break;

    case LEFT:
        cameraPos.z +=cos( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
        cameraPos.x -=sin( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
        break;

    case RIGHT:
        cameraPos.z -=cos( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
        cameraPos.x +=sin( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
        break;

    case UP:
        cameraPos.z -=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x +=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y +=sin((-cameraRot.y + 90)*M_PI/180)*multiplyMove;
        break;

    case DOWN:
        cameraPos.z +=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x -=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y +=sin((-cameraRot.y - 90)*M_PI/180)*multiplyMove;
        break;

    default: break;
    }

    cameraDirection = STAY;
}

void Simulator::mouseMove(const int dx, const int dy)
{
    cameraRot.x += dx * 0.2;
    cameraRot.y += dy * 0.2;

    if (cameraRot.y > 90) cameraRot.y = 90;
    else if (cameraRot.y < -90) cameraRot.y = -90;
}

void Simulator::loadRoad(const string fileName)
{
    fstream file;
    file.open(fileName.c_str(), ios::app | ios::in);
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

                if (ss.fail()) throw "blad wczytywania linijki tekstu";

                string type;
                string id;

                ss >> type;
                if (ss.fail()) throw 0;

                ss >> id;
                if (ss.fail()) throw "nie udalo sie wczytac wybranego id obiektu";

                if (findObjectByName(id) != NULL) throw "obiekt o id " + id + " juz istnieje";

                if (type.compare("CR") == 0 || type.compare("CROSS") == 0)
                {
                    float x1,y1,z1;

                    ss >> x1 >> y1 >> z1;
                    if (ss.fail())
                    {
                        throw "nie udalo sie wczytac pozycji skrzyzowania " + id;
                    }
                    Vec3 v1(x1,y1,z1);

                    Cross *temp;
                    temp = new Cross(v1);
                    temp->id = id;
                    objects.push_back(temp);
                    crosses.push_back(temp);

                    //cout<<"dodano cross: "<<id<<endl;
                }
                else if (type.compare("ST") == 0 || type.compare("STREET") == 0)
                {
                    string begCrossID;
                    string endCrossID;
                    ss >> begCrossID >> endCrossID;
                    if (ss.fail()) throw "nie udalo sie wczytac info o ulicy " + id;
                    Cross *crossB = NULL;
                    Cross *crossE = NULL;

                    for (unsigned int i=0;i<crosses.size();i++)
                    {
                        if (crosses[i]->id.compare(begCrossID) == 0)
                        {
                            crossB = crosses[i];
                            break;
                        }
                    }
                    if (crossB == NULL) throw "nie znaleziono skrzyzowania " + begCrossID + " dla ulicy " + id;

                    for (unsigned int i=0;i<crosses.size();i++)
                    {
                        if (crosses[i]->id.compare(endCrossID) == 0)
                        {
                            crossE = crosses[i];
                            break;
                        }
                    }
                    if (crossE == NULL) throw "nie znaleziono skrzyzowania " + endCrossID + " dla ulicy " + id;

                    GameObject *temp;
                    temp = new Street(crossB, crossE);
                    temp->id = id;
                    objects.push_back(temp);

                    //cout<<"dodano street: "<<id<<endl;
                }
                else if (type.compare("GA") == 0 || type.compare("GARAGE") == 0)
                {
                    string jointCross;
                    string vehType;
                    float x,y,z;
                    float spotFrec;
                    int maxVehicles;
                    ss >> vehType >> jointCross >> x >> y >> z >> spotFrec >> maxVehicles;
                    if (ss.fail()) throw "nie udalo sie wczytac danych dla garazu " + id;

                    Vec3 v(x,y,z);
                    Cross *cross = NULL;
                    for(unsigned int i=0;i<objects.size();i++)
                    {
                        if (crosses[i]->id.compare(jointCross) == 0)
                        {
                            cross = crosses[i];
                            break;
                        }
                    }
                    if (cross == NULL) throw "nie udalo sie znalezc skrzyzowania " + jointCross + " dla garazu " + id;

                    Garage *temp;
                    temp = new Garage(v, cross);
                    temp->id = id;
                    temp->maxVehicles = maxVehicles;
                    temp->frecSpot = spotFrec;

                    if (vehType.compare("C") == 0) temp->vehType = 0;
                    if (vehType.compare("B") == 0) temp->vehType = 1;

                    objects.push_back(temp);
                    spots.push_back(temp);

                    //cout<<"dodano garage: "<<id<<endl;
                }
                else if (type.compare("CL") == 0 || type.compare("CROSSLIGHTS") == 0)
                {
                    float x1,y1,z1;
                    ss >> x1 >> y1 >> z1;
                    if (ss.fail()) throw "nie udalo sie wczytac danych dla skrzyzowania ze swiatlami " + id;
                    Vec3 v1(x1,y1,z1);

                    Cross *temp;
                    temp = new CrossLights(v1);
                    temp->id = id;
                    objects.push_back(temp);
                    crosses.push_back(temp);

                    //cout<<"dodano crossLights: "<<id<<endl;
                }
                else
                {
                    throw "nie udalo sie znalezc typu " + type;
                }
            }
            catch (string e)
            {
                cout << "ERROR podczas wczytywania obiektu: " << e <<endl;
            }
            catch (int e)
            {

            }
        }
    }
    else
    {
        throw "nie udalo sie otworzyc pliku z obiektami";
    }
    file.close();
}

void Simulator::loadPriority(const string fileName)
{
    fstream file;
    file.open(fileName.c_str(), ios::app | ios::in);
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
                if (ss.fail()) throw 0;

                ss >> number;
                if (ss.fail() || number > 4 || number < 2) throw "blad w czytaniu ilosci ulic na skrzyzowaniu " + id;

                Driveable *ptrs[4];

                for(int i=0;i<number;i++)
                {
                    string ids;
                    ss >> ids;
                    if (ss.fail()) throw "nie udalo sie wczytac ulic na skrzyzowaniu " + id;

                    ptrs[i] = dynamic_cast<Driveable*>(findObjectByName(ids));
                    if (ptrs[i] == NULL) throw "blad wczytywania ulicy " + ids + " na skrzyzowaniu " + id;
                }

                Cross *cross = dynamic_cast<Cross*>(findObjectByName(id));
                if (cross != NULL)
                {
                    if (number != (int)cross->streets.size()) throw "niepoprawna ilosc ulic na " + id;
                    cross->setDefaultPriority(ptrs[0], ptrs[1], ptrs[2], ptrs[3]);
                }
                else
                {
                    throw "nie udalo sie znalezc skrzyzowania " + id;
                }
            }
            catch (int e)
            {

            }
            catch (string e)
            {
                cout << "ERROR podczas wczytywania pierwszenstwa: " << e << endl;
            }
        }
    }
    else
    {
        throw "nie udalo sie otworzyc pliku z pierwszenstwem";
    }
    file.close();
}

void Simulator::singleUpdate(const float delta)
{
    cameraMove(delta);
}

void Simulator::update(const float delta)
{
    for (auto &spot : spots)
    {
        if (spot->checkReadyToSpot())
        {
            registerObject(spot->spotVeh());
        }
        if (spot->checkReadyToDelete())
        {
            destroyObject(spot->deleteVeh());
        }
    }

    for (unsigned int i=0;i<objects.size();i++)
    {
        if (objects[i] == NULL)
        {
            objects.erase(objects.begin() + i);
            continue;
        }
        objects[i]->updateObject(delta);
    }
}

void Simulator::registerObject(GameObject *go)
{
    objects.push_back(go);
}

void Simulator::destroyObject(GameObject *go)
{
    for(unsigned int i=0;i<objects.size();i++)
    {
        if (objects[i] == go)
        {
            objects.erase(objects.begin() + i);
            break;
        }
    }
}

void Simulator::cleanSimulation()
{
    int i = objects.size();

    while (i > 0)
    {
        destroyObject(*objects.begin());
        i--;
    }
}

GameObject* Simulator::findObjectByName(const string no) const
{
    for (const auto &object : objects)
    {
        if (object->id.compare(no) == 0)
        {
            return object;
        }
    }

    return NULL;
}
