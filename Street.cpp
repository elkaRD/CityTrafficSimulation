#include "Street.h"
#include<iostream>
using namespace std;

float Road::freeSpace(bool dir)
{
    if (dir)
    {
        //cout<<"test debug: "<<vehiclesBeg.size()<<endl;
        if (vehiclesBeg.size() == 0) return length;
        return length - vehiclesBeg.back()->xPos;
    }
    if (vehiclesEnd.size() == 0) return length;
    return length - vehiclesEnd.back()->xPos;
}

Cross::Cross(Vec3 position)// : GameObject(engine)
{
    pos = position;
    allowedVeh = 0;

    isSet = false;

    //cout<<(streets[0]==NULL)<<endl<<(streets[1]==NULL)<<endl;
    //cout<<streets[0].street<<endl;

}

void Cross::update(float delta)
{
    if(!isSet && /*id.compare("SK2") == 0*/ streets.size() == 4)
    {
        isSet = true;
        setDefaultPriority(streets[0].street, streets[1].street, streets[2].street, streets[3].street);
        cout<<id<<"   did happen?"<<endl;
    }

    if (!isSet && streets.size() == 2)
    {
        isSet = true;
        setDefaultPriority();
        cout<<"should happen"<<endl;
    }

    if (!isSet)
    {
        cout<<id<<"   "<<streets.size()<<endl;
    }

    /*cout<<"allowed vehs "<<id<<": " << allowedVeh<<endl;
    if (allowedVeh == 0)
    {
        int priority = -1;
        for(int i=0;i<streets.size();i++)
        {
            if (streets[i].vehicles.size() == 0)
            {
                int j = (i+1) % streets.size();
                while (streets[j].vehicles.size() == 0)
                {
                    if (i == j)
                    {
                        priority = -1;
                        break;
                    }
                    j = (j+1)%streets.size();
                }
                if (i == j) break;
                priority = (j) % streets.size();
                break;
            }
        }

        if (priority > 0)
        {
            //cout<<streets[priority].vehicles.size()<<endl;
            streets[priority].vehicles[0]->allowedToCross = true;
            streets[priority].vehicles.erase(streets[priority].vehicles.begin());
            allowedVeh=1;

            for(int i=0;i<streets[priority].vehicles.size();i++)
            {
                if (streets[priority].vehicles[i]->desiredTurn == streets[priority].vehicles[0]->desiredTurn)
                {
                    streets[priority].vehicles[i]->allowedToCross = true;
                    allowedVeh++;
                    //cout<<streets[priority].vehicles[i]->id<<"################################"<<streets[priority].vehicles[0]->id<<endl;
                    streets[priority].vehicles.erase(streets[priority].vehicles.begin()+i);
                    i--;
                }
                else break;
            }
        }
    }*/

    bool didAllow = false;

    //cout << id<< "    "<<allowedVeh<<endl;

    if ((allowedVeh == 0 || streets.size() <= 2) && isSet)
    {
        for (int i=0;i<streets.size();i++)
        {
            if (streets[i].vehicles.size() > 0)
            {
                if (streets[i].vehicles[0]->dstToCross > 0.8) continue;
                //if (streets[i].vehicles[0]->curCross == NULL) continue;
                //cout<<streets[i].vehicles[0]->id << "    "<<streets[i].vehicles[0]->allowedToCross<<endl;

                int which = streets[i].vehicles[0]->desiredTurn;
                //int which2 = streets[i].yield[which];
                bool isOK = true;

                cout<<"test "<<id<<"   "<<streets[i].vehicles[0]->id<<": "<<which<< "    " << streets[i].yield.size()<<endl;

                //if (which > 0)
                for (int j=0;j<streets[i].yield[which].size();j++)
                {
                    cout<<id<<"   "<<streets[i].vehicles[0]->id<<"   "<<j<<"    "<<streets[streets[i].yield[which][j]].vehicles.size()<<endl;

                    if (streets[streets[i].yield[which][j]].vehicles.size() > 0)
                    {
                        isOK = false;
                        break;
                    }
                }

                if (isOK)
                {
                    streets[i].vehicles[0]->allowedToCross = true;
                    streets[i].vehicles.erase(streets[i].vehicles.begin());
                    allowedVeh = 1;

                    for (int j=1;j<streets[i].vehicles.size();j++)
                    {
                        if (streets[i].vehicles[j]->desiredTurn == streets[i].vehicles[0]->desiredTurn)
                        {
                            streets[i].vehicles[j]->allowedToCross = true;
                            streets[i].vehicles.erase(streets[i].vehicles.begin());
                            allowedVeh++;
                        }
                        else break;
                    }
                }
            }
        }
    }
}

