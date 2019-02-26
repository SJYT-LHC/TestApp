#ifndef _HelloServer_H_
#define _HelloServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class LobbyServer : public Application
{
public:
    /**
     *
     **/
    virtual ~LobbyServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern LobbyServer g_app;

////////////////////////////////////////////
#endif
