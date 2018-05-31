#include"Simulator.h"
using namespace std;

void Simulator::redraw()
{
    glRotatef(cameraRot.y, 1,0,0);
    glRotatef(cameraRot.x, 0,1,0);

    glScalef(10, 10, 10);

    glTranslatef(-cameraPos.x, -cameraPos.y, -cameraPos.z);

    glScalef(1,1,-1);

    glPushMatrix();

    for (unsigned int i=0;i<objects.size();i++)
    {
        objects[i]->drawObject();
    }

    glPopMatrix();
}

Simulator::Simulator()
{
    if (!didInit)
    {
        //todo throw
    }

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
        default:  cameraDirection = STAY;
    }
}

void Simulator::cameraMove(float delta)
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

        break;

    case RIGHT:

        break;
    }

    cameraDirection = STAY;
}

void Simulator::mouseMove(int dx, int dy)
{
    cameraRot.x += dx * 0.2;
    cameraRot.y += dy * 0.2;

    if (cameraRot.y > 90) cameraRot.y = 90;
    if (cameraRot.y < -90) cameraRot.y = -90;

    cout<<cameraRot.x<<"   "<<cameraRot.y<<endl;
}

void Simulator::moveCamera(float delta)
{
    float multiplyMove=CAMERA_VELOCITY;
    float multiplyMoveHorizontal;
    if(isKeyPressed(XK_Shift_L))multiplyMove *= 5;

    multiplyMove *= delta;
    multiplyMoveHorizontal = cos(-cameraRot.y * M_PI / 180) * multiplyMove;

    if(isKeyPressed('W'))
    {
        cout<<"UP:  "<<cos( cameraRot.x * M_PI / 180 )<<"    "<<sin( cameraRot.x * M_PI / 180 )<<endl;
        //cout<<"DEBUG POS: "<<cameraPos<<"   "<<tan((-cameraRot.y)*M_PI/180)*multiplyMove;endl;
        cameraPos.z -=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x +=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y +=sin((-cameraRot.y)*M_PI/180)*multiplyMove;
    }
    if(isKeyPressed('S'))
    {
        cout<<"DOWN"<<endl;
        cameraPos.z+=cos( cameraRot.x * M_PI / 180 )*multiplyMove;
        cameraPos.x-=sin( cameraRot.x * M_PI / 180 )*multiplyMove;
        cameraPos.y+=atan(cameraRot.y*M_PI/180)*multiplyMove;
    }
    if(isKeyPressed('A'))
    {
        cout<<"LEFT"<<endl;
        cameraPos.z+=cos( (cameraRot.x+270) * M_PI / 180 )*multiplyMove;
        cameraPos.x-=sin( (cameraRot.x+270) * M_PI / 180 )*multiplyMove;
    }
    if(isKeyPressed('D'))
    {
        cout<<"RIGHT"<<endl;
        cameraPos.z+=cos( (cameraRot.x+90) * M_PI / 180 )*multiplyMove;
        cameraPos.x-=sin( (cameraRot.x+90) * M_PI / 180 )*multiplyMove;
    }
    if(isKeyPressed('Q'))
    {

    }
    if(isKeyPressed('E'))
    {

    }
}

void Simulator::moveCamera2(char c)
{
    float multiplyMove=100;

    cout<<cameraPos<<endl;

    if(c=='W' || c=='w')
    {
        cout<<"UP  "<<cos( cameraRot.x * M_PI / 180 )<<endl;
        cameraPos.z+=cos( cameraRot.x * M_PI / 180 )/10*multiplyMove;
        cameraPos.x-=sin( cameraRot.x * M_PI / 180 )/10*multiplyMove;
        cameraPos.y+=atan(cameraRot.y*M_PI/180)/10*multiplyMove;

        cout<<cameraPos<<endl;
    }
    if(c=='S'||c=='s')
    {
        cameraPos.z+=cos( cameraRot.x * M_PI / 180 )/10*multiplyMove*-1;
        cameraPos.x-=sin( cameraRot.x * M_PI / 180 )/10*multiplyMove*-1;
        cameraPos.y-=atan(cameraRot.y*M_PI/180)/10*multiplyMove;
    }
    if(c=='A'||c=='a')
    {
        cameraPos.z+=cos( (cameraRot.x+270) * M_PI / 180 )/10*multiplyMove;
        cameraPos.x-=sin( (cameraRot.x+270) * M_PI / 180 )/10*multiplyMove;
    }
    if(c=='D'||c=='d')
    {
        cameraPos.z+=cos( (cameraRot.x+90) * M_PI / 180 )/10*multiplyMove;
        cameraPos.x-=sin( (cameraRot.x+90) * M_PI / 180 )/10*multiplyMove;
    }
}

