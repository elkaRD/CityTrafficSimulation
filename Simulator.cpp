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
        //instance = new Simulator();
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
    glRotatef(cameraRot.y, 1,0,0);
    glRotatef(cameraRot.x, 0,1,0);

    glScalef(10, 10, 10);

    glTranslatef(-cameraPos.x, -cameraPos.y, -cameraPos.z);

    glScalef(1,1,-1);

    glPushMatrix();

    for (const auto &object : objects)
    {
        object->drawObject();
    }

    glPopMatrix();
}

/*Simulator::Simulator()
{
    if (!didInit)
    {
        //todo throw
    }

    cameraPos = Vec3(0,20,0);
    cameraRot = Vec3(180,70,0);

    cameraDirection = STAY;
}*/

Simulator::Simulator(int argc, char **argv)
{
    init(argc, argv);

    cameraPos = Vec3(0,20,0);
    cameraRot = Vec3(180,70,0);

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
            string data;
            getline(file, data);
            stringstream ss;
            ss << data;

            string type;
            string id;

            ss >> type >> id;

            if (type.compare("SK") == 0)
            {
                float x1,y1,z1;

                ss >> x1 >> y1 >> z1;
                Vec3 v1(x1,y1,z1);

                Cross *temp;
                temp = new Cross(v1);
                temp->id = id;
                objects.push_back(temp);
                crosses.push_back(temp);

                cout<<"dodano cross: "<<id<<endl;
            }
            if (type.compare("ST") == 0)
            {
                string begCrossID;
                string endCrossID;
                ss >> begCrossID >> endCrossID;
                Cross *crossB;
                Cross *crossE;

                for (unsigned int i=0;i<crosses.size();i++)
                {
                    if (crosses[i]->id.compare(begCrossID) == 0)
                    {
                        crossB = crosses[i];
                        break;
                    }
                }
                for (unsigned int i=0;i<crosses.size();i++)
                {
                    if (crosses[i]->id.compare(endCrossID) == 0)
                    {
                        crossE = crosses[i];
                        break;
                    }
                }

                GameObject *temp;
                temp = new Street(crossB, crossE);
                temp->id = id;
                objects.push_back(temp);

                cout<<"dodano street: "<<id<<endl;
            }
            if (type.compare("GA") == 0)
            {
                string jointCross;
                string vehType;
                float x,y,z;
                ss >> vehType >> jointCross >> x >> y >> z;
                Vec3 v(x,y,z);
                Cross *cross;
                for(unsigned int i=0;i<objects.size();i++)
                {
                    if (crosses[i]->id.compare(jointCross) == 0)
                    {
                        cross = crosses[i];
                        break;
                    }
                }
                Garage *temp;
                temp = new Garage(v, cross);
                temp->id = id;

                if (vehType.compare("C") == 0) temp->vehType = 0;
                if (vehType.compare("B") == 0) temp->vehType = 1;

                objects.push_back(temp);
                spots.push_back(temp);

                cout<<"dodano garage: "<<id<<endl;
            }
            if (type.compare("CL") == 0)
            {
                float x1,y1,z1;
                ss >> x1 >> y1 >> z1;
                Vec3 v1(x1,y1,z1);

                Cross *temp;
                temp = new CrossLights(v1);
                temp->id = id;
                objects.push_back(temp);
                crosses.push_back(temp);

                cout<<"dodano crossLights: "<<id<<endl;
            }
        }
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
            string name;//, mode;
            int number; //of streets in the cross

            file >> name;// >> mode;

            file >> number;

            string ids[4];

            for(int i=0;i<number;i++)
            {
                file >> ids[i];
            }

            Cross *cross = dynamic_cast<Cross*>(findObjectByName(name));
            if (cross != NULL)
            {
                cout<<"set priority on "<<cross->id<<endl;
                cross->setDefaultPriority(dynamic_cast<Driveable*>(findObjectByName(ids[0])),
                                          dynamic_cast<Driveable*>(findObjectByName(ids[1])),
                                          dynamic_cast<Driveable*>(findObjectByName(ids[2])),
                                          dynamic_cast<Driveable*>(findObjectByName(ids[3])));
            }
            else
            {
                cout<<"unable to set priority on "<<name<<endl;
            }
        }
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
