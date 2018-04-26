/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */

#include"Simulator.h"
using namespace std;

int Simulator::width = 1280;
int Simulator::height = 720;

int Simulator::snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
int Simulator::dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *Simulator::dpy = NULL;
Window    Simulator::win = (Window) NULL;
GLboolean Simulator::doubleBuffer = GL_TRUE;

void fatalError(char *message)
{
  fprintf(stderr, "main: %s\n", message);
  exit(1);
}

void Simulator::redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

        /*Graphics g;
        g.setColor(0,0,0);
        g.drawCube(1);*/

        for (int i=0;i<objects.size();i++)
        {
            objects[i]->drawObject();
        }

    glPopMatrix();

    /* front face */
    /*glBegin(GL_QUADS);
    glPushMatrix();
        glScalef(0.5,0.5,0.5);
      glColor3f(0.0, 0.7, 0.1);  /* green */
      /*glVertex3f(-1.0, 1.0, 1.0);
      glVertex3f(1.0, 1.0, 1.0);
      glVertex3f(1.0, -1.0, 1.0);
      glVertex3f(-1.0, -1.0, 1.0);

      /* back face */
      //glColor3f(0.9, 1.0, 0.0);  /* yellow */
      /*glVertex3f(-1.0, 1.0, -1.0);
      glVertex3f(1.0, 1.0, -1.0);
      glVertex3f(1.0, -1.0, -1.0);
      glVertex3f(-1.0, -1.0, -1.0);

      /* top side face */
      //glColor3f(0.2, 0.2, 1.0);  /* blue */
      /*glVertex3f(-1.0, 1.0, 1.0);
      glVertex3f(1.0, 1.0, 1.0);
      glVertex3f(1.0, 1.0, -1.0);
      glVertex3f(-1.0, 1.0, -1.0);

      /* bottom side face */
      //glColor3f(0.7, 0.0, 0.1);  /* red */
      /*glVertex3f(-1.0, -1.0, 1.0);
      glVertex3f(1.0, -1.0, 1.0);
      glVertex3f(1.0, -1.0, -1.0);
      glVertex3f(-1.0, -1.0, -1.0);
    glPopMatrix();
    glEnd();*/

  if (doubleBuffer)
    glXSwapBuffers(dpy, win);/* buffer swap does implicit glFlush */
  else
    glFlush();  /* explicit flush for single buffered case */
}

int Simulator::init(int argc, char **argv)
{
  XVisualInfo         *vi;
  Colormap             cmap;
  XSetWindowAttributes swa;
  GLXContext           cx;

  int                  dummy;

  /*** (1) open a connection to the X server ***/

  dpy = XOpenDisplay(NULL);
  if (dpy == NULL)
    fatalError("could not open display");

  /*** (2) make sure OpenGL's GLX extension supported ***/

  if(!glXQueryExtension(dpy, &dummy, &dummy))
    fatalError("X server has no OpenGL GLX extension");

  /*** (3) find an appropriate visual ***/

  /* find an OpenGL-capable RGB visual with depth buffer */
  vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
  if (vi == NULL)
  {
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
    if (vi == NULL) fatalError("no RGB visual with depth buffer");
    doubleBuffer = GL_FALSE;
  }
  if(vi->c_class != TrueColor)
    fatalError("TrueColor visual required for this program");

  /*** (4) create an OpenGL rendering context  ***/

  /* create an OpenGL rendering context */
  cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                        /* direct rendering if possible */ GL_TRUE);
  if (cx == NULL)
    fatalError("could not create rendering context");

  /*** (5) create an X window with the selected visual ***/

  /* create an X colormap since probably not using default visual */
  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask    | ExposureMask
                 | ButtonPressMask | StructureNotifyMask;
  win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                      1280, 720, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &swa);
  XSetStandardProperties(dpy, win, "main", "main", None,
                         argv, argc, NULL);

  /*** (6) bind the rendering context to the window ***/

  glXMakeCurrent(dpy, win, cx);

  /*** (7) request the X window to be displayed on the screen ***/

  XMapWindow(dpy, win);

  /*** (8) configure the OpenGL context for rendering ***/

  glEnable(GL_DEPTH_TEST); /* enable depth buffering */
  glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
  glClearDepth(1.0);       /* pedantic, 1.0 is the default */

  initLight();

  /* frame buffer clears should be to black */
  glClearColor(0.0, 0.0, 0.0, 0.0);

    float screenRatio = width / height *2.0;

  /* set up projection transform */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0 * screenRatio, 1.0 * screenRatio, -1.0, 1.0, 20.0, 1000.0);
  /* establish initial viewport */
  /* pedantic, full window size is default viewport */
  glViewport(0, 0, 1280, 720);

  printf( "Press left mouse button to rotate around X axis\n" );
  printf( "Press middle mouse button to rotate around Y axis\n" );
  printf( "Press right mouse button to rotate around Z axis\n" );
  printf( "Press ESC to quit the application\n" );

  /*** (9) dispatch X events ***/



  return 0;
}

