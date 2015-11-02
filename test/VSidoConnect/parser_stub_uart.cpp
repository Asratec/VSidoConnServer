#include <list>
#include <map>
#include <chrono>
#include <tuple>
using namespace std;



list<unsigned char> sendBuffer;
list<unsigned char> readBuffer;
map<int,tuple<list<unsigned char>,chrono::milliseconds>> ut_uart_read;


namespace VSido
{
/** 送信する
* @param data 送信データ
* @return None
*/
void sendUart(list<unsigned char> data)
{
	sendBuffer = data ;
}

/** UARTを受信する。 
* @return 受信データ
*/
map<int,tuple<list<unsigned char>,chrono::milliseconds>> readUart()
{
	return ut_uart_read;
}


} // namespace VSido

