using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketTester
{
    class ImuFrame
    {
        public float Quaternion_x;
        public float Quaternion_y;
        public float Quaternion_z;
        public float Quaternion_w;
        public UInt16 Magnetic_x;
        public UInt16 Magnetic_y;
        public UInt16 Magnetic_z;
        public UInt16 Acceleration_x;
        public UInt16 Acceleration_y;
        public UInt16 Acceleration_z;
        public UInt16 Rotation_x;
        public UInt16 Rotation_y;
        public UInt16 Rotation_z;

        public ImuFrame(RawPacket packet)
        {
            if (packet.PayloadSize >= 37)
            {
                Quaternion_x = BitConverter.ToSingle(packet.Payload, 0+3);
                Quaternion_y = BitConverter.ToSingle(packet.Payload, 4 + 3);
                Quaternion_z = BitConverter.ToSingle(packet.Payload, 8 + 3);
                Quaternion_w = BitConverter.ToSingle(packet.Payload, 12 + 3);
                Magnetic_x = BitConverter.ToUInt16(packet.Payload, 16 + 3);
                Magnetic_y = BitConverter.ToUInt16(packet.Payload, 18 + 3);
                Magnetic_z = BitConverter.ToUInt16(packet.Payload, 20 + 3);
                Acceleration_x = BitConverter.ToUInt16(packet.Payload, 22 + 3);
                Acceleration_y = BitConverter.ToUInt16(packet.Payload, 24 + 3);
                Acceleration_z = BitConverter.ToUInt16(packet.Payload, 26 + 3);
                Rotation_x = BitConverter.ToUInt16(packet.Payload, 28 + 3);
                Rotation_y = BitConverter.ToUInt16(packet.Payload, 30 + 3);
                Rotation_z = BitConverter.ToUInt16(packet.Payload, 32 + 3);
            }
        }
        override public string ToString()
        {
            //return this.roll.ToString("F6") + ";0.0;0.0";
            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(String.Format("Qx{0}:Qy{1}:Qz{2}:Qw{3}\r\n", Quaternion_x, Quaternion_y, Quaternion_z, Quaternion_w));
            strBuilder.Append(String.Format("Mx{0}:My{1}:Mz{2}\r\n", Magnetic_x, Magnetic_y, Magnetic_z));
            strBuilder.Append(String.Format("Ax{0}:Ay{1}:Az{2}\r\n", Acceleration_x, Acceleration_y, Acceleration_z));
            strBuilder.Append(String.Format("Rx{0}:Ry{1}:Rz{2}\r\n", Rotation_x, Rotation_y, Rotation_z));
            return strBuilder.ToString();
        }


    }
}
