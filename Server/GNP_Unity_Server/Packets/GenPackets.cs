using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GNP_Unity_Server
{
    public enum PacketID
    {
        S_GETSCORE = 0,
        C_GETSCORE = 1,
        S_AttackPlayer = 2,
        C_AttackPlayer = 3,
        S_MovePlayer = 4,
        C_MovePlayer = 5,
        S_JoinPlayer = 6,
        C_JoinPlayer = 7
        //C_GetScore = 1,
        //S_SignUp = 10,
        //C_SignUp = 2,
        //S_SignIn = 3,
        //C_SignIn = 4,
        //S_BroadcastEnterGame = 5,
        //C_LeaveGame = 6,
        //S_BroadcastLeaveGame = 7,
        //S_PlayerList = 8,
        //C_CarMove = 9,
        //S_BroadcastCarMove = 10,
    }

    public interface IPacket
    {
        ushort ID { get; }
        void Read(ArraySegment<byte> segment);
        ArraySegment<byte> Write();
    }

    public interface IPacketHandler
    {
        bool PacketHandle();
    }
}
