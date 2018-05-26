#include "Vehicle.h"
#include "Street.h"

class Road;

int Vehicle::numVeh = 0;

Vehicle::Vehicle(Road *spawnRoad)
{
    //debugstop = false;

    numVeh++;

    maxV = randFloat(1, 1.5);
    minV = randFloat(0.02, 0.08);
    velocity = 0;
    //cornerVelocity = randFloat(0.5, 1);
    cornerVelocity = 1;
    stopTime = randFloat(0.5, 0.8);
    acceleration = randFloat(0.1,0.2);
    vehicleLength = 0.2;
    remainDst = randFloat(0.1, 0.2);

    xPos = 0;

    isBraking = false;

    dstToCross = 1000;
    direction = true;
    desiredTurn = 0;

    blinker = 0;
    blinkerTime = 0;
    blinkerDuration  = randFloat(0.45,0.55);
    blinkerLight = true;

    gameEngine = spawnRoad->gameEngine;

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

    rot = Vec3(0, curRoad->direction.angleXZ(), 0);

    if (curRoad->vehiclesBeg.size() > 0 /*&& !curRoad->vehiclesBeg.back()->isChanging*/)
    {
        isFirstVeh = false;
        frontVeh = curRoad->vehiclesBeg.back();
        frontVeh->backVeh = this;
    }
}
#include<iostream>
using namespace std;
bool pierwszy = true;
Vehicle *pie;
int kla = 0;