void Cross::setDefaultPriority(Road *s0, Road *s1, Road *s2, Road *s3)
{
    for(int i=0;i<streets.size();i++)
    {
        streets[i].yield.clear();
    }

    cout<<id<<"   number of streets: "<<streets.size()<<endl;

    if (streets.size() == 2)
    {
        vector<int> yield;
        vector<vector<int> > finalVec;
        finalVec.push_back(yield);
        finalVec.push_back(yield);

        streets[0].yield = finalVec;
        streets[1].yield = finalVec;

        cout<<id<<"   "<<streets[0].yield.size();
    }

    if (streets.size() == 4)
    {
        vector<int> yield[4];

        yield[0].push_back(1);
        yield[0].push_back(2);

        yield[1].push_back(1);
        yield[2].push_back(1);

        yield[3].push_back(1);
        yield[3].push_back(2);

        vector<vector<int> > finalVec;
        finalVec.push_back(yield[0]);
        finalVec.push_back(yield[1]);
        finalVec.push_back(yield[2]);
        finalVec.push_back(yield[3]);

        for (int i=0;i<4;i++)
        {
            //streets[i].yield.push_back()

            streets[i].yield = finalVec;

            for(int j=0;j<4;j++)
            {
                for(int k=0;k<finalVec[j].size();k++)
                {
                    //cout<<"DEbug: "<<finalVec[j][k]<<endl;
                    finalVec[j][k] = (finalVec[j][k]+1) % 4;
                }
            }
        }

        //debug
        /*for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                cout<<i<<" -> "<<j<<": ";
                for(int k=0;k<streets[i].yield[j].size();k++)
                {
                    cout<<streets[i].yield[j][k]<<" ";

                }
                cout<<endl;
            }
        }*/
    }


}

void glP(Vec3 x)
{
    glVertex3f(x.x,x.y,x.z);
}

void Cross::draw()
{
    setColor(1,0,0);
    drawCube(0.6, 0.1, 0.6);

}

Street::Street(Cross *begCross, Cross *endCross)
{
    crossBeg = begCross;
    crossEnd = endCross;

    length = Vec3::dst(crossBeg->getPos(), crossEnd->getPos());
    direction = crossEnd->getPos() - crossBeg->getPos();
    direction.normalize();

    Cross::OneStreet temp;
    temp.street = this;
    temp.enabled = true;
    temp.direction = true;
    temp.jointPos = crossBeg->getPos() + direction * 0.3;

    crossBeg->streets.push_back(temp);

    temp.direction = false;
    temp.jointPos = crossEnd->getPos() - direction * 0.3;
    crossEnd->streets.push_back(temp);

    begPos = crossBeg->getPos();
    endPos = crossEnd->getPos();



    normal = Vec3::cross(Vec3(0,1,0), direction);
    normal.normalize();
}

void Street::draw()
{
    setColor(0.3,0.3,0.3);
    drawLine(crossBeg->getPos(), crossEnd->getPos());

    Vec3 szer = Vec3::cross(Vec3(0,1,0), direction);
    szer.normalize();
    szer *= 0.3;

    Vec3 a = endPos + szer;
    Vec3 b = endPos - szer;
    Vec3 c = begPos + szer;
    Vec3 d = begPos - szer;

    //cout<<direction<<endl;

    glBegin(GL_POLYGON);
    glP(a);
    glP(b);
    glP(d);
    glP(c);
    //glP(a);
    glEnd();
}

Garage::Garage(Simulator *engine, Vec3 p, Cross *c)
{
    pos = p;
    crossEnd = c;
    crossBeg = NULL;

    begPos = pos;
    endPos = crossEnd->getPos();

    direction = endPos - begPos;
    direction.normalize();

    length = Vec3::dst(begPos, endPos);

    Cross::OneStreet temp;
    temp.street = this;
    temp.enabled = false;
    temp.direction = false;
    temp.jointPos = crossEnd->getPos() - direction * 0.3;
    cout<<"garage joint: "<<temp.jointPos<<endl;

    gameEngine = engine;

    crossEnd->streets.push_back(temp);



    normal = Vec3::cross(Vec3(0,1,0), direction);
    normal.normalize();

    curTime = 0;
    frec = 5;
}

void Garage::draw()
{
    setColor(0,0,1);
    drawCube(1);
}

void Garage::update(float delta)
{
    //cout<<id<<"   "<<curTime<<" "<<delta<<endl;

    //if (vehiclesBeg.size() > 0)cout<< vehiclesBeg.back()->xPos <<endl;
    //8cout<<"veh size: "<<vehiclesBeg.size()<<endl;
    if (vehiclesBeg.size() == 0 || (vehiclesBeg.size() > 0 && vehiclesBeg.back()->xPos > 1))
    curTime += delta;
    if (curTime > frec)
    {
        //cout<<"%%%%%%%"<<endl;
        curTime = 0;
        spotCar();
    }
}

void registerNewObject(Simulator *engine, GameObject *go);

string Garage::itos(int x)
{
    ostringstream ss;
    ss << x;
    return ss.str();
}

//int number = 0;
void Garage::spotCar()
{
    //number ++;
    //if (number > 3) return;
    Vehicle *temp;
    temp = new Car(this);
    temp->curRoad = this;
    temp->direction = true;
    temp->id = "CAR_" + itos(Vehicle::numVeh) + id;
    temp->isFirstVeh = vehiclesBeg.size() == 0;
    vehiclesBeg.push(temp);
//    gameEngine->registerObject(temp);
    registerNewObject(gameEngine, temp);

    cout<<"spotted " << temp->id << " by "<<id<<endl;
}
