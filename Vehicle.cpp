#include "Vehicle.h"
#include "Street.h"

class Driveable;

int Vehicle::numVeh = 0;

Vehicle::Vehicle(Driveable *spawnRoad)
{
    numVeh++;

    initRandValues();

    velocity = 0;
    vehicleLength = 0.2;
    xPos = 0;
    maxAcceleration = 0.5;

    isBraking = false;

    dstToCross = 1000;
    direction = true;
    desiredTurn = 0;

    initBlinkers();

    initPointers(spawnRoad);

    rot = Vec3(0, curRoad->getDirection().angleXZ(), 0);
}

void Vehicle::initRandValues()
{
    maxV = randFloat(1, 1.5);
    minV = randFloat(0.02, 0.08);
    cornerVelocity = 1;
    stopTime = randFloat(0.5, 0.8);
    acceleration = randFloat(0.1,0.2);
    remainDst = randFloat(0.1, 0.2);
}

void Vehicle::initBlinkers()
{
    blinker = 0;
    blinkerTime = 0;
    blinkerDuration  = randFloat(0.45,0.55);
    blinkerLight = true;
}

void Vehicle::initPointers(Driveable *spawnRoad)
{
    curRoad = spawnRoad;
    curCross = NULL;
    nextRoad = NULL;

    backVeh = NULL;

    allowedToCross = false;
    isChanging = false;
    didReachCross = false;
    isLeavingRoad = false;

    frontVeh = NULL;
    isFirstVeh = true;

    if (curRoad->vehiclesBeg.size() > 0 /*&& !curRoad->vehiclesBeg.back()->isChanging*/)
    {
        isFirstVeh = false;
        frontVeh = curRoad->vehiclesBeg.back();
        frontVeh->backVeh = this;
    }
}

void Vehicle::update(float delta)
{
    if (!isChanging && !didReachCross)
    {
        float prevVelocity = velocity;

        xPos += velocity * delta;

        setVelocity();
        checkVelocity(delta, prevVelocity);
        setNewPos();

        if (curRoad->getLength() - xPos < 1.5 && curCross == NULL)
        {
            registerToCross();
        }
    }

    if (!isLeavingRoad && curCross != NULL && nextRoad != NULL && allowedToCross/* && nextRoad->freeSpace(direction) > vehicleLength + remainDst*/)
    {
        tryBeAllowedToEnterCross();
    }

    if (isChanging)
    {
        xPos += cornerVelocity * delta;

        float s = xPos / curRoad->getLength();

        if (direction)
        {
            pos = Vec3::lerp(curRoad->getBegJointWidth(direction), curRoad->getEndJointWidth(direction), s);
        }
        else
        {
            pos = Vec3::lerp(curRoad->getEndJointWidth(direction), curRoad->getBegJointWidth(direction), s);
        }

        if(s > 1)
        {
            leaveRoad();
        }
    }

    if (didReachCross)
    {
        xPos += cornerVelocity * delta;

        setCornerPosition();

        if (crossProgress>=1)
        {
            enterNewRoad();
        }
    }

    updateBlinkers(delta);
}

void Vehicle::setVelocity()
{
    float posDiff = getDst() - vehicleLength / 2.0 - remainDst;
    float newDst = velocity * stopTime - acceleration * stopTime * stopTime / 2.0;

    if (newDst > posDiff)
    {
        velocity = posDiff - acceleration * stopTime * stopTime / 2.0;
        velocity /= stopTime;
    }
    else if (posDiff > newDst)
    {
        velocity = posDiff - acceleration * stopTime * stopTime / 2.0;
        velocity /= stopTime;
    }
}

void Vehicle::checkVelocity(float delta, float prevVelocity)
{
    float braking = (velocity - prevVelocity) / delta;

    if (braking < -0.3)
    {
        isBraking = true;
    }
    else
    {
        isBraking = false;
    }

    if (velocity < minV)
    {
        velocity = 0;
        isBraking = true;
    }

    if (velocity > maxV)
    {
        velocity = maxV;
    }
}

float Vehicle::getXPos()
{
    return xPos;
}

float Vehicle::getDstToCross()
{
    return dstToCross;
}

int Vehicle::getNumberId()
{
    return numVeh;
}

void Vehicle::setNewPos()
{

    if(xPos > curRoad->getLength())
    {
        xPos = curRoad->getLength();
    }

    float s = xPos / curRoad->getLength();

    if (s>1)
    {
        s=1;
    }

    if (direction)
    {
        pos = Vec3::lerp(curRoad->getBegJointWidth(direction), curRoad->getEndJointWidth(direction), s);
    }
    else
    {
        pos = Vec3::lerp(curRoad->getEndJointWidth(direction), curRoad->getBegJointWidth(direction), s);
    }

    dstToCross = curRoad->getLength() - xPos;
}

