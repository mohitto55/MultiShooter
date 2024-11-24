using ServerCore;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using UnitySQ;

namespace GNP_Unity_Server.Packets
{
    public class S_AttackPlayer : IPacket, IPacketHandler
    {
        public ushort playerId;
        public ushort damagedPlayer;
        public ushort damageAmount;
        public ushort dist;

        public ushort ID { get { return (ushort)PacketID.C_AttackPlayer; } }

        public bool PacketHandle()
        {
            if(dist < 500)
            {
                return false;
            }
            return true;
        }

        public void Read(ArraySegment<byte> segment)
        {
            Program.MyConsoleWriteLine("AttackPlayer 시작!");
            ushort count = 0;
            count += sizeof(ushort); // 패킷 크기
            count += sizeof(int); // 패킷 번호
            count += sizeof(ushort); // 플레이어 아이디
            this.damagedPlayer = BitConverter.ToUInt16(segment.Array, segment.Offset + count);
            count += sizeof(ushort);
            this.damageAmount = BitConverter.ToUInt16(segment.Array, segment.Offset + count);
            count += sizeof(ushort);
            this.dist = BitConverter.ToUInt16(segment.Array, segment.Offset + count);
            count += sizeof(ushort);
            Program.MyConsoleWriteLine("AttackPlayer / 공격 받는 사람 : " + damagedPlayer + " / 총합 : " + damageAmount + " / 거리 : " + dist);
            count += sizeof(UInt32);
        }

        public ArraySegment<byte> Write()
        {
            ArraySegment<byte> segment = SendBufferHelper.Open(4096);
            ushort count = 0;

            // 데이터 직렬화
            count += sizeof(ushort);
            Array.Copy(BitConverter.GetBytes((int)PacketID.S_AttackPlayer), 0, segment.Array, segment.Offset + count, sizeof(ushort));
            count += sizeof(int);
            Array.Copy(BitConverter.GetBytes(this.playerId), 0, segment.Array, segment.Offset + count, sizeof(int));
            count += sizeof(ushort);
            Array.Copy(BitConverter.GetBytes(this.damagedPlayer), 0, segment.Array, segment.Offset + count, sizeof(float));
            count += sizeof(ushort);
            Array.Copy(BitConverter.GetBytes(this.damageAmount), 0, segment.Array, segment.Offset + count, sizeof(float));
            count += sizeof(ushort);
            Array.Copy(BitConverter.GetBytes(this.dist), 0, segment.Array, segment.Offset + count, sizeof(float));
            count += sizeof(ushort);
            Array.Copy(BitConverter.GetBytes(count), 0, segment.Array, segment.Offset, sizeof(ushort)); // 패킷 크기는 마지막에 입력

            return SendBufferHelper.Close(count);
        }
    }
}
