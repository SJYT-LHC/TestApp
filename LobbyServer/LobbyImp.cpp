#include "LobbyImp.h"
#include "servant/Application.h"
using namespace std;

//////////////////////////////////////////////////////
void LobbyImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void LobbyImp::destroy()
{
    //destroy servant here:
    //...
}


int LobbyImp::doRequest(tars::TarsCurrentPtr current, vector<char>& response)
{   LOG->debug() << "main init succ" << endl;
    printf("=======doRequest=====\n");
	//返回给客户端它自己请求的数据包，即原包返回
	const vector<char>& request = current->getRequestBuffer();
	response = request;
    string stBuf;
    stBuf.assign(response.begin(), response.end());
    cout<< "response:========"<<stBuf<<endl;
    LOG->debug() << "response:========:"<< stBuf<< ",RequestId:" << current->getRequestId()<< endl;

	return 0;
}
//客户端关闭到服务端的连接，或者服务端发现客户端长时间未发送包过来，然后超过60s就关闭连接
//调用的方法
int LobbyImp::doClose(tars::TarsCurrentPtr current)
{


	return 0;
}


