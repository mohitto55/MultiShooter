using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using ServerCore;
using UnitySQ;

namespace GNP_Unity_Server.Packets
{
    public class S_BroadcastEnterGame //: IPacket
    {
        //    public int playerId;
        //    public float posX;

        //    public ushort ID => (ushort)PacketID.S_BroadcastEnterGame;

        //    public void Read(ArraySegment<byte> segment)
        //    {
        //        Program.MyConsoleWriteLine("리드 시작!");
        //        ushort count = 0;
        //        count += sizeof(ushort); // 패킷 크기
        //        count += sizeof(ushort); // 패킷 번호
        //        this.playerId = BitConverter.ToInt32(segment.Array, segment.Offset + count);
        //        count += sizeof(int);
        //        this.posX = BitConverter.ToSingle(segment.Array, segment.Offset + count);
        //        count += sizeof(float);
        //    }

        //    public ArraySegment<byte> Write()
        //    {
        //        ArraySegment<byte> segment = SendBufferHelper.Open(4096);
        //        ushort count = 0;

        //        // 데이터 직렬화
        //        count += sizeof(ushort); // 패킷 크기
        //        Array.Copy(BitConverter.GetBytes((ushort)PacketID.S_BroadcastEnterGame), 0, segment.Array, segment.Offset + count, sizeof(ushort));
        //        count += sizeof(ushort);
        //        Array.Copy(BitConverter.GetBytes(this.playerId), 0, segment.Array, segment.Offset + count, sizeof(int));
        //        count += sizeof(int);
        //        Array.Copy(BitConverter.GetBytes(this.posX), 0, segment.Array, segment.Offset + count, sizeof(float));
        //        count += sizeof(float);

        //        Array.Copy(BitConverter.GetBytes(count), 0, segment.Array, segment.Offset, sizeof(ushort)); // 패킷 크기는 마지막에 입력

        //        return SendBufferHelper.Close(count);
        //    }
        //}

    }
}