void Vehicle::registerToCross()
{
    allowedToCross = false;

    if(direction)
    {
        curCross = curRoad->crossEnd;
    }
    else
    {
        curCross = curRoad->crossBeg;
    }

    blinker = 0;

    if (curCross != NULL)
    {
        for (unsigned int i=0;i<curCross->streets.size();i++)
        {
            if (curCross->streets[i].street == curRoad)
            {
                desiredTurn = randInt(0, curCross->streets.size()-1);
                if (desiredTurn == (int)i) desiredTurn = (desiredTurn+1) % curCross->streets.size();

                if (curCross->streets.size() == 2) desiredTurn = (i+1) % 2;

                nextRoad = curCross->streets[desiredTurn].street;

                begRot = curRoad->direction.angleXZ();
                endRot = nextRoad->direction.angleXZ();

                if (!direction) begRot += 180;
                if (!curCross->streets[desiredTurn].direction) endRot += 180;

                blinker = rotateDirection(begRot, endRot);
                if (curCross->streets.size() == 2) blinker = 0;

                blinkerTime = 0;
                blinkerLight = true;

                curCross->streets[i].vehicles.push_back(this);

                break;
            }
        }
    }
}

void Vehicle::tryBeAllowedToEnterCross()
{
    for(unsigned int i=0;i<curCross->streets.size();i++)
    {
        if (curCross->streets[i].street == nextRoad)
        {
            if (nextRoad->freeSpace(curCross->streets[i].direction) > vehicleLength + remainDst)
            {
                isLeavingRoad = true;

                if (curCross->streets[i].direction)
                {
                    nextRoad->reservedSpaceBeg += vehicleLength + remainDst;
                }
                else
                {
                    nextRoad->reservedSpaceEnd += vehicleLength + remainDst;
                }

                isChanging = true;
                didReachCross = false;
            }

            break;
        }
    }
}

void Vehicle::leaveRoad()
{
    xPos = 0;
    didReachCross = true;
    isChanging = false;

    if (curCross->streets[desiredTurn].direction)
    {
        nextRoadJoint = nextRoad->getBegJointWidth(curCross->streets[desiredTurn].direction);
    }
    else
    {
        nextRoadJoint = nextRoad->getEndJointWidth(curCross->streets[desiredTurn].direction);
    }

    if(backVeh != NULL)
    {
        backVeh->isFirstVeh = true;
        backVeh->frontVeh = NULL;
    }

    begRot = curRoad->direction.angleXZ();
    endRot = nextRoad->direction.angleXZ();

    if (!direction) begRot += 180;
    if (!curCross->streets[desiredTurn].direction) endRot += 180;
}

void Vehicle::setCornerPosition()
{
    float tempLength;
    float s;

    if (direction)
    {
        tempLength = Vec3::length(curRoad->getEndJointWidth(direction) - nextRoadJoint);
        s = xPos / tempLength;

        if(s>1)s=1;

        pos  = Vec3::lerp(curRoad->getEndJointWidth(direction), nextRoadJoint, s);
    }
    else
    {
        tempLength = Vec3::length(curRoad->getBegJointWidth(direction) - nextRoadJoint);
        s = xPos / tempLength;

        if(s>1)s=1;

        pos  = Vec3::lerp(curRoad->getBegJointWidth(direction), nextRoadJoint, s);
    }

    rot = Vec3(0, lerpAngle(begRot, endRot, s), 0);
    crossProgress = s;
}

void Vehicle::enterNewRoad()
{
    blinker = 0;

    if(backVeh != NULL)
    {
        backVeh->isFirstVeh = true;
        backVeh->frontVeh = NULL;
    }

    if (direction)
    {
        curRoad->vehiclesBeg.pop();
    }
    else
    {
        curRoad->vehiclesEnd.pop();
    }

    xPos = 0;
    isBraking = false;

    allowedToCross = false;

    isChanging = false;
    didReachCross = false;
    isLeavingRoad = false;

    velocity = cornerVelocity;

    curRoad = curCross->streets[desiredTurn].street;

    direction = curCross->streets[desiredTurn].direction;

    dstToCross = curCross->getLength();

    if (direction)
    {
        nextRoad->reservedSpaceBeg -= vehicleLength + remainDst;
    }
    else
    {
        nextRoad->reservedSpaceEnd -= vehicleLength + remainDst;
    }

    curCross->allowedVeh--;
    desiredTurn = 0;

    backVeh = NULL;

    frontVeh = NULL;
    isFirstVeh = true;

    if (direction)
    {
        if (curRoad->vehiclesBeg.size() > 0 /*&& !curRoad->vehiclesBeg.back()->isChanging*/)
        {
            isFirstVeh = false;
            frontVeh = curRoad->vehiclesBeg.back();
            frontVeh->backVeh = this;
        }
    }
    else
    {
        if (curRoad->vehiclesEnd.size() > 0 /*&& !curRoad->vehiclesEnd.back()->isChanging*/)
        {
            isFirstVeh = false;
            frontVeh = curRoad->vehiclesEnd.back();
            frontVeh->backVeh = this;
        }
    }

    if (direction)
    {
        curRoad->vehiclesBeg.push(this);
    }
    else
    {
        curRoad->vehiclesEnd.push(this);
    }

    nextRoad = NULL;
    curCross = NULL;
}