void Vehicle::update(float delta)
{
    //if (debugstop) return;

    if (!isChanging && !didReachCross)
    {
        //if (id.compare("CAR_3GA1A")==0)cout<<"             "<<id<<endl;

        float prevVelocity = velocity;

        xPos += velocity * delta;

        float posDiff = getDst() - vehicleLength / 2.0 - remainDst;
        float newDst = velocity * stopTime - acceleration * stopTime * stopTime / 2.0;

        if (newDst > posDiff)
        {
            velocity = posDiff - acceleration * stopTime * stopTime / 2.0;
            velocity /= stopTime;
            //isBraking = true;
        }
        else if (posDiff > newDst)
        {
            velocity = posDiff - acceleration * stopTime * stopTime / 2.0;
            velocity /= stopTime;
            //velocity += acceleration * delta;
            //isBraking = false;
        }

        //if (id.compare("CAR_3GA1A")==0)cout<<"           "<<getDst()<<endl;
        //if (id.compare("CAR_3GA1A")==0)cout<<"             "<<velocity<<endl;

        float braking = (velocity - prevVelocity) / delta;
        //cout<<"BRAKE:  "<<braking<<endl;
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



        //cout << "         "<<velocity<<endl;

        /*if ((xPos > 2 && pierwszy) || pie == this)
        {xPos = 2;
        pie= this;
        pierwszy = false;
        kla++;
        if (kla > 300)
            pie = NULL;
        }*/

        if(xPos > curRoad->length)
        {
            xPos = curRoad->length;
            //didReachCross = true;
        }

        float s = xPos / curRoad->length;

        if (s>1)
        {
            s=1;
        }

        //if (frontVeh!=NULL) cout<<id<<"   "<<xPos<<"   "<<frontVeh->xPos<< " "<<curRoad->length<<"  "<<getDst()<<endl;
        //cout<<id<<"   "<<curRoad->id<<"  "<<xPos<<"  "<<pos<< "   "<<curRoad->length<<"  "<<getDst()<<endl;

        if (direction)
        {
            //pos = Vec3::lerp(curRoad->begJoint, curRoad->endJoint, s);
            //pos += curRoad->normal * 0.1;
            pos = Vec3::lerp(curRoad->getBegJoint(direction), curRoad->getEndJoint(direction), s);
        }
        else
        {
            //pos = Vec3::lerp(curRoad->endJoint, curRoad->begJoint, s);
            //pos -= curRoad->normal * 0.1;
            pos = Vec3::lerp(curRoad->getEndJoint(direction), curRoad->getBegJoint(direction), s);
        }

        //pos = Vec3::lerp(curRoad->getJoint(direction))

        dstToCross = curRoad->length - xPos;
        //cout<<id<<endl;
        if (curRoad->length - xPos < 1.5 && curCross == NULL)
        {
            //Cross *newCross
            //cout<<"CHECK  "<<id<<endl;

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
                        if (curCross->id.compare("SK3") ==0) cout<< curCross->streets.size()-1<<endl;

                        desiredTurn = randInt(0, curCross->streets.size()-1);
                        if (desiredTurn == (int)i) desiredTurn = (desiredTurn+1) % curCross->streets.size();

                        if (curCross->streets.size() == 2) desiredTurn = (i+1) % 2;

                        nextRoad = curCross->streets[desiredTurn].street;

                        begRot = curRoad->direction.angleXZ();
                        endRot = nextRoad->direction.angleXZ();

                        if (!direction) begRot += 180;
                        if (!curCross->streets[desiredTurn].direction) endRot += 180;

                        //blinker = rotateDirection(curRoad->direction.angleXZ(), nextRoad->direction.angleXZ());
                        blinker = rotateDirection(begRot, endRot);
                        if (curCross->streets.size() == 2) blinker = 0;

                        blinkerTime = 0;
                        blinkerLight = true;

                        //cout<<id<<"  desired road: "<<nextRoad->id<<endl;

                        curCross->streets[i].vehicles.push_back(this);

                        break;
                    }
                }
            }

        }
    }

    //changing street at cross

    /*if (nextRoad!=NULL && nextRoad->id.compare("GA1A") == 0)
    {
        cout<<id<<" "<<isLeavingRoad<< " ";
        cout<<(curCross!=NULL)<<" "<<(nextRoad!= NULL)<< " "<<allowedToCross<<"  "<<desiredTurn;
        //cout<<"adress: "<<nextRoad<<endl;
        cout<< " "<<nextRoad->freeSpace(direction)<<endl;

        //int i;cin>>i;
    }*/

    if (!isLeavingRoad && curCross != NULL && nextRoad != NULL && allowedToCross/* && nextRoad->freeSpace(direction) > vehicleLength + remainDst*/)
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

                    /*if(backVeh != NULL)
                    {
                        backVeh->isFirstVeh = true;
                        backVeh->frontVeh = NULL;
                        //cout<<"prev veh: "<<backVeh->id<<"  "<<backVeh->frontVeh<<endl;
                        //int i;cin>>i;
                    }*/
                    isChanging = true;
                    didReachCross = false;
                }

                //cout<<curCross->id<<"  "<<id<<" warunek"<<endl;
                //xPos = 0;

                break;
            }
        }
    }

    /*if (isChanging && curRoad != NULL)
    {

        xPos += velocity * delta;
         float s = xPos / curRoad->length;
        //cout<<xPos<<"   "<<getDst()<<endl;
        pos = Vec3::lerp(curRoad->begPos, curRoad->endPos, s);

        if (s>1.5)//DEBUG
        {
            if (allowedToCross && curRoad != NULL && curCross != NULL)
            {
                debugstop = true;
                allowedToCross = false;
                cout<<"unregister&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;

                if (direction)
                {
                    curRoad->vehiclesBeg.pop();
                }
                else
                {
                    curRoad->vehiclesEnd.pop();
                }
                curCross->allowedVeh--;
                curCross = NULL;
                curRoad = NULL;
            }
        }
    }*/

    //if (id.compare("CAR_6GA1B") == 0) cout<<rot<<endl;

    if (isChanging)
    {
        xPos += cornerVelocity * delta;

        float s = xPos / curRoad->length;
        //cout<<xPos<<"   "<<s<<endl;
        //cout<<curRoad->begPos<<curRoad->endPos<<endl;

        if (direction)
        {
            pos = Vec3::lerp(curRoad->getBegJoint(direction), curRoad->getEndJoint(direction), s);
        }
        else
        {
            pos = Vec3::lerp(curRoad->getEndJoint(direction), curRoad->getBegJoint(direction), s);
        }



        if(s > 1)
        {
            xPos = 0;
            didReachCross = true;
            isChanging = false;

            if (curCross->streets[desiredTurn].direction)
            {
                nextRoadJoint = nextRoad->getBegJoint(curCross->streets[desiredTurn].direction);
            }
            else
            {
                nextRoadJoint = nextRoad->getEndJoint(curCross->streets[desiredTurn].direction);
            }

            if(backVeh != NULL)
            {
                backVeh->isFirstVeh = true;
                backVeh->frontVeh = NULL;
                //cout<<"prev veh: "<<backVeh->id<<"  "<<backVeh->frontVeh<<endl;
                //int i;cin>>i;
            }

            begRot = curRoad->direction.angleXZ();
            endRot = nextRoad->direction.angleXZ();

            if (!direction) begRot += 180;
            if (!curCross->streets[desiredTurn].direction) endRot += 180;

            //cout<<begRot<<"    "<<endRot<<"   "<<blinker<<endl;
            //int r;cin>>r;
        }
        //cout<<"changing "<<s<<endl;
        //didReachCross = true;
        //int i;cin>>i;
    }



    if (didReachCross)
    {
        xPos += cornerVelocity * delta;
        float tempLength;
        float s;

            if (direction)
            {
                //cout<<"desired: "<<id<<"   "<<desiredTurn<<"   "<<curCross->streets.size()<<endl;
                //tempLength = Vec3::length(curRoad->endPos - curCross->streets[desiredTurn].jointPos);
                tempLength = Vec3::length(curRoad->getEndJoint(direction) - nextRoadJoint);
                s = xPos / tempLength;

                if(s>1)s=1;

                //cout<<"S: "<<id<<"   "<<s<<"   "<<tempLength<< " "<<curRoad->endPos << curCross->streets[desiredTurn].jointPos<<endl;

                //pos = Vec3::lerp(curRoad->endPos, curCross->streets[desiredTurn].jointPos, s);
                pos  = Vec3::lerp(curRoad->getEndJoint(direction), nextRoadJoint, s);
            }
            else
            {
                //tempLength = Vec3::length(curRoad->begPos - curCross->streets[desiredTurn].jointPos);
                tempLength = Vec3::length(curRoad->getBegJoint(direction) - nextRoadJoint);
                s = xPos / tempLength;

                if(s>1)s=1;

                //pos = Vec3::lerp(curRoad->begPos, curCross->streets[desiredTurn].jointPos, s);
                pos  = Vec3::lerp(curRoad->getBegJoint(direction), nextRoadJoint, s);
            }

            rot = Vec3(0, lerpAngle(begRot, endRot, s), 0);
            crossProgress = s;

            //if (id.compare("CAR_6GA1B") == 0) cout<<begRot<<"   "<<endRot<<"  "<<lerpAngle(begRot, endRot, s)<<endl;

            if (s>=1)
            {
                blinker = 0;

                if(backVeh != NULL)
                {
                    backVeh->isFirstVeh = true;
                    backVeh->frontVeh = NULL;
                    //cout<<"prev veh: "<<backVeh->id<<"  "<<backVeh->frontVeh<<endl;
                    //int i;cin>>i;
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

                //bool prevDir = direction;
                direction = curCross->streets[desiredTurn].direction;

                dstToCross = curCross->length;

                if (direction)
                {
                    nextRoad->reservedSpaceBeg -= vehicleLength + remainDst;
                }
                else
                {
                    nextRoad->reservedSpaceEnd -= vehicleLength + remainDst;
                }


                /*if (curCross->streets[desiredTurn].direction)
                {
                    curRoad->vehiclesBeg.push(this);
                }
                else
                {
                    curRoad->vehiclesEnd.push(this);
                }*/
                //cout<<id<<"    "<<curCross->id<<"    befroe "<<curCross->allowedVeh<<endl;
                curCross->allowedVeh--;
                desiredTurn = 0;
                //cout<<id<<curCross->id<<"    after  "<<curCross->allowedVeh<<endl;

                //curCross->streets[desiredTurn].vehicles.erase(curCross->streets[desiredTurn].vehicles.begin());

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

            //cout<<"reachcross"<<endl;
    }

    blinkerTime += delta;

    if (blinkerTime > blinkerDuration)
    {
        blinkerTime = 0;
        blinkerLight = !blinkerLight;
    }
}

float Vehicle::getDst()
{
    //cout<<"getdst  "<<id<<"  "<<frontVeh<<endl;
    //if (id.compare("CAR_3GA1A")==0 && frontVeh != NULL)cout<<"          "<<curRoad->length<<"   "<<frontVeh->xPos<<frontVeh->id<<endl;
    if (frontVeh != NULL)
        return frontVeh->xPos - xPos - frontVeh->vehicleLength/2.0;

    return curRoad->length - xPos;
}

bool Vehicle::isEnoughSpace()
{
    if (nextRoad == NULL || curCross == NULL || desiredTurn >= (int)curCross->streets.size()) return false;

    return nextRoad->freeSpace(curCross->streets[desiredTurn].direction) > vehicleLength + remainDst;
}

Car::Car(Road *spawnRoad) : Vehicle(spawnRoad)
{
//    Vehicle(spawnRoad);
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
        //drawCube(1,0.02,0.01);
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

    //setColor(0,1,0);

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
    if (curCross != NULL/* && allowedToCross*/)
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
    }

    if (idnumber == 64)
    {
        setColor(1,0.667,0);
    }
    if (idnumber == 41)
    {
        setColor(0.7,0.5,0);
    }



    /*if (curCross != NULL)
    {
        setColor(0,0,0);
    }*/

    //drawCube(0.2,4,0.2);
    pushMatrix();
    translate(0,0.05,0);
    drawCube(0.2,0.05,0.1);
    drawRoof();

    //glTranslatef(0.2,0,0);
    //setColor(0,0,0);
    //drawCube(0.1);
    popMatrix();

    /*glPushMatrix();
    glTranslatef(0.25,0,0);
    setColor(0,0,0);
    for(int i=0;i<idnumber;i++)
    {
        glTranslatef(0.2,0,0);
        drawCube(0.1);
    }
    glPopMatrix();*/
}

