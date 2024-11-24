using MySql.Data;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using GNP_Unity_Server;
using System.Text.RegularExpressions;
using MySqlX.XDevAPI;
using Newtonsoft.Json.Linq;
using MySql;
using MySql.Data.MySqlClient;
using System.Runtime.Remoting.Messaging;
using GNP_Unity_Server.Packets;

namespace UnitySQ
{
    class Program
    {
        public const int BUFFSIZE = 4096;
        public const int PORT = 12000;
        public static Socket server;
        private static MySqlConnection sqlConn;
        private static PacketManager packetManager;

        public const string UserTable = "users";
        public const string RecordTable = "record";

        // 서버로 부터 받는 명령어를 처리하고 처리 값을 반환하는 함수를 담는 테이블
        public static string[] ExcuteSQL(string cmd)
        {
            List<string> data = new List<string>();
            try
            {
                MySqlCommand sqlCmd = new MySqlCommand(cmd, sqlConn);
                using (MySqlDataReader reader = sqlCmd.ExecuteReader())
                {
                    int k = 0;
                    while (reader.Read())
                    {
                        for (int i = 0; i < reader.FieldCount; i++)
                        {
                            data.Add(String.Format("{0}", reader[0]));
                        }
                    }

                }
            }
            catch (Exception e){
                MyConsoleWriteLine(e.Message);
            }
            return data.ToArray();
        }
        public static string[][] ExcuteSQLColumn(string cmd)
        {
            List<string[]> data = new List<string[]>();
            try
            {
                MySqlCommand sqlCmd = new MySqlCommand(cmd, sqlConn);
                using (MySqlDataReader reader = sqlCmd.ExecuteReader())
                {
                    int k = 0;
                    while (reader.Read())
                    {
                        data.Add(new string[reader.FieldCount]);
                        for (int i = 0; i < reader.FieldCount; i++)
                        {
                            data[k][i] = String.Format("{0}", reader[i]);
                        }
                        k++;
                    }

                }
            }
            catch (Exception e)
            {
                MyConsoleWriteLine(e.Message);
            }
            return data.ToArray();
        }
        static void Main(string[] args)
        {
            // AWS EC2에서 작동중인 Docker>MySQL로 연결한다.
            string mySQLServerDNS = "ec2-15-165-142-97.ap-northeast-2.compute.amazonaws.com";
            string strConn = "host=" + mySQLServerDNS + ";Port=3306;Database=ckgame;UserName=root;Pwd=5865";

            sqlConn = new MySqlConnection(strConn);
            sqlConn.Open();

            // 패킷 핸들러
            packetManager = new PacketManager();

            iocpServerOpen();
            Console.ReadLine();
        }

        /// <summary>
        /// 서버 오픈 후 클라이언트 접속시 콜백 메서드로 접속 처리르 해준다.
        /// </summary>
        static void iocpServerOpen()
        {
            try
            {
                server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, PORT);
                server.Bind(endPoint);
                server.Listen(10);

                SocketAsyncEventArgs sockAsync = new SocketAsyncEventArgs();
                // Completed에 이벤트 핸들러 객체를 연결해주고 AcceptAsync를 호출할 때 해당 객체를 파라미터로 넘긴다.
                sockAsync.Completed += new EventHandler<SocketAsyncEventArgs>(AcceptClient);
                // AcceptAsync 메서드 호출하면 서버가 대기 상태에 있다가 클라이언트가 접속하는 순간 콜백 메서드로 통지가 온다.
                // 이렇게 함으로써 프로그램이 멈추지 않고 계속 작업이 가능하다.
                MyConsoleWriteLine("서버 오픈");
                server.AcceptAsync(sockAsync);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
                server.Disconnect(true);
                MyConsoleWriteLine("접속종료");
            }
        }
        // 클라이언트에게 받은 메시지 처리하기
        static void receiveAsync_Completed(object sender, SocketAsyncEventArgs e)
        {
            MyConsoleWriteLine("클라이언트가 데이터를 보냄");
            Socket svrSocket = (Socket)sender;
            Socket clientSocket = (Socket)e.UserToken;
            if (e.BytesTransferred > 0 && e.SocketError == SocketError.Success)
            {
                try
                {
                    byte[] recvBtye = new byte[BUFFSIZE];
                    // 클라이언트의 메시지는 e.Buffer안에 있다.
                    recvBtye = e.Buffer;
                    // 커맨드|파라미터
                    https://velog.io/@yarogono/C-ArraySegment%EB%9E%80
                    ArraySegment<byte> recvArrayBuf = new ArraySegment<byte>(e.Buffer);
                    // 패킷 매니저 내부에서 핸들러를 통해 패킷을 처리해준다.
                    packetManager.OnRecvPacket(recvArrayBuf, clientSocket);
                    // 계속 받을 준비하기
                    SocketAsyncEventArgs receiveAsync = new SocketAsyncEventArgs();
                    receiveAsync.Completed += new EventHandler<SocketAsyncEventArgs>(receiveAsync_Completed);
                    receiveAsync.SetBuffer(new byte[BUFFSIZE], 0, BUFFSIZE);
                    receiveAsync.UserToken = clientSocket;
                    clientSocket.ReceiveAsync(receiveAsync);
                }
                catch (Exception ex)
                {
                    MyConsoleWriteLine(ex.ToString());
                    MyConsoleWriteLine("===================오류 발생===================");

                    Close_ClientSocket(clientSocket);
                }
            }
            else
            {
                Close_ClientSocket(clientSocket);
            }
        }
        static void Close_ClientSocket(Socket e)
        {
            MyConsoleWriteLine("유저 종료");
        }
        // 클라이언트 최초 접속시 실행하는 함수
        static void AcceptClient(object sender, SocketAsyncEventArgs e)
        {
            try
            {
                Socket server = (Socket)sender;
                Socket client = e.AcceptSocket;


                MyConsoleWriteLine("클라이언트가 접속했습니다 IP : {0}", client.RemoteEndPoint.ToString());
                byte[] name = new byte[BUFFSIZE];

                //16진수 -> string형으로 parsing
                String str_data = Encoding.UTF8.GetString(name);
                // 클라이언트로 부터 메시지 받기
                SocketAsyncEventArgs receiveAsync = new SocketAsyncEventArgs();
                receiveAsync.Completed += new EventHandler<SocketAsyncEventArgs>(receiveAsync_Completed);
                receiveAsync.SetBuffer(new byte[BUFFSIZE], 0, BUFFSIZE);
                receiveAsync.UserToken = client;
                client.ReceiveAsync(receiveAsync);
                StringBuilder sb = new StringBuilder();

                // 지속적으로 유저 받을 준비하기
                e.AcceptSocket = null;
                server.AcceptAsync(e);
            }
            catch (Exception ex)
            {
                MyConsoleWriteLine("Error : " + ex.Message);
            }
        }

        // 테이블의 행 요소의 이름을 가져온다.
        public static void MyConsoleWriteLine(string msg, params string[] arg)
        {
            DateTime nowDate = DateTime.Now;
            string format = "yyyy.MM.dd HH:mm:ss";
            string date = '[' + nowDate.ToString(format) + ']';           
            Console.WriteLine(date + msg, arg);
        }

        public static string[] GetAllPlayerID()
        {
            List<string> list = new List<string>();
            //foreach(var player in dic_client)
            //{
            //    list.Add(player.Key);
            //}
            return list.ToArray();
        }
    }
}
