#include <iostream>
#include "servant/Communicator.h"
#include "Hello.h"

using namespace std;
using namespace TestApp;
using namespace tars;

class HelloCallBack : public HelloPrxCallback
{
public:
    HelloCallBack(){}

    virtual ~HelloCallBack(){}

    virtual void callback_testHello(tars::Int32 ret,  const std::string& sRsp)
    {
        cout<<"callback_testHello ret:"<< ret << "|sRsp:" << sRsp <<endl; 
    }

    virtual void callback_testHello_exception(tars::Int32 ret)
    {
        cout<<"callback_testHello_exception ret:"<< ret <<endl;
    }
};

int main(int argc,char ** argv)
{
    Communicator comm;

    try
    {
        HelloPrx prx;
        comm.stringToProxy("TestApp.HelloServer.HelloObj@tcp -h 192.168.157.130 -p 10008" , prx);

        try
        {
            string sReq("hello world");
            HelloPrxCallbackPtr cb = new HelloCallBack();
            prx->async_testHello(cb, sReq);
            cout<<" sReq:"<<sReq<<endl;
        }
        catch(exception &ex)
        {
            cerr<<"ex:"<<ex.what() <<endl;
        }
        catch(...)
        {
            cerr<<"unknown exception."<<endl;
        }
    }
    catch(exception& e)
    {
        cerr<<"exception:"<<e.what() <<endl;
    }
    catch (...)
    {
        cerr<<"unknown exception."<<endl;
    }

    getchar();

    return 0;
}