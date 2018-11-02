///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Road.cpp


#include "Road.h"
using namespace std;

class Simulator;

Vec3 Road::roadColor = Vec3(0.2, 0.2, 0.2);

float Driveable::getLength() const
{
    return length;
}

Driveable::Driveable(Cross *begCross, Cross *endCross)
{
    crossBeg = begCross;
    crossEnd = endCross;

    begPos = crossBeg->getPos();
    endPos = crossEnd->getPos();

    commonConstructor();

    Cross::OneStreet temp;
    temp.street = this;

    temp.direction = true;
    begJoint = crossBeg->getPos() + direction * 0.3;;
    crossBeg->streets.push_back(temp);

    temp.direction = false;
    endJoint = crossEnd->getPos() - direction * 0.3;
    crossEnd->streets.push_back(temp);
}

Driveable::Driveable(Vec3 p, Cross *endCross)
{
    crossBeg = nullptr;
    crossEnd = endCross;

    pos = p;

    begPos = pos;
    endPos = crossEnd->getPos();

    commonConstructor();

    Cross::OneStreet temp;
    temp.street = this;
    temp.direction = false;
    begJoint = begPos;
    endJoint = crossEnd->getPos() - direction * 0.3;
    crossEnd->streets.push_back(temp);
}

void Driveable::commonConstructor()
{
    direction = endPos - begPos;
    direction.normalize();

    length = Vec3::dst(begPos, endPos);

    normal = Vec3::cross(Vec3(0,1,0), direction);
    normal.normalize();

    reservedSpaceBeg = 0;
    reservedSpaceEnd = 0;
}

float Driveable::freeSpace(const bool dir) const
{
    if (dir)
    {
        if (vehiclesBeg.size() == 0) return getLength() - reservedSpaceBeg - 0.2;

        return vehiclesBeg.back()->getXPos() - reservedSpaceBeg - 0.2;
    }

    if (vehiclesEnd.size() == 0) return getLength() - reservedSpaceEnd - 0.2;

    return vehiclesEnd.back()->getXPos() - reservedSpaceEnd - 0.2;
}

Vec3 Driveable::getJointPoint(const bool dir) const
{
    if (dir) return begJoint;
    return endJoint;
}

Vec3 Driveable::getNormal() const
{
    return normal;
}

Vec3 Driveable::getDirection() const
{
    return direction;
}

Vec3 Driveable::getBegJointWidth(const bool dir) const
{
    if (dir)
    {
        return begJoint + normal * 0.1;
    }

    return begJoint - normal * 0.1;
}

Vec3 Driveable::getEndJointWidth(const bool dir) const
{
    if (dir)
    {
        return endJoint + normal * 0.1;
    }

    return endJoint - normal * 0.1;
}

void Driveable::draw()
{
    setColor(roadColor);

    Vec3 szer = Vec3::cross(Vec3(0,1,0), direction);
    szer.normalize();
    szer *= 0.3;

    Vec3 a = endPos + szer;
    Vec3 b = endPos - szer;
    Vec3 c = begPos + szer;
    Vec3 d = begPos - szer;

    beginDraw(POLYGON);
    setNormal(0, -1, 0);
    drawVertex(a);
    drawVertex(b);
    drawVertex(d);
    drawVertex(c);
    endDraw();
}

Cross::Cross(Vec3 position)
{
    pos = position;
    allowedVeh = 0;

    isSet = false;
}

void Cross::update(const float delta)
{
    updateCross(delta);
}

bool Cross::dontCheckStreet(const int which)
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

    else if (streets.size() == 2)
    {
        setDefaultPriority();
    }

    else if (streets.size() == 3)
    {
        setDefaultPriority(streets[0].street, streets[1].street, streets[2].street);
    }
    else
    {
        throw "failed to set default right of way at intersection " + id;
    }

    return !isSet;
}

void Cross::updateCross(const float delta)
{
    if (checkSet()) return;

    if (allowedVeh == 0 || streets.size() <= 2)
    {
        tryPassVehiclesWithRightOfWay();

        if (allowedVeh == 0)
        {
            tryPassAnyVehicle();
        }
    }
}

