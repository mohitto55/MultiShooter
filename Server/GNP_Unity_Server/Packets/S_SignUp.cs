using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ServerCore;

namespace GNP_Unity_Server
{
    //public class S_SignUp : IPacket
    //{
    //    public ushort id;
    //    public ushort pw;

    //    public ushort Protocol { get { return (ushort)PacketID.S_SignIn; } }

    //    public void Read(ArraySegment<byte> segment)
    //    {
    //        ushort count = 0;
    //        count += sizeof(ushort); // 패킷 크기
    //        count += sizeof(ushort); // 패킷 번호
    //        this.id = BitConverter.ToUInt16(segment.Array, segment.Offset + count);
    //        count += sizeof(ushort);
    //        this.pw = BitConverter.ToUInt16(segment.Array, segment.Offset + count);
    //        count += sizeof(ushort);
    //    }

    //    public ArraySegment<byte> Write()
    //    {
    //        ArraySegment<byte> segment = SendBufferHelper.Open(4096);
    //        ushort count = 0;

    //        // 데이터 직렬화
    //        count += sizeof(ushort); // 패킷 크기
    //        Array.Copy(BitConverter.GetBytes((ushort)PacketID.S_BroadcastEnterGame), 0, segment.Array, segment.Offset + count, sizeof(ushort));
    //        count += sizeof(ushort);
    //        Array.Copy(BitConverter.GetBytes(this.id), 0, segment.Array, segment.Offset + count, sizeof(int));
    //        count += sizeof(ushort);
    //        Array.Copy(BitConverter.GetBytes(this.pw), 0, segment.Array, segment.Offset + count, sizeof(float));
    //        count += sizeof(ushort);

    //        Array.Copy(BitConverter.GetBytes(count), 0, segment.Array, segment.Offset, sizeof(ushort)); // 패킷 크기는 마지막에 입력
    //        return SendBufferHelper.Close(count);
    //    }
    //}
}
