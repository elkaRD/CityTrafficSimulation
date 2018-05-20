#include "Street.h"
#include<iostream>
using namespace std;

class Simulator;

float Road::freeSpace(bool dir)
{
    if (dir)
    {
        if (vehiclesBeg.size() == 0) return length - reservedSpaceBeg;

        return vehiclesBeg.back()->xPos - reservedSpaceBeg;
    }

    if (vehiclesEnd.size() == 0) return length - reservedSpaceEnd;

    return vehiclesEnd.back()->xPos - reservedSpaceEnd;
}

Vec3 Road::getBegJoint(bool dir)
{
    if (dir)
    {
        return begJoint + normal * 0.1;
    }

    return begJoint - normal * 0.1;
}

Vec3 Road::getEndJoint(bool dir)
{
    if (dir)
    {
        return endJoint + normal * 0.1;
    }

    return endJoint - normal * 0.1;
}

Cross::Cross(Vec3 position)// : GameObject(engine)
{
    pos = position;
    allowedVeh = 0;

    isSet = false;
}

void Cross::update(float delta)
{
    updateCross(delta);
}

bool Cross::dontCheckStreet(int which)
{
    return false;
}

bool Cross::checkSet()
{
    if (isSet) return false;

    if(streets.size() == 4)
    {
        setDefaultPriority(streets[0].street, streets[1].street, streets[2].street, streets[3].street);
    }

    if (streets.size() == 2)
    {
        setDefaultPriority();
    }

    if (streets.size() == 3)
    {
        setDefaultPriority(streets[0].street, streets[1].street, streets[2].street);
    }

    return !isSet;
}