void Cross::tryPassVehiclesWithRightOfWay()
{
    vector<int> indexesToPass;

    if (streets.size() == 2)
    {
        for(unsigned int i=0;i<streets.size();i++)
        {
            if (streets[i].vehicles.size() > 0)
            {
                indexesToPass.push_back(i);
            }
        }
    }

    else if (streets.size() > 2)
    for (unsigned int i=0;i<streets.size();i++)
    {
        if (streets[i].vehicles.size() > 0)
        {
            if (dontCheckStreet(i)) continue;
            if (streets[i].vehicles[0]->getDstToCross() > 1.2) continue;

            int which = streets[i].vehicles[0]->desiredTurn;
            vector<int> yielding = streets[i].yield[which];
            bool isOK = true;

            for (unsigned int j = 0; j < yielding.size(); j++)
            {
                if (streets[yielding[j]].vehicles.size() > 0 && !dontCheckStreet(yielding[j]))
                {
                    isOK = false;
                    break;
                }
            }

            if (isOK)
            {
                indexesToPass.push_back(i);
            }
        }
    }

    for (unsigned int i=0;i<indexesToPass.size();i++)
    {
        if (streets[indexesToPass[i]].vehicles[0]->isEnoughSpace())
        {
            streets[indexesToPass[i]].vehicles[0]->allowedToCross = true;
            streets[indexesToPass[i]].vehicles.erase(streets[indexesToPass[i]].vehicles.begin());
            allowedVeh++;
        }
    }
}

void Cross::tryPassAnyVehicle()
{
    for(unsigned int i=0;i<streets.size();i++)
    {
        if (dontCheckStreet(i)) continue;
        if (streets[i].vehicles.size() > 0 && streets[i].vehicles[0]->dstToCross < 0.7)
        {
            if (streets[i].vehicles[0]->isEnoughSpace())
            {
                streets[i].vehicles[0]->allowedToCross = true;
                streets[i].vehicles.erase(streets[i].vehicles.begin());
                allowedVeh++;

                break;
            }
        }
    }
}

void Cross::setDefaultPriority(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3)
{
    isSet = true;

    for(unsigned int i=0;i<streets.size();i++)
    {
        streets[i].yield.clear();
    }

    if (streets.size() == 2)
    {

    }
    else if (streets.size() == 3)
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

    else if (streets.size() == 4)
    {
        vector<int> yield[4];

        //yield[0].push_back(1);
        //yield[0].push_back(2);
        //yield[1].push_back(1);

        yield[2].push_back(1);

        yield[3].push_back(1);
        yield[3].push_back(2);

        vector<vector<int> > finalVec;

        finalVec.push_back(yield[0]);
        finalVec.push_back(yield[1]);
        finalVec.push_back(yield[2]);
        finalVec.push_back(yield[3]);

        vector<OneStreet> tempVector = streets;

        for (int i = 0; i < 4; i++)
        {
            if (streets[i].street == s0) tempVector[0] = streets[i];
            if (streets[i].street == s1) tempVector[1] = streets[i];
            if (streets[i].street == s2) tempVector[2] = streets[i];
            if (streets[i].street == s3) tempVector[3] = streets[i];
        }
        streets = tempVector;

        for (int i = 0; i < 4; i++)
        {
            streets[i].yield = finalVec;

            rotate(streets[i].yield.begin(), streets[i].yield.begin() + (-i + 4)%4 , streets[i].yield.end());
            for(int j = 0; j < 4; j++)
            for(unsigned int k = 0; k < streets[i].yield[j].size(); k++)
            {
                streets[i].yield[j][k]+= i;
                streets[i].yield[j][k]%=4;
            }
        }
    }
    else
    {
        throw "incorrect number of streets at intersection " + id;
    }
}

Vec3 Cross::OneStreet::getJointPos()
{
    return street->getJointPoint(direction);
}

void Cross::draw()
{
    setColor(roadColor);
    drawTile(0.6);
}

void CrossLights::setDefaultPriority(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3)
{
    Cross::setDefaultPriority(s0, s1, s2, s3);
    setDefaultLights(s0, s1, s2, s3);
}

bool CrossLights::dontCheckStreet(const int which)
{
    return !curPriority[which];
}

