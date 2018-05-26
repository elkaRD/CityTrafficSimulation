/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */

#include"Simulator.h"
using namespace std;



void Simulator::redraw()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    for (unsigned int i=0;i<objects.size();i++)
    {
        objects[i]->drawObject();
    }

    glPopMatrix();


    if (doubleBuffer)
        glXSwapBuffers(dpy, win);/* buffer swap does implicit glFlush */
    else
        glFlush();  /* explicit flush for single buffered case */
}



Simulator::Simulator()
{
    if (!didInit)
    {
        //todo throw
    }

    cameraPos = Vec3(0,20,0);
    cameraRot = Vec3(0,90,0);
}

void Simulator::run()
{
    GLboolean            needRedraw = GL_FALSE, recalcModelView = GL_TRUE;;
    XEvent               event;

    //startTime = time(0);
    //lastTime = startTime;
    gettimeofday(&startTime, 0);
    lastTime = startTime;

    while (1)
    {

        if (XCheckWindowEvent(dpy, win, eventMask, &event))
        {
            //   cout<<"event"<<endl;
        }
       // else cout<<"not"<<endl;
    //do
    {
        //XNextEvent(dpy, &event);

        switch (event.type)
        {
            case KeyPress:
            {
                KeySym     keysym;
                XKeyEvent *kevent;
                char       buffer[1];
                /* It is necessary to convert the keycode to a
                    * keysym before checking if it is an escape */
                kevent = (XKeyEvent *) &event;
                if (   (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1) && (keysym == (KeySym)XK_Escape) )
                {
                    cleanSimulation();
                    exit(0);
                }

                if (buffer[0] < 256)
                    pressedKey[buffer[0]]=true;

                cout<<"key pressed           sdfdsf"<<endl;

                moveCamera(0.016);

                if(buffer[0] >= 'a' && buffer[0] <= 'z')
                    pressedKey[buffer[0] - 32] = true;

                if(buffer[0] >= 'A' && buffer[0] <= 'Z')
                    pressedKey[buffer[0] + 32] = true;

                if (keysym == (KeySym)XK_Shift_L) pressedShift = true;

                //cout<<buffer[0]<<"   "<<keysym<<endl;
                break;
            }

            case KeyRelease:
            {
                KeySym     keysym;
                XKeyEvent *kevent;
                char       buffer[1];
                /* It is necessary to convert the keycode to a
                * keysym before checking if it is an escape */
                kevent = (XKeyEvent *) &event;
                /*if (   (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1) && (keysym == (KeySym)XK_Escape) )
                {
                    cleanSimulation();
                    cout<<"TEST DEST"<<endl;

                    exit(0);
                }*/

                //if (buffer[0] < 256) pressedKey[buffer[0]]=false;

                //if (keysym == (KeySym)XK_Shift_L) pressedShift = false;

                //cout<<buffer[0]<<"   "<<keysym<<endl;

                /*switch(buffer[0])
                {
                    case 'w':case 'W': pressedKey['W']=true; break;
                }*/

                //cout<<"before: "<<cameraPos<<endl;

                //moveCamera(0.016);

                //cout<<"after: "<<cameraPos<<endl;
                //cout<<"after all: "<<cameraPos<<endl;
                //cout<<"after all: "<<cameraPos<<endl;

                //for(int i=0;i<256;i++) pressedKey[i]=false;


                //cout<<"after all: "<<cameraPos<<endl;
                break;
            }


            case ButtonPress:

                XButtonEvent *bevent;
                bevent = (XButtonEvent*) &event;

                prevMouseX = bevent->x;
                prevMouseY = bevent->y;

                recalcModelView = GL_TRUE;
                switch (event.xbutton.button)
                {
                    case 1: xAngle += 10.0;
                            break;
                    case 2: yAngle += 10.0;
                            break;
                    case 3: zAngle += 10.0;
                            break;
                }
                break;

            case MotionNotify:
            {
                //KeySym     keysym;
                XMotionEvent *mevent;
                //char       buffer[1];

                mevent = (XMotionEvent *) &event;

                //cout<<mevent->x<<"     "<<mevent->y<<endl;

                int dx = mevent->x - prevMouseX;
                int dy = mevent->y - prevMouseY;

                cameraRot.x += dx * 0.2;
                cameraRot.y += dy * 0.2;

                if (cameraRot.y > 90) cameraRot.y = 90;
                        //cout<<"camera y: "<<cameraRot.y<<endl;
                if (cameraRot.y < -90) cameraRot.y = -90;

                prevMouseX = mevent->x;
                prevMouseY = mevent->y;

                break;
            }

            case ConfigureNotify:

                updateRatio = true;
                width = event.xconfigure.width;
                height = event.xconfigure.height;
          //glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
          /* fall through... */
            case Expose:
                needRedraw = GL_TRUE;
                break;
        }
    }// while(XPending(dpy)); /* loop to compress events */
//cout<<"after all: "<<cameraPos<<endl;

    recalcModelView = GL_TRUE;
    //if (recalcModelView)
    {
    //if (updateRatio)
      {
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          updateRatio = false;
          float screenRatio = width / height * 2.0;
          glViewport(0,0,width,height);
          glFrustum(-1.0 * screenRatio, 1.0 * screenRatio, -1.0, 1.0, 5.0, 1000.0);
          //glTranslatef(0,0,-5);
      }
//cout<<"after all: "<<cameraPos<<endl;


      glMatrixMode(GL_MODELVIEW);

      /* reset modelview matrix to the identity matrix */
      glLoadIdentity();



        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      /* move the camera back three units */
        glTranslatef(0,0,5);

      //glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);
        //glRotatef(cameraRot.y, 1,0,0);
        //glRotatef(cameraRot.x, 0,1,0);

        //glTranslatef(-cameraPos.x,-cameraPos.y,-cameraPos.z);
        //glTranslatef(0,-xAngle,0);
      //glTranslatef(0.0, -xAngle, 0.0);
    //glRotatef(xAngle*10,1,0,0);
        //cout<<xAngle<<endl;
        //glRotatef(90,1,0,0);
        glRotatef(cameraRot.y, 1,0,0);
        glRotatef(cameraRot.x, 0,1,0);
        //glTranslatef(0,-100,0);
        glScalef(10, 10, 10);

      glTranslatef(-cameraPos.x, -cameraPos.y, -cameraPos.z);

      glScalef(1,1,-1);
      //glRotatef(cameraRot.x, 0,1,0);
      //glRotatef(camer11aRot.y, 1,0,0);


      /* rotate by X, Y, and Z angles */
      /*glRotatef(xAngle, 0.1, 0.1, 0.0);
      glRotatef(yAngle, 0.0, 0.1, 0.0);
      glRotatef(zAngle, 0.0, 0.0, 1.0);*/
      xAngle +=0.1;

      recalcModelView = GL_FALSE;
      needRedraw = GL_TRUE;
    }
    if (needRedraw)
    {
        timeval newTime;
        gettimeofday(&newTime, 0);
        //float timeB = (int)newTime.tv_sec + (float)newTime.tv_usec / 1000000.0;
        //float timeB = (long long)newTime.tv_sec;
        //float timeE = lastTime.tv_sec + (float)lastTime.tv_usec / 1000000.0;
        //float delta =  0;//newTime.tv_sec;// - lastTime.tv_sec;
        //delta += newTime.tv_usec / 1000000.0;
        //delta -= lastTime.tv_sec;
        //delta -= lastTime.tv_usec / 1000000.0;
//        delta +=
        //delta /= 1000000;
        int secB = newTime.tv_sec * 1000000 + newTime.tv_usec;
        int secE = lastTime.tv_sec * 1000000 + lastTime.tv_usec;
        float delta = secB - secE;
        delta /= 1000000.0;

        //moveCamera(delta);

        delta *= MULTIPLY_TIME;
        if (delta > MAX_DELTA * MULTIPLY_TIME) delta = MAX_DELTA * MULTIPLY_TIME;
        if (delta < MIN_DELTA * MULTIPLY_TIME) delta = MIN_DELTA * MULTIPLY_TIME;
        if (delta > 0.4) delta = 0.4;
        //cout<<"                  "<<fixed<<newTime.tv_usec<<"  "<<timeE<<"  "<<timeB<<"   "<<delta<<endl;
        //cout<<delta<<endl;
        for(int i=0;i<REAL_INT_MULTIPLY;i++ )
        {
            update(delta);
        }
        lastTime = newTime;
      redraw();
      needRedraw = GL_FALSE;


    }
  }
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
    //if(isKeyPressed(XK_Shift_L))multiplyMove *= 5;

    //multiplyMove *= delta;

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

                GameObject *temp;
                temp = new Cross(v1);
                temp->gameEngine = this;
                temp->id = id;
                objects.push_back(temp);
                crosses.push_back(dynamic_cast<Cross*>(temp));

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

                objects.push_back((GameObject*)temp);



                cout<<"dodano garage: "<<id<<endl;
            }
            if (type.compare("CL") == 0)
            {
                float x1,y1,z1;
                //float x2,y2,z2;
                file >> x1 >> y1 >> z1;// >> x1 >> y2 >> z2;
                Vec3 v1(x1,y1,z1);
                //Vec3 v2(x2,y2,z2);

                GameObject *temp;
                temp = new CrossLights(v1);
                temp->gameEngine = this;
                temp->id = id;
                objects.push_back(temp);
                crosses.push_back(dynamic_cast<Cross*>(temp));

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
            string name, mode;
            int number; //of streets in the cross

            file >> name >> mode;

            if (mode.compare("DEFAULT") == 0)
            {
                file >> number;

                string ids[4];

                for(int i=0;i<number;i++)
                {
                    file >> ids[i];
                }

                Cross *cross = dynamic_cast<Cross*>(findObjectByName(name));
                cross->setDefaultPriority(dynamic_cast<Road*>(findObjectByName(ids[0])),
                                          dynamic_cast<Road*>(findObjectByName(ids[1])),
                                          dynamic_cast<Road*>(findObjectByName(ids[2])),
                                          dynamic_cast<Road*>(findObjectByName(ids[3])));
            }
        }
    }
    file.close();
}

void Simulator::update(float delta)
{
    for(unsigned int i=0;i<objects.size();i++)
    {
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
            delete go;
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

void registerNewObject(Simulator *engine, GameObject *go)
{
    engine->registerObject(go);
}

void destroyNextObject(Simulator *engine, GameObject *go)
{
    engine->destroyObject(go);
}

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