bool Simulator::isKeyPressed(long k)
{
    if (k<256)
    {
        return pressedKey[k];
    }

    if (k == XK_Shift_L) return pressedShift;

    return false;
}




void Simulator::loadRoad(string fileName)
{
    fstream file;
    file.open(fileName.c_str(), ios::app | ios::in);
    if (file.good())
    {
        /*string data;
        getline(file, data);

        stringstream ss;
        ss << data;*/

        vector<Cross*>crosses;

        while (!file.eof())
        {
            string type;
            string id;
            file >> type >> id;
            if (type.compare("SK") == 0)
            {
                float x1,y1,z1;
                //float x2,y2,z2;
                file >> x1 >> y1 >> z1;// >> x1 >> y2 >> z2;
                Vec3 v1(x1,y1,z1);
                //Vec3 v2(x2,y2,z2);

                Cross *temp;
                temp = new Cross(v1);
                temp->gameEngine = this;
                temp->id = id;
                objects.push_back(temp);
                crosses.push_back(temp);

                cout<<"dodano cross: "<<id<<endl;
            }
            if (type.compare("ST") == 0)
            {
                string begCrossID;
                string endCrossID;
                file >> begCrossID >> endCrossID;
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
                temp->gameEngine = this;
                temp->id = id;
                objects.push_back(temp);

                cout<<"dodano street: "<<id<<endl;
            }
            if (type.compare("GA") == 0)
            {
                string jointCross;
                string vehType;
                float x,y,z;
                file >> vehType >> jointCross >> x >> y >> z;
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
                temp = new Garage(this, v, cross);
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
                //float x2,y2,z2;
                file >> x1 >> y1 >> z1;// >> x1 >> y2 >> z2;
                Vec3 v1(x1,y1,z1);
                //Vec3 v2(x2,y2,z2);

                Cross *temp;
                temp = new CrossLights(v1);
                temp->gameEngine = this;
                temp->id = id;
                objects.push_back(temp);
                crosses.push_back(temp);

                cout<<"dodano crossLights: "<<id<<endl;
            }
        }
    }
    file.close();
}

void Simulator::loadPriority(string fileName)
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

            //if (mode.compare("DEFAULT") == 0)
            {
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
    }
    file.close();
}

void Simulator::singleUpdate(float delta)
{
    cameraMove(delta);
}

void Simulator::update(float delta)
{
    for (unsigned int i = 0; i < spots.size(); i++)
    {
        if (spots[i]->isReadyToSpot)
        {
            registerObject(spots[i]->spotVeh());
        }
        if (spots[i]->isReadyToDelete)
        {
            destroyObject(spots[i]->deleteVeh());
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
            //cout<<"TEST DEST   "<<go->id<<endl;
            //delete go;
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
        //objects.erase(objects.begin());
        destroyObject(*objects.begin());
        i--;
    }

}

/*void registerNewObject(Simulator *engine, GameObject *go)
{
    engine->registerObject(go);
}

void destroyNextObject(Simulator *engine, GameObject *go)
{
    engine->destroyObject(go);
}*/

GameObject* Simulator::findObjectByName(string no)
{
    for(unsigned int i=0;i<objects.size();i++)
    {
        if (objects[i]->id.compare(no) == 0)
        {
            return objects[i];
        }
    }

    return NULL;
}