void CrossLights::setDefaultLights(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3)
{
    defaultPriority.clear();
    curPriority.clear();

    for (unsigned int i=0;i<streets.size();i++)
    {
        defaultPriority.push_back(false);
        curPriority.push_back(false);
    }

    if (streets.size() == 3)
    {
        //defaultPriority[0] = true;
        for (unsigned int i=0;i<streets.size();i++)
        {
            if (streets[i].street == s0) defaultPriority[i] = true;
        }
    }

    else if (streets.size() == 4)
    {
        for (unsigned int i=0;i<streets.size();i++)
        {
            if (streets[i].street == s0) defaultPriority[i] = true;
            if (streets[i].street == s2) defaultPriority[i] = true;
        }
    }
    else
    {
        throw "incorrect number of streets at intersection " + id;
    }
}

void CrossLights::setLightsPriority()
{
    if (curState == G1 || curState == Y1)
    {
        for(unsigned int i=0;i<streets.size();i++)
        {
            curPriority[i] = defaultPriority[i];
        }
    }

    if (curState == B1 || curState == B2)
    {
        for(unsigned int i=0;i<streets.size();i++)
        {
            curPriority[i] = false;
        }
    }

    if (curState == G2 || curState == Y2)
    {
        for(unsigned int i=0;i<streets.size();i++)
        {
            curPriority[i] = !defaultPriority[i];
        }
    }
}

CrossLights::CrossLights(Vec3 position) : Cross(position)
{
    setLightsDurations();

    curState = B1;
    curTime = durLight.durationBreak;
    setLightsPriority();
}

void CrossLights::setLightsDurations()
{
    durLight.durationGreen1 = randFloat(10, 20);
    durLight.durationYellow1 = randFloat(2, 6);
    durLight.durationGreen2 = randFloat(10, 20);
    durLight.durationYellow2 = randFloat(2, 6);
    durLight.durationBreak = randFloat(1, 6);
}

void CrossLights::getNextState()
{
    if (curTime <= 0)
    {
        if (curState == G1)
        {
            curState = Y1;
            curTime = durLight.durationYellow1;
            setLightsPriority();
            return;
        }
        if (curState == Y1)
        {
            curState = B1;
            curTime = durLight.durationBreak;
            setLightsPriority();
            return;
        }
        if (curState == B1)
        {
            curState = G2;
            curTime = durLight.durationGreen2;
            setLightsPriority();
            return;
        }
        if (curState == G2)
        {
            curState = Y2;
            curTime = durLight.durationYellow2;
            setLightsPriority();
            return;
        }
        if (curState == Y2)
        {
            curState = B2;
            curTime = durLight.durationBreak;
            setLightsPriority();
            return;
        }
        if (curState == B2)
        {
            curState = G1;
            curTime = durLight.durationGreen1;
            setLightsPriority();
            return;
        }
    }
}

void CrossLights::update(const float delta)
{
    updateCross(delta);

    curTime -= delta;
    getNextState();
}

void CrossLights::draw()
{
    Cross::draw();

    Vec3 color1;
    Vec3 color2;

    if (curState == G1)
    {
        color1 = Vec3(0,1,0);
        color2 = Vec3(1,0,0);
    }
    if (curState == G2)
    {
        color1 = Vec3(1,0,0);
        color2 = Vec3(0,1,0);
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
    }

    translate(-pos);

    for(unsigned int i =0;i<streets.size();i++)
    {
        pushMatrix();
        translate(0,0.35,0);

        if (!streets[i].direction)
            translate(streets[i].street->getNormal() / 10.0);
        else
            translate(-streets[i].street->getNormal() / 10.0);

        translate(streets[i].getJointPos());
        if (defaultPriority[i]) setColor(color1.x, color1.y, color1.z);
        else setColor(color2.x, color2.y, color2.z);

        if (streets[i].direction)
            rotateY(streets[i].street->getNormal().angleXZ());
        else
            rotateY(streets[i].street->getNormal().angleXZ() + 180);

        drawCube(0.05,0.1,0.05);

        translate(-0.2,0,0);

        setColor(0.5, 0.5, 0.5);
        translate(0,-0.35/2,0);
        drawCube(0.025,0.35,0.025);

        translate(0.1,0.35/2,0);
        drawCube(0.225,0.025,0.025);

        popMatrix();
    }
}

Street::Street(Cross *begCross, Cross *endCross) : Driveable(begCross, endCross)
{

}

void Street::draw()
{
    Driveable::draw();
}
