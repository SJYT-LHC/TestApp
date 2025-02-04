#ifndef _LobbyImp_H_
#define _LobbyImp_H_

#include "servant/Application.h"

/**
 *
 *
 */
class LobbyImp : public  tars::Servant
{
public:
    /**
     *
     */
    virtual ~LobbyImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};


    //重载Servant的doRequest方法
    int doRequest(tars::TarsCurrentPtr current, vector<char>& response);

    //重载Servant的doClose方法
    int doClose(tars::TarsCurrentPtr current);

};
/////////////////////////////////////////////////////
#endif