Simulator::Simulator()
{
    if (!didInit)
    {
        //todo throw
    }
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
    /*do
    {
      /*XNextEvent(dpy, &event);
      switch (event.type)
      {
        case KeyPress:
        {
          KeySym     keysym;
          XKeyEvent *kevent;
          char       buffer[1];
          /* It is necessary to convert the keycode to a
           * keysym before checking if it is an escape */
          /*kevent = (XKeyEvent *) &event;
          if (   (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1)
              && (keysym == (KeySym)XK_Escape) )
            exit(0);
          break;
        }
        case ButtonPress:
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
        case ConfigureNotify:
          glViewport(0, 0, event.xconfigure.width,
                     event.xconfigure.height);
          /* fall through... */
        /*case Expose:
          needRedraw = GL_TRUE;
          break;
      }
    } while(XPending(dpy)); /* loop to compress events */

    recalcModelView = GL_TRUE;
    //if (recalcModelView)
    {
      glMatrixMode(GL_MODELVIEW);

      /* reset modelview matrix to the identity matrix */
      glLoadIdentity();
        glClearColor(1,1,1,1);
      /* move the camera back three units */
      glTranslatef(0,0,-20);
      glRotatef(-90,1,0,0);
      glTranslatef(0.0, 100.0, 0.0);
      //glRotatef(90,0,0,1);


      /* rotate by X, Y, and Z angles */
      //glRotatef(xAngle, 0.1, 0.1, 0.0);
      /*glRotatef(yAngle, 0.0, 0.1, 0.0);
      glRotatef(zAngle, 0.0, 0.0, 1.0);*/
      //xAngle +=0.1;

      recalcModelView = GL_FALSE;
      needRedraw = GL_TRUE;
    }
    if (needRedraw)
    {
        timeval newTime;
        gettimeofday(&newTime, 0);
        //float timeB = (int)newTime.tv_sec + (float)newTime.tv_usec / 1000000.0;
        float timeB = (long long)newTime.tv_sec;
        float timeE = lastTime.tv_sec + (float)lastTime.tv_usec / 1000000.0;
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
        delta *= MULTIPLY_TIME;
        //cout<<"                  "<<fixed<<newTime.tv_usec<<"  "<<timeE<<"  "<<timeB<<"   "<<delta<<endl;
        //cout<<delta<<endl;
        update(delta);
        lastTime = newTime;
      redraw();
      needRedraw = GL_FALSE;
    }
  }
}

void Simulator::initLight()
{
    const GLfloat lambient[]  = { 0.3,0.3,0.3, 1.0f };
    const GLfloat ldiffuse[]  = { 1,1,1, 1.0f };    //111
    const GLfloat lspecular[] = { 0,0,0, 1.0f };    //111
    const GLfloat lposition[] = { 30,30,30, 0.0f };

    const GLfloat mambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mdiffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mspecular[]   = { 0,0,0, 1.0f };               //111
    const GLfloat shininess[] = { 100 };

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  lambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  ldiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lposition);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mspecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
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
                float x2,y2,z2;
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

                for (int i=0;i<crosses.size();i++)
                {
                    if (crosses[i]->id.compare(begCrossID) == 0)
                    {
                        crossB = crosses[i];
                        break;
                    }
                }
                for (int i=0;i<crosses.size();i++)
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
                float x,y,z;
                file >> jointCross >> x >> y >> z;
                Vec3 v(x,y,z);
                Cross *cross;
                for(int i=0;i<objects.size();i++)
                {
                    if (crosses[i]->id.compare(jointCross) == 0)
                    {
                        cross = crosses[i];
                        break;
                    }
                }
                GameObject *temp;
                temp = new Garage(this, v, cross);
                temp->id = id;
                objects.push_back(temp);

                cout<<"dodano garage: "<<id<<endl;
            }
        }
    }
    file.close();
}

void Simulator::update(float delta)
{
    for(int i=0;i<objects.size();i++)
    {
        objects[i]->updateObject(delta);
    }
}

void Simulator::registerObject(GameObject *go)
{
    objects.push_back(go);
}

void registerNewObject(Simulator *engine, GameObject *go)
{
    engine->registerObject(go);
}
