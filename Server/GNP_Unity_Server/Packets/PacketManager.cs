using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using UnitySQ;

namespace GNP_Unity_Server.Packets
{
    public class PacketManager
    {
        #region Singleton
        //static PacketManager _instance = new PacketManager();
        //public static PacketManager Instance { get { return _instance; } }
        #endregion

        public PacketManager()
        {
            Register();
        }

        Dictionary<int, Func<PacketID, ArraySegment<byte>, IPacket>> _makeFunc = new Dictionary<int, Func<PacketID, ArraySegment<byte>, IPacket>>(); // 패킷 Id - 해당 Id 패킷 생성 함수
        Dictionary<int, Action<PacketID, IPacket>> _handler = new Dictionary<int, Action<PacketID, IPacket>>(); // 패킷 Id - 해당 Id 패킷 Handle 함수

        // 패킷 별 생성 함수, Handle 함수 등록
        public void Register()
        {
            _makeFunc.Add((int)PacketID.C_GETSCORE, MakePacket<C_GetScore>);
            _handler.Add((int)PacketID.C_GETSCORE, C_GetScoreHandler);
            _makeFunc.Add((int)PacketID.C_AttackPlayer, MakePacket<C_AttackPlayer>);
            _handler.Add((int)PacketID.C_AttackPlayer, C_GetScoreHandler);
            //_makeFunc.Add((ushort)PacketID.C_Move, MakePacket<C_Move>);
            //_handler.Add((ushort)PacketID.C_Move, PacketHandler.C_MoveHandler);
        }
        public void C_GetScoreHandler(PacketID b, IPacket a)
        {

        }
        // 패킷 처리
        public void OnRecvPacket(ArraySegment<byte> buffer, Socket socket)
        {
            ushort count = 0;
            byte[] byteArray = buffer.Array;
            ushort packetSize = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
            count += sizeof(ushort);

            ArraySegment<byte> resizeBuffer = new ArraySegment<byte>(buffer.Array, 0, 8);
            string hexString = BitConverter.ToString(resizeBuffer.Array, 0, packetSize);

            int packetId = BitConverter.ToInt32(buffer.Array, buffer.Offset + count);
            count += sizeof(int);

            ushort playerId = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
            Program.MyConsoleWriteLine("패킷 수신 완료 / Size : " + packetSize.ToString() +
                " / Type : " + packetId.ToString() +
                " / PlayerID : " + playerId.ToString());
            count += sizeof(ushort);
            Func<PacketID, ArraySegment<byte>, IPacket> func = null;

            if (_makeFunc.TryGetValue(packetId, out func))
            {
                PacketID _packetId = (PacketID)packetId;
                // 패킷 생성 후 콜백 함수 실행, 없으면 패킷 처리 함수 실행
                IPacket packet = func.Invoke(_packetId, buffer);

                IPacketHandler handler = packet as IPacketHandler;
                if (handler != null)
                {
                    if (handler.PacketHandle())
                    {
                        //ArraySegment<byte> a = packet.Write();
                        //try
                        //{
                        //    socket.Send(a.Array, a.Count, SocketFlags.None);
                        //}
                        //catch (SocketException ex)
                        //{
                        //    // 예외 처리 코드
                        //    Console.WriteLine("SocketException: " + ex.Message);
                        //}
                    }
                }
            }
        }
        // byte 버퍼로 패킷 생성
        T MakePacket<T>(PacketID session, ArraySegment<byte> buffer) where T : IPacket, new()
        {
            T pkt = new T();
            pkt.Read(buffer);
            return pkt;
        }

        // 패킷 Handle
        public void HandlePacket(int session, IPacket packet)
        {
            Action<PacketID, IPacket> action = null;
            if (_handler.TryGetValue(packet.ID, out action))
            {
                PacketID packetId = (PacketID)session;
                action.Invoke(packetId, packet);
            }
        }
    }
}