void Cross::updateCross(float delta)
{
    if (checkSet()) return;

    bool didAllow = false;
    //int smallestDstIndex = -1;
    //float smallestDstValue = -1;

    if (allowedVeh == 0 || streets.size() <= 2)
    {
        vector<int> indexesToPass;

        if (streets.size() == 2)
        {
            for(int i=0;i<streets.size();i++)
            {
                if (streets[i].vehicles.size() > 0)
                {
                    indexesToPass.push_back(i);
                }
            }
        }

        else if (streets.size() > 2)
        for (int i=0;i<streets.size();i++)
        {
            if (streets[i].vehicles.size() > 0)
            {
                if (dontCheckStreet(i)) continue;
                if (streets[i].vehicles[0]->dstToCross > 1.2) continue;

                /*if (streets[i].vehicles[0]->dstToCross < smallestDstValue || smallestDstIndex < 0)
                {
                    smallestDstIndex = i;
                    smallestDstValue = streets[i].vehicles[0]->dstToCross;
                }*/

                int which = streets[i].vehicles[0]->desiredTurn;
                bool isOK = true;

                for (int j=0;j<streets[i].yield[which].size();j++)
                {
                    //cout<<"YIELD:  "<<id<<"   "<<streets[i].vehicles[0]->id<<"   "<<j<<"    "<<streets[streets[i].yield[which][j]].vehicles.size()<<endl;

                    if (streets[streets[i].yield[which][j]].vehicles.size() > 0 && !dontCheckStreet(streets[i].yield[which][j]))
                    {
                        isOK = false;
                        break;
                    }
                }
                //cout<<endl;

                if (isOK)
                {
                    //cout<<"ALLOWED:  "<<streets[i].vehicles[0]->id<<endl;


                    didAllow = true;

                    //if (streets[i].vehicles[0]->dstToCross > 0.7) continue;//MAYBE

                    //if (streets[i].vehicles[0]->isEnoughSpace())

                    indexesToPass.push_back(i);
                }
            }
        }

        for (int i=0;i<indexesToPass.size();i++)
        {
            //cout<<streets[indexesToPass[i]].vehicles[0]->id<<endl;
            if (streets[indexesToPass[i]].vehicles[0]->isEnoughSpace())
            {
                streets[indexesToPass[i]].vehicles[0]->allowedToCross = true;
                streets[indexesToPass[i]].vehicles.erase(streets[indexesToPass[i]].vehicles.begin());
                allowedVeh++;
            }
        }


        if (allowedVeh == 0)
        {
            /*if (smallestDstIndex >= 0 && streets[smallestDstIndex].vehicles[0]->isEnoughSpace())
            {
                streets[smallestDstIndex].vehicles[0]->allowedToCross = true;
                streets[smallestDstIndex].vehicles.erase(streets[smallestDstIndex].vehicles.begin());
                allowedVeh++;
                didAllow = true;
            }*/

            if (allowedVeh == 0)
            {
                for(int i=0;i<streets.size();i++)
                {
                    if (dontCheckStreet(i)) continue;
                    if (streets[i].vehicles.size() > 0 && streets[i].vehicles[0]->dstToCross < 0.7)
                    {
                        if (streets[i].vehicles[0]->isEnoughSpace())
                        {
                            //cout<<"ALLOWED2:   "<<streets[i].vehicles[0]->id<<"    "<<streets[i].vehicles[0]->dstToCross<<endl;
                            //int t;cin>>t;
                            /*for(int j=0;j<streets.size();j++)
                            {
                                cout<<streets[j].street->id<<"   "<<streets[j].vehicles.size()<<endl;
                            }*/

                            streets[i].vehicles[0]->allowedToCross = true;
                            streets[i].vehicles.erase(streets[i].vehicles.begin());
                            allowedVeh++;
                            didAllow = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Cross::setDefaultPriority(Road *s0, Road *s1, Road *s2, Road *s3)
{
    isSet = true;

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

    if (streets.size() == 3)
    {
        vector<int> yield0;
        yield0.push_back(1);
        vector<int> yield0empty;
        vector<vector<int> > finalVec0;
        finalVec0.push_back(yield0);
        finalVec0.push_back(yield0empty);
        finalVec0.push_back(yield0);

        vector<int> yield1;
        vector<int> yield1empty;
        yield1.push_back(2);
        vector<vector<int> > finalVec1;
        finalVec1.push_back(yield1);
        finalVec1.push_back(yield1);
        finalVec1.push_back(yield1empty);

        vector<int> yield2;
        vector<int> yield2empty;
        yield2.push_back(0);
        vector<vector<int> >finalVec2;
        finalVec2.push_back(yield2empty);
        finalVec2.push_back(yield2);
        //yield2.push_back(1);
        finalVec2.push_back(yield2);

        for(int i=0;i<3;i++)
        {
            if (streets[i].street == s0) streets[i].yield = finalVec0;
            if (streets[i].street == s1) streets[i].yield = finalVec1;
            if (streets[i].street == s2) streets[i].yield = finalVec2;
        }
    }

    if (streets.size() == 4)
    {
        vector<int> yield[4];

        yield[0].push_back(1);
        yield[0].push_back(2);

        //yield[1].push_back(1);
        yield[2].push_back(1);

        yield[3].push_back(1);
        yield[3].push_back(2);

        vector<vector<int> > finalVec;
        //finalVec.push_back(yield[0]);
        //finalVec.push_back(yield[1]);
        //finalVec.push_back(yield[2]);
        //finalVec.push_back(yield[3]);

        for (int i=0;i<4;i++)
        {
            if (streets[i].street == s0) finalVec.push_back(yield[0]);
            if (streets[i].street == s1) finalVec.push_back(yield[1]);
            if (streets[i].street == s2) finalVec.push_back(yield[2]);
            if (streets[i].street == s3) finalVec.push_back(yield[3]);
        }

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
    additionalSetup(s0, s1, s2, s3);

}

Vec3 Cross::OneStreet::getJointPos()
{
    if (!direction) return street->endJoint;
    return street->begJoint;
}

void Cross::additionalSetup(Road *s0, Road *s1, Road *s2, Road *s3)
{

}

void CrossLights::additionalSetup(Road *s0, Road *s1, Road *s2, Road *s3)
{
    setDefaultLights(s0, s1, s2, s3);
}

bool CrossLights::dontCheckStreet(int which)
{
    //cout<<"DEBUG  SEGFAULT   "<<id<<"   "<<curPriority.size()<<"  "<<which<<endl;
    return !curPriority[which];
}

void CrossLights::setDefaultLights(Road *s0, Road *s1, Road *s2, Road *s3)
{
    defaultPriority.clear();
    curPriority.clear();

    for (int i=0;i<streets.size();i++)
    {
        defaultPriority.push_back(false);
        curPriority.push_back(false);
    }

    if (streets.size() == 3)
    {
        //defaultPriority[0] = true;
        for (int i=0;i<streets.size();i++)
        {
            if (streets[i].street == s0) defaultPriority[i] = true;
        }
    }

    if (streets.size() == 4)
    {
        //defaultPriority[0] = true;
        //defaultPriority[2] = true;
        for (int i=0;i<streets.size();i++)
        {
            if (streets[i].street == s0) defaultPriority[i] = true;
            if (streets[i].street == s2) defaultPriority[i] = true;
        }
    }

    cout<<id<<"  SET DEFAULT LIGHTS:   "<<curPriority.size()<<endl;
}

void CrossLights::setLightsPriority()
{
    if (curState == G1 || curState == Y1)
    {
        for(int i=0;i<streets.size();i++)
        {
            curPriority[i] = defaultPriority[i];
        }
    }

    if (curState == B1 || curState == B2)
    {
        for(int i=0;i<streets.size();i++)
        {
            curPriority[i] = false;
        }
    }

    if (curState == G2 || curState == Y2)
    {
        for(int i=0;i<streets.size();i++)
        {
            curPriority[i] = !defaultPriority[i];
        }
    }
}

void glP(Vec3 x)
{
    glVertex3f(x.x,x.y,x.z);
}

void Cross::draw()
{
    setColor(0.1,0.1,0.1);
    drawCube(0.6, 0.01, 0.6);

}

CrossLights::CrossLights(Vec3 position) : Cross(position)
{
    durationGreen1 = randFloat(10, 20);
    durationYellow1 = randFloat(2, 6);
    durationGreen2 = randFloat(10, 20);
    durationYellow2 = randFloat(2, 6);
    durationBreak = randFloat(1, 6);

    curState = G1;
    curTime = durationGreen1;
}

void CrossLights::getNextState()
{
    if (curTime <= 0)
    {
        if (curState == G1)
        {
            curState = Y1;
            curTime = durationYellow1;
            setLightsPriority();
            return;
        }
        if (curState == Y1)
        {
            curState = B1;
            curTime = durationBreak;
            setLightsPriority();
            return;
        }
        if (curState == B1)
        {
            curState = G2;
            curTime = durationGreen2;
            setLightsPriority();
            return;
        }
        if (curState == G2)
        {
            curState = Y2;
            curTime = durationYellow2;
            setLightsPriority();
            return;
        }
        if (curState == Y2)
        {
            curState = B2;
            curTime = durationBreak;
            setLightsPriority();
            return;
        }
        if (curState == B2)
        {
            curState = G1;
            curTime = durationGreen1;
            setLightsPriority();
            return;
        }
    }
}

void CrossLights::update(float delta)
{
    //Cross:update(delta);
    updateCross(delta);



    curTime -= delta;
    getNextState();
    //cout<<"STATE:    "<<curState<<"   "<<curTime<<endl;
}

void CrossLights::draw()
{
    Cross::draw();

    //cout<<".";
    Vec3 color1;
    Vec3 color2;

    if (curState == G1)
    {
        color1 = Vec3(0,1,0);
        color2 = Vec3(1,0,0);
        //setColor(0,1,0);
        //drawCube(0.5);
    }
    if (curState == G2)
    {
        color1 = Vec3(1,0,0);
        color2 = Vec3(0,1,0);
        //setColor(1,0,0);
        //drawCube(0.5);
    }
    if (curState == Y1)
    {
        color1 = Vec3(1,1,0);
        color2 = Vec3(1,0,0);
    }

    if (curState == Y2)
    {
        color1 = Vec3(1,0,0);
        color2 = Vec3(1,1,0);
    }
    if (curState == B1 || curState == B2 )
    {
        color1 = Vec3(1,0,0);
        color2 = Vec3(1,0,0);
        //setColor(1,1,0);
        //drawCube(0.5);
    }

    glTranslatef(-pos.x,-pos.y,-pos.z);

    for(int i =0;i<streets.size();i++)
    {
        glPushMatrix();
        glTranslatef(0,0.35,0);
        //glPushMatrix();
        //glScalef(0.4,0.4,0.4);
        if (!streets[i].direction)
            glTranslatef(streets[i].street->normal.x/10.0,streets[i].street->normal.y/10.0,streets[i].street->normal.z/10.0);
        else
            glTranslatef(-streets[i].street->normal.x/10.0,-streets[i].street->normal.y/10.0,-streets[i].street->normal.z/10.0);
            //glPopMatrix();
        glTranslatef(streets[i].getJointPos().x,streets[i].getJointPos().y,streets[i].getJointPos().z);
        if (defaultPriority[i]) setColor(color1.x, color1.y, color1.z);
        else setColor(color2.x, color2.y, color2.z);

        if (streets[i].direction)
            glRotatef(streets[i].street->normal.angleXZ(), 0,1,0);
        else
            glRotatef(streets[i].street->normal.angleXZ() + 180, 0,1,0);

        drawCube(0.05,0.1,0.05);
        //glPopMatrix();

        /*if (!streets[i].direction)
            glTranslatef(streets[i].street->normal.x/5.0,streets[i].street->normal.y/5.0,streets[i].street->normal.z/5.0);
        else
            glTranslatef(-streets[i].street->normal.x/5.0,-streets[i].street->normal.y/5.0,-streets[i].street->normal.z/5.0);*/

        glTranslatef(-0.2,0,0);

        setColor(0,0,0);
        glTranslatef(0,-0.35/2,0);
        drawCube(0.025,0.35,0.025);

        glTranslatef(0.1,0.35/2,0);
        drawCube(0.2,0.025,0.025);

        glPopMatrix();
    }
}

/*void CrossLights::setDefaultPriority(Road *s0, Road *s1, Road *s2, Road *s3)
{
    //Cross:setDefaultPriority(s0,s1,s2,s3);
}*/

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
    //temp.jointPos = crossBeg->getPos() + direction * 0.3;
    begJoint = crossBeg->getPos() + direction * 0.3;;

    crossBeg->streets.push_back(temp);

    temp.direction = false;
    //temp.jointPos = crossEnd->getPos() - direction * 0.3;
    endJoint = crossEnd->getPos() - direction * 0.3;
    crossEnd->streets.push_back(temp);

    begPos = crossBeg->getPos();
    endPos = crossEnd->getPos();

    reservedSpaceBeg = 0;
    reservedSpaceEnd = 0;

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
    //temp.jointPos = crossEnd->getPos() - direction * 0.3;

    begJoint = begPos;
    endJoint = crossEnd->getPos() - direction * 0.3;

    //cout<<"garage joint: "<<temp.jointPos<<endl;

    gameEngine = engine;

    crossEnd->streets.push_back(temp);

    reservedSpaceBeg = 0;
    reservedSpaceEnd = 0;

    normal = Vec3::cross(Vec3(0,1,0), direction);
    normal.normalize();

    curTimeSpot = 0;
    frecSpot = 4;

    curTimeDelete = 0;
    frecDelete = 6;
}

void Garage::draw()
{
    setColor(0,0,1);
    drawCube(0.3);

    setColor(0.3,0.3,0.3);
    //drawLine(crossBeg->getPos(), crossEnd->getPos());

    Vec3 szer = Vec3::cross(Vec3(0,1,0), direction);
    szer.normalize();
    szer *= 0.3;

    Vec3 a = endPos + szer;
    Vec3 b = endPos - szer;
    Vec3 c = begPos + szer;
    Vec3 d = begPos - szer;

    //cout<<direction<<endl;

    glTranslatef(-pos.x,-pos.y,-pos.z);

    glBegin(GL_POLYGON);
    glP(a);
    glP(b);
    glP(d);
    glP(c);
    //glP(a);
    glEnd();
}

void Garage::update(float delta)
{
    //cout<<id<<"   "<<curTime<<" "<<delta<<endl;

    //if (vehiclesBeg.size() > 0)cout<< vehiclesBeg.back()->xPos <<endl;
    //8cout<<"veh size: "<<vehiclesBeg.size()<<endl;
    if (vehiclesBeg.size() == 0 || (vehiclesBeg.size() > 0 && vehiclesBeg.back()->xPos > 1))
        curTimeSpot += delta;

    if (curTimeSpot > frecSpot)
    {
        //cout<<"%%%%%%%"<<endl;
        curTimeSpot = 0;
        spotVeh();
    }

    if (vehiclesEnd.size() > 0)
        curTimeDelete += delta;

    if (curTimeDelete > frecDelete)
    {
        curTimeDelete = 0;
        deleteVeh();
    }
}

void registerNewObject(Simulator *engine, GameObject *go);
void destroyNextObject(Simulator *engine, GameObject *go);

string Garage::itos(int x)
{
    ostringstream ss;
    ss << x;
    return ss.str();
}

int number = 0;
void Garage::spotVeh()
{
    number ++;
    //if (number > 2) return;
    Vehicle *temp;
    if (vehType == 0)
    {
        temp = new Car(this);
        temp->id = "CAR_" + itos(Vehicle::numVeh);
    }
    else
    {
        temp = new Bus(this);
        temp->id = "BUS_" + itos(Vehicle::numVeh);
    }
    temp->curRoad = this;
    temp->direction = true;

    temp->isFirstVeh = vehiclesBeg.size() == 0;
    temp->idnumber = Vehicle::numVeh;
    vehiclesBeg.push(temp);
//    gameEngine->registerObject(temp);
    registerNewObject(gameEngine, temp);

    cout<<"spotted " << temp->id << " by "<<id<<endl;
}

void Garage::deleteVeh()
{
    if (vehiclesEnd.size() > 0)
    {
        Vehicle *temp = vehiclesEnd.front();
        vehiclesEnd.pop();

        if(temp->backVeh != NULL)
        {
            temp->backVeh->isFirstVeh = true;
            temp->backVeh->frontVeh = NULL;
        }

        destroyNextObject(gameEngine, temp);
    }
}