void Vehicle::updateBlinkers(float delta)
{
    blinkerTime += delta;

    if (blinkerTime > blinkerDuration)
    {
        blinkerTime = 0;
        blinkerLight = !blinkerLight;
    }
}

float Vehicle::getDst()
{
    if (frontVeh != NULL)
        return frontVeh->xPos - xPos - frontVeh->vehicleLength/2.0;

    return curRoad->getLength() - xPos;
}

bool Vehicle::isEnoughSpace()
{
    if (nextRoad == NULL || curCross == NULL || desiredTurn >= (int)curCross->streets.size()) return false;

    return nextRoad->freeSpace(curCross->streets[desiredTurn].direction) > vehicleLength + remainDst;
}

Car::Car(Driveable *spawnRoad) : Vehicle(spawnRoad)
{
    velocity = randFloat(2,5);
}

void Car::update(float delta)
{
    Vehicle::update(delta);
}

void Car::draw()
{
    if (blinker < 0 && blinkerLight)
    {
        pushMatrix();
        translate(0,0.05,-0.038);
        setColor(1,0.647,0);
        drawCube(0.22,0.02,0.01);
        popMatrix();
    }
    if (blinker > 0 && blinkerLight)
    {
        pushMatrix();
        translate(0,0.05,0.038);
        setColor(1,0.647,0);
        drawCube(0.22,0.02,0.01);
        popMatrix();
    }

    if (isBraking)
    {
        setColor(1,0,0);

        pushMatrix();
        translate(-0.05,0.08,0);
        drawCube(0.07,0.003,0.04);
        popMatrix();

        pushMatrix();
        translate(-0.05,0.05,0.033);
        drawCube(0.12,0.01,0.01);
        popMatrix();

        pushMatrix();
        translate(-0.05,0.05,-0.033);
        drawCube(0.12,0.01,0.01);
        popMatrix();
    }
    setColor(0,1,0);
    /*if (curCross != NULL)
    {
        setColor(0,0,1);
    }
    if (isChanging)
    {
        setColor(1,1,0);
    }
    if (isLeavingRoad)
    {
        setColor(0,1,1);
    }*/

    /*if (curCross != NULL)
    {
        setColor(0,0,0);
    }*/

    //drawCube(0.2,4,0.2);
    pushMatrix();
    translate(0,0.05,0);
    drawCube(0.2,0.05,0.1);
    drawRoof();

    popMatrix();
}

Bus::Bus(Driveable *spawnRoad) : Vehicle(spawnRoad)
{
    maxV = randFloat(0.8, 1.1);
    velocity = randFloat(2,5);
    vehicleLength = 0.65;
}

void Bus::update(float delta)
{
    Vehicle::update(delta);

    if (didReachCross)
    {
        //busAngle = (begRot - endRot) * crossProgress;
        float s = crossProgress * 2.0 - 1.0;
        s = 1 - abs(s);
        float a = Vec3::angleDiff(begRot, endRot);
        a /= 4.0;
        //cout<<"BUS ANG   "<<begRot<<"  "<<endRot<<"    "<<a*2.3<<endl;

        busAngle = lerpAngle(0, a, s);
    }
    else
    {
        busAngle = 0;
    }
}

void Bus::draw()
{
    pushMatrix();

    translate(0,0.1,0);

    setColor(0.7,0.7,0);
    pushMatrix();
    //glRotatef(busAngle, 0,-1,0);
    rotateY(-busAngle / 2);
    translate(-0.2,0,0);
    drawCube(0.3,0.13,0.135);
    setColor(0,0.8,0.8);
    translate(-0.02,0.02,0);
    drawCube(0.25,0.07,0.14);
    popMatrix();

    setColor(0.7,0.7,0);
    pushMatrix();
    //glRotatef(busAngle , 0,1,0);
    //rotateY(busAngle);
    translate(0.2,0,0);
    drawCube(0.3, 0.13, 0.135);
    setColor(0,0.8,0.8);
    translate(0.02,0.02,0);
    drawCube(0.27,0.07,0.14);
    popMatrix();

    setColor(0.5,0.5,0);
    drawCube(0.2,0.12,0.12);

    pushMatrix();
    if (blinker < 0 && blinkerLight)
    {
        setColor(1,0.647,0);
        translate(0, -0.031,-0.046);
        drawCube(0.73,0.01,0.01);
    }
    if (blinker > 0 && blinkerLight)
    {
        setColor(1,0.647,0);
        translate(0, -0.031,0.046);
        drawCube(0.73,0.01,0.01);
    }
    popMatrix();

    if (isBraking)
    {
        pushMatrix();
        setColor(1,0,0);
        rotateY(-busAngle / 2);

        pushMatrix();
        translate(-0.3,0.05,0);
        drawCube(0.12,0.003,0.06);
        popMatrix();

        pushMatrix();
        translate(-0.3,-0.02,0.04);
        drawCube(0.12,0.01,0.01);
        popMatrix();

        pushMatrix();
        translate(-0.3,-0.02,-0.04);
        drawCube(0.12,0.01,0.01);
        popMatrix();
        popMatrix();
    }

    popMatrix();
}
