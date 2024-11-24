using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public class SendBuffer
    {
        byte[] _buffer;
        int _usedSize = 0; // Write 커서

        public int FreeSize { get { return _buffer.Length - _usedSize; } }

        public SendBuffer(int chunkSize)
        {
            _buffer = new byte[chunkSize];
        }

        // SendBuffer에 데이터를 담기 전 실행
        public ArraySegment<byte> Open(int reserveSize)
        {
            if (reserveSize > FreeSize)
                return new ArraySegment<byte>();

            // 요청 크기만큼 빈 버퍼 반환
            return new ArraySegment<byte>(_buffer, _usedSize, reserveSize);
        }

        // SendBuffer에 데이터를 담은 후 실행
        public ArraySegment<byte> Close(int usedSize)
        {
            // 데이터가 담긴 버퍼 반환
            ArraySegment<byte> segment = new ArraySegment<byte>(_buffer, _usedSize, usedSize);
            _usedSize += usedSize;
            return segment;
        }
    }
}
