using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GNP_Unity_Server
{
    class MyPacket
    {
        void GenerateBody()
        {
            // 헤더
            short size = 5;
            string data = "Hello";

            byte[] header = BitConverter.GetBytes(size);
            byte[] body = Encoding.UTF8.GetBytes(data);

            byte[] packet = new byte[1024];

            // 패킷 데이터에 헤더와 body 붙이기
            Array.Copy(header, 0, packet, 0, header.Length);
            Array.Copy(body, 0, packet, header.Length, body.Length);
        }
    }
}
