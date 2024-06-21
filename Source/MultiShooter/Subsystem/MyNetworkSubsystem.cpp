#include "Subsystem/MyNetworkSubsystem.h"
#include "Network/ClientSocket.h"
#include <fstream>
#include "Network/AttackPlayerPacket.h"
#include "Network/TCPClient.h"
using namespace boost::asio;
UMyNetworkSubsystem::UMyNetworkSubsystem()
{
	std::function<void(char*, uint16)> attackPlayer =
		std::bind(&UMyNetworkSubsystem::S_AttackPlayerHandler, this, std::placeholders::_1, std::placeholders::_2);
	handlers.insert(std::make_pair(PacketID::C_AttackPlayer, attackPlayer));
}

UMyNetworkSubsystem::~UMyNetworkSubsystem()
{
	ServerDisConnect();
}
void UMyNetworkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	 pool = new boost::asio::thread_pool(4); // 4 threads
	 boost::asio::post(*pool, std::bind(&UMyNetworkSubsystem::StartConnect, this));
}



void UMyNetworkSubsystem::StartConnect()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	FString::Printf(TEXT("Start")));
	
	ip::tcp::endpoint endpoints(
	ip::address::from_string(SERVER_IP),
	PORT);
	
	client = new TCPClient(std::bind(&UMyNetworkSubsystem::ReceivePacket, this, std::placeholders::_1, std::placeholders::_2));
	client->Start(endpoints);
}

// 헤더 2바이트를 제외한 나머지를 다 받는다.
void UMyNetworkSubsystem::ReceivePacket(char* buf, uint16 size)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple,
FString::Format(TEXT("Callback : {0}"), {size}));
	int count = 0;
	
	int packetID;
	memcpy(&packetID, buf + count, sizeof(int));
	count += sizeof(int);

	uint16 playerID;
	memcpy(&playerID, buf + count, sizeof(uint16));
	count += sizeof(uint16);
	
	uint16 damagedPlayer;
	memcpy(&damagedPlayer, buf + count, sizeof(uint16));
	count += sizeof(uint16);

	uint16 damageAmount;
	memcpy(&damageAmount, buf + count, sizeof(uint16));
	count += sizeof(uint16);
	
	uint16 dist;
	memcpy(&dist, buf + count, sizeof(uint16));
	count += sizeof(uint16);

	FString f = FString::Format(TEXT("{0} {1} {2} {3} {4}"), {packetID, playerID, damagedPlayer, damageAmount, dist});
	GEngine->AddOnScreenDebugMessage(-1,3, FColor::Purple, f);
}

void UMyNetworkSubsystem::ServerDisConnect()
{
	if(client)
	{
		delete client;
	}
	if(pool)
	{
		pool->stop();
		delete pool;
	}
}

void UMyNetworkSubsystem::S_AttackPlayerHandler(char* buf, uint16 size)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple,
FString::Printf(TEXT("Attack")));
}

void UMyNetworkSubsystem::SendPacket(NetPacket* packet)
{
	if(!client)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Not Connected"));
		return;
	}
	char* buffer = new char[BUFSIZE];
	char* buf = buffer; 
	buf = packet->GenerateSendPacket(buf);
	size_t totalBytes = buf - buffer;
	// 버퍼 내용을 문자열로 변환하여 출력
	std::string readableStr = BufferToString(buffer, totalBytes);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(readableStr.c_str()));
	client->Send(buffer, packet->_PacketSize);
	delete[] buffer;
}

std::string UMyNetworkSubsystem::BufferToString(const char* buf, size_t length) {
	std::ostringstream oss;
	for (size_t i = 0; i < length; ++i) {
		oss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buf[i];
		if (i % 2 == 1 && i < length - 1) { // 각 uint16 값 사이에 공백을 추가
			oss << " ";
		}
	}
	return oss.str();
}