Bus::Bus(Road *spawnRoad) : Vehicle(spawnRoad)
{
//    Vehicle(spawnRoad);
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

    //if(id.compare("BUS_4")==0) cout<<"BUS ANGLE  "<<id<<"  "<<busAngle<<endl;
}

void Bus::draw()
{
    pushMatrix();

    translate(0,0.1,0);

    setColor(0.7,0.7,0);
    pushMatrix();
    //glRotatef(busAngle, 0,-1,0);
    rotateY(-busAngle);
    translate(-0.2,0,0);
    drawCube(0.3,0.13,0.135);
    setColor(0,0.8,0.8);
    translate(-0.02,0.02,0);
    drawCube(0.25,0.07,0.14);
    popMatrix();

    setColor(0.7,0.7,0);
    pushMatrix();
    //glRotatef(busAngle , 0,1,0);
    rotateY(busAngle);
    translate(0.2,0,0);
    drawCube(0.3, 0.13, 0.135);
    setColor(0,0.8,0.8);
    translate(0.02,0.02,0);
    drawCube(0.27,0.07,0.14);
    popMatrix();

    setColor(0.5,0.5,0);
    drawCube(0.4,0.12,0.12);

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
        rotateY(-busAngle);

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

    /*if (blinker < 0 && blinkerLight)
    {
        setColor(1,0.647,0);
        glPushMatrix();
        glRotatef(busAngle, 0,-1,0);
        glTranslatef(-0.2,0,0);
        drawCube(0.22,0.02,0.01);
        //setColor(0,0.8,0.8);
        //glTranslatef(-0.02,0.02,0);
        //drawCube(0.27,0.07,0.14);
        glPopMatrix();
    }
    if (blinker > 0 && blinkerLight)
    {
        glPushMatrix();
        glTranslatef(0,0.05,0.038);
        setColor(1,0.647,0);
        drawCube(0.22,0.02,0.01);
        glPopMatrix();
    }*/


    popMatrix();
}
