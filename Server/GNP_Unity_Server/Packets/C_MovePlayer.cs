using ServerCore;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnitySQ;

namespace GNP_Unity_Server.Packets
{
    public class C_MovePlayer : IPacket
    {
        public ushort playerId;
        public float x;
        public float y;
        public float z;
        public ushort speed;

        public ushort ID { get { return (ushort)PacketID.C_MovePlayer; } }

        public void Read(ArraySegment<byte> segment)
        {
            Program.MyConsoleWriteLine("GetSocre 시작!");
            ushort count = 0;
            count += sizeof(ushort); // 패킷 크기
            count += sizeof(int); // 패킷 번호
            count += sizeof(ushort); // 플레이어 아이디

            this.x = BitConverter.ToSingle(segment.Array, segment.Offset + count);
            count += sizeof(float);
            this.y = BitConverter.ToSingle(segment.Array, segment.Offset + count);
            count += sizeof(float);
            this.z = BitConverter.ToSingle(segment.Array, segment.Offset + count);
            count += sizeof(float);
            this.speed = BitConverter.ToUInt16(segment.Array, segment.Offset + count);
            count += sizeof(ushort);
            Program.MyConsoleWriteLine("현재 포지션 : " + x + " / " + y + " / " + z + " / 속도 : " + speed);
        }

        public ArraySegment<byte> Write()
        {
            ArraySegment<byte> segment = SendBufferHelper.Open(4096);
            ushort count = 0;

            // 데이터 직렬화
            //count += sizeof(ushort);
            //Array.Copy(BitConverter.GetBytes((ushort)PacketID.C_GETSCORE), 0, segment.Array, segment.Offset + count, sizeof(ushort));
            //count += sizeof(ushort);
            //Array.Copy(BitConverter.GetBytes(this.playerId), 0, segment.Array, segment.Offset + count, sizeof(int));
            //count += sizeof(int);
            //Array.Copy(BitConverter.GetBytes(this.score), 0, segment.Array, segment.Offset + count, sizeof(float));
            //count += sizeof(float);

            Array.Copy(BitConverter.GetBytes(count), 0, segment.Array, segment.Offset, sizeof(ushort)); // 패킷 크기는 마지막에 입력

            return SendBufferHelper.Close(count);
        }
    }
}
