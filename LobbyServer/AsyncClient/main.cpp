#include <iostream>
#include "servant/Communicator.h"

using namespace std;
using namespace tars;

static void printResult(int iRequestId, const string &sResponseStr)
{
	cout << "request id: " << iRequestId << endl;
	cout << "response str: " << sResponseStr << endl;
}
static void printPushInfo(const string &sResponseStr)
{
	cout << "push message: " << sResponseStr << endl;
}



class TestPushCallBack : public ServantProxyCallback
{
public:
	virtual int onDispatch(ReqMessagePtr msg);
};
typedef tars::TC_AutoPtr<TestPushCallBack> TestPushCallBackPtr;

int TestPushCallBack::onDispatch(ReqMessagePtr msg)
{
	if(msg->request.sFuncName == "printResult")
	{
		string sRet;
		cout << "sBuffer: " << msg->response.sBuffer.size() << endl;
		sRet.assign(&(msg->response.sBuffer[0]), msg->response.sBuffer.size());
		printResult(msg->request.iRequestId, sRet);
		return 0;
	}
	else if(msg->response.iRequestId == 0)
	{
		string sRet;
		sRet.assign(&(msg->response.sBuffer[0]), msg->response.sBuffer.size());
		printPushInfo(sRet);
		return 0;
	}
	else
	{
		cout << "no match func!" <<endl;
	}
	return -3;
}



/*
 响应包解码函数，根据特定格式解码从服务端收到的数据，解析为ResponsePacket
*/
static size_t pushResponse(const char* recvBuffer, size_t length, list<ResponsePacket>& done)
{
	size_t pos = 0;
    while (pos < length)
    {
        unsigned int len = length - pos;
        if(len < sizeof(unsigned int))
        {
            break;
        }

        unsigned int iHeaderLen = ntohl(*(unsigned int*)(recvBuffer + pos));

        //做一下保护,长度大于M
        if (iHeaderLen > 100000 || iHeaderLen < sizeof(unsigned int))
        {
            throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
        }

        //包没有接收全
        if (len < iHeaderLen)
        {
            break;
        }
        else
        {
            tars::ResponsePacket rsp;
			rsp.iRequestId = ntohl(*((unsigned int *)(recvBuffer + pos + sizeof(unsigned int))));
			rsp.sBuffer.resize(iHeaderLen - 2*sizeof(unsigned int));
		  ::memcpy(&rsp.sBuffer[0], recvBuffer + pos + 2*sizeof(unsigned int), iHeaderLen - 2*sizeof(unsigned int));

			pos += iHeaderLen;

            done.push_back(rsp);
        }
    }

    return pos;
}
/*
   请求包编码函数，本函数的打包格式为
   整个包长度（字节）+iRequestId（字节）+包内容
*/
static void pushRequest(const RequestPacket& request, string& buff)
{
    unsigned int net_bufflength = htonl(request.sBuffer.size()+8);
    unsigned char * bufflengthptr = (unsigned char*)(&net_bufflength);

    buff = "";
    for (int i = 0; i<4; ++i)
    {
        buff += *bufflengthptr++;
    }

    unsigned int netrequestId = htonl(request.iRequestId);
    unsigned char * netrequestIdptr = (unsigned char*)(&netrequestId);

    for (int i = 0; i<4; ++i)
    {
        buff += *netrequestIdptr++;
    }

    string tmp;
    tmp.assign((const char*)(&request.sBuffer[0]), request.sBuffer.size());
    buff+=tmp;
}



int main(int argc,char ** argv)
{
    Communicator comm;

    try
    {
        ServantPrx prx;
        comm.stringToProxy("TestApp.LobbyServer.LobbyObj@tcp -h 192.168.157.130 -p 11008" , prx);
        ProxyProtocol prot;
        prot.requestFunc = pushRequest;
        prot.responseFunc = pushResponse;
        
        prx->tars_set_protocol(prot);

        try
        {
            string buf("1111heartbeat");
            TestPushCallBackPtr cb = new TestPushCallBack();
            prx->rpc_call_async(prx->tars_gen_requestid(), "printResult", buf.c_str(), buf.length(), cb);

            cout<<" rpc_call_async Over"<<endl;
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
