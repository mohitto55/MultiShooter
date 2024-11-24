using Mysqlx.Crud;
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
    public class C_GetScore : IPacket, IPacketHandler
    {
        public ushort playerId;
        public int score;
        public int wcharSize;
        public string uniqueID;

        public ushort ID { get { return (ushort)PacketID.C_GETSCORE; } }

        public void Read(ArraySegment<byte> segment)
        {
            Program.MyConsoleWriteLine("GetSocre 시작!");
            ushort count = 0;
            count += sizeof(ushort); // 패킷 크기
            count += sizeof(int); // 패킷 번호
            count += sizeof(ushort); // 플레이어 아이디
            this.score = BitConverter.ToInt32(segment.Array, segment.Offset + count);
            count += sizeof(UInt32);
            this.wcharSize = BitConverter.ToInt32(segment.Array, segment.Offset + count);
            count += sizeof(UInt32);
            this.uniqueID = BitConverter.ToString(segment.Array, segment.Offset + count, wcharSize);
            uniqueID = Encoding.Unicode.GetString(segment.Array, segment.Offset + count, wcharSize);
            count += (ushort)wcharSize;
        }

        public ArraySegment<byte> Write()
        {
            ArraySegment<byte> segment = SendBufferHelper.Open(4096);
            ushort count = 0;

            // 데이터 직렬화
            count += sizeof(ushort); 
            Array.Copy(BitConverter.GetBytes((ushort)PacketID.C_GETSCORE), 0, segment.Array, segment.Offset + count, sizeof(ushort));
            count += sizeof(ushort);
            Array.Copy(BitConverter.GetBytes(this.playerId), 0, segment.Array, segment.Offset + count, sizeof(int));
            count += sizeof(int);
            Array.Copy(BitConverter.GetBytes(this.score), 0, segment.Array, segment.Offset + count, sizeof(float));
            count += sizeof(float);

            Array.Copy(BitConverter.GetBytes(count), 0, segment.Array, segment.Offset, sizeof(ushort)); // 패킷 크기는 마지막에 입력

            return SendBufferHelper.Close(count);
        }

        public bool PacketHandle()
        {
            string uid = uniqueID.ToString();
            string record = score.ToString();
            string getUIDcmd = string.Format("Select uid From {0} where uid = '{1}';", Program.UserTable, uid);
            bool isFirstJoin = Program.ExcuteSQL(getUIDcmd).Length != 0 ? true : false;
            
           if (!isFirstJoin)
            {
                Program.MyConsoleWriteLine(uid + "유저가 처음 들어왔습니다.");
                string insertRecodeCmd = string.Format("INSERT INTO {0} (uid) VALUES ('{1}');", Program.UserTable, uid);
                Program.ExcuteSQL(insertRecodeCmd);
            }
            Program.MyConsoleWriteLine(uid + "유저 등록");

            string isUidInRecordCmd = string.Format("Select uid from {0} where uid = '{1}';", Program.RecordTable, uid);
            bool isMyUid = Program.ExcuteSQL(isUidInRecordCmd).Length != 0 ? true : false;
            // 본인이 등록이 안되었으면 본인을 등록한다.
            Program.MyConsoleWriteLine("유저기록 등록을 확인합니다.");
            if (!isMyUid)
            {
                Program.MyConsoleWriteLine("유저가 등록되어 있지 않습니다.");
                string insertRecodeCmd = string.Format("Insert into {0} values('{1}', {2});", Program.RecordTable, uid, record);
                Program.ExcuteSQL(insertRecodeCmd);
                Program.MyConsoleWriteLine(uid + "유저 기록 등록 완료");
            }
            else
            {
                Program.MyConsoleWriteLine("점수 추가.");
                string myTopRecodeCmd = string.Format("Select topRecord from {0} where uid = '{1}';", Program.RecordTable, uid);
                string myTopRecode = Program.ExcuteSQL(myTopRecodeCmd)[0];
                Program.MyConsoleWriteLine("나의 최고기록 : " + myTopRecode);

                string updateCmd = string.Format("Update {0} set topRecord = {1} where uid = '{2}';", Program.RecordTable, int.Parse(myTopRecode) + int.Parse(record), uid);
                Program.ExcuteSQL(updateCmd);
            }
            return false;
        }
    }
}
